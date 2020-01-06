/* Core DSP support for i.MX8 audio DSP.
 *
 * Copyright 2019 NXP
 *
 * Author: Diana Cretu <diana.cretu@nxp.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "qemu/osdep.h"
#include "qapi/error.h"
#include "qemu-common.h"
#include "qemu/timer.h"
#include "sysemu/sysemu.h"
#include "hw/boards.h"
#include "hw/loader.h"
#include "hw/hw.h"
#include "exec/memory.h"
#include "exec/address-spaces.h"
#include "hw/sysbus.h"
#include "qemu/error-report.h"
#include "qemu/io-bridge.h"

#include "hw/audio/adsp-dev.h"
#include "hw/adsp/shim.h"
#include "hw/adsp/mu.h"
#include "hw/adsp/log.h"
#include "hw/adsp/byt.h"
#include "hw/adsp/imx8.h"
#include "hw/ssi/ssp.h"
#include "hw/ssi/sai.h"
#include "hw/ssi/esai.h"
#include "hw/dma/dw-dma.h"
#include "mbox.h"
#include "imx8.h"
#include "common.h"

static void adsp_reset(void *opaque)
{

}

static void adsp_pm_msg(struct adsp_dev *adsp, struct qemu_io_msg *msg)
{
}

static int bridge_cb(void *data, struct qemu_io_msg *msg)
{
    struct adsp_dev *adsp = (struct adsp_dev *)data;

    log_text(adsp->log, LOG_MSGQ,
            "msg: id %d msg %d size %d type %d\n",
            msg->id, msg->msg, msg->size, msg->type);

    switch (msg->type) {
    case QEMU_IO_TYPE_REG:
        /* mostly handled by SHM, some exceptions */
        adsp_imx8_mu_msg(adsp, msg);
        break;
    case QEMU_IO_TYPE_IRQ:
        adsp_imx8_irq_msg(adsp, msg);
        break;
    case QEMU_IO_TYPE_PM:
        adsp_pm_msg(adsp, msg);
        break;
    case QEMU_IO_TYPE_DMA:
        dw_dma_msg(msg);
        break;
    case QEMU_IO_TYPE_MEM:
    default:
        break;
    }

    return 0;
}

static void imx8_irq_set(struct adsp_io_info *info, int irq, uint32_t mask)
{
     struct adsp_dev *adsp = info->adsp;
     adsp_set_lvl1_irq(adsp, irq, 1);
}

static void imx8_irq_clear(struct adsp_io_info *info, int irq, uint32_t mask)
{
     struct adsp_dev *adsp = info->adsp;
     adsp_set_lvl1_irq(adsp, irq, 0);
}

struct adsp_dev_ops imx8_ops = {
    .irq_set = imx8_irq_set,
    .irq_clear = imx8_irq_clear,
};

static struct adsp_dev *adsp_init(const struct adsp_desc *board,
    MachineState *machine, const char *name)
{
    struct adsp_dev *adsp;
    uint8_t *ldata;
    size_t lsize;
    int n;

    adsp = g_malloc(sizeof(*adsp));
    adsp->log = log_init(NULL);    /* TODO: add log name to cmd line */
    adsp->desc = board;
    adsp->shm_idx = 0;
    adsp->system_memory = get_system_memory();
    adsp->machine_opts = qemu_get_machine_opts();
    adsp->cpu_model = machine->cpu_model;
    adsp->kernel_filename = qemu_opt_get(adsp->machine_opts, "kernel");
    adsp->ops = &imx8_ops;

    /* initialise CPU */
    if (!adsp->cpu_model) {
        adsp->cpu_model = XTENSA_DEFAULT_CPU_MODEL;
    }

    for (n = 0; n < smp_cpus; n++) {

        adsp->xtensa[n] = g_malloc(sizeof(struct adsp_xtensa));
        adsp->xtensa[n]->cpu = XTENSA_CPU(cpu_create(machine->cpu_type));

        if (adsp->xtensa[n]->cpu == NULL) {
            error_report("unable to find CPU definition '%s'",
                adsp->cpu_model);
            exit(EXIT_FAILURE);
        }

        adsp->xtensa[n]->env = &adsp->xtensa[n]->cpu->env;
        adsp->xtensa[n]->env->sregs[PRID] = n;
        qemu_register_reset(adsp_reset, adsp->xtensa[n]->cpu);

        /* Need MMU initialized prior to ELF loading,
        * so that ELF gets loaded into virtual addresses
        */
        cpu_reset(CPU(adsp->xtensa[n]->cpu));
    }

    adsp_create_memory_regions(adsp);
    adsp_create_io_devices(adsp, NULL);

    /* reset all devices to init state */
    qemu_devices_reset();

    /* initialise bridge to x86 host driver */
    qemu_io_register_child(name, &bridge_cb, (void*)adsp);

    /* load binary file if one is specified on cmd line otherwise finish */
    if (adsp->kernel_filename == NULL) {
        printf(" ** IMX8 EP DSP initialised.\n"
            " ** Waiting for host to load firmware...\n");
        return adsp;
    }

    /* load the binary image and copy to IRAM */
    ldata = g_malloc(ADSP_IMX8_IRAM_SIZE + ADSP_IMX8_SDRAM0_SIZE + ADSP_IMX8_SDRAM1_SIZE);
    lsize = load_image_size(adsp->kernel_filename, ldata,
         ADSP_IMX8_IRAM_SIZE + ADSP_IMX8_SDRAM0_SIZE + ADSP_IMX8_SDRAM1_SIZE);

    adsp_load_modules(adsp, ldata, lsize);

    return adsp;
}

static uint64_t io_read(void *opaque, hwaddr addr,
        unsigned size)
{
    struct adsp_io_info *info = opaque;
    struct adsp_dev *adsp = info->adsp;
    struct adsp_reg_space *space = info->space;

    log_read(adsp->log, space, addr, size,
        info->region[addr >> 2]);

    return info->region[addr >> 2];
}

/* SHIM IO from ADSP */
static void io_write(void *opaque, hwaddr addr,
        uint64_t val, unsigned size)
{
    struct adsp_io_info *info = opaque;
    struct adsp_dev *adsp = info->adsp;
    struct adsp_reg_space *space = info->space;

    info->region[addr >> 2] = val;

    /* omit 0 writes as it fills mbox log */
    if (val == 0)
        return;

    log_write(adsp->log, space, addr, val, size,
         info->region[addr >> 2]);
}

static const MemoryRegionOps mbox_io_ops = {
    .read = io_read,
    .write = io_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static struct adsp_mem_desc imx8_mem[] = {
    {.name = "iram", .base = ADSP_IMX8_DSP_IRAM_BASE,
        .size = ADSP_IMX8_IRAM_SIZE},
    {.name = "dram", .base = ADSP_IMX8_DSP_DRAM_BASE,
        .size = ADSP_IMX8_DRAM_SIZE},
   {.name = "sdram0", .base = ADSP_IMX8_DSP_SDRAM0_BASE,
        .size = ADSP_IMX8_SDRAM0_SIZE},
};


static struct adsp_reg_space imx8_io[] = {
    { .name = "dmac0", .reg_count = ARRAY_SIZE(adsp_gp_dma_map),
        .reg = adsp_gp_dma_map, .irq = IRQ_NUM_EXT_DMAC0,
        .init = &dw_dma_init_dev, .ops = &dw_dmac_ops,
        .desc = {.base = ADSP_IMX8_DMA0_BASE, .size = ADSP_IMX8_DMA0_SIZE},},
   { .name = "esai0", .reg_count = ARRAY_SIZE(adsp_esai_map),
        .reg = adsp_esai_map, .irq = IRQ_NUM_EXT_SSP0,
        .init = &adsp_esai_init, .ops = &esai_ops,
        .desc = {.base = ADSP_IMX8_ESAI_BASE, .size = ADSP_IMX8_ESAI_SIZE},},
    { .name = "sai0", .reg_count = ARRAY_SIZE(adsp_sai_map),
        .reg = adsp_sai_map, .irq = IRQ_NUM_EXT_SSP1,
        .init = &adsp_sai_init, .ops = &sai_ops,
        .desc = {.base = ADSP_IMX8_SAI_1_BASE, .size = ADSP_IMX8_SAI_1_SIZE},},
    { .name = "sai1", .reg_count = ARRAY_SIZE(adsp_sai_map),
        .reg = adsp_sai_map, .irq = IRQ_NUM_EXT_SSP2,
        .init = &adsp_sai_init, .ops = &sai_ops,
	.desc = {.base = ADSP_IMX8_SAI_1_BASE, .size = ADSP_IMX8_SAI_1_SIZE},},
    { .name = "sai2", .reg_count = ARRAY_SIZE(adsp_sai_map),
        .reg = adsp_sai_map, .irq = IRQ_NUM_EXT_SSP0,
        .init = &adsp_sai_init, .ops = &sai_ops,
        .desc = {.base = ADSP_IMX8_SAI_1_BASE, .size = ADSP_IMX8_SAI_1_SIZE},},
    { .name = "sai3", .reg_count = ARRAY_SIZE(adsp_sai_map),
        .reg = adsp_sai_map, .irq = IRQ_NUM_EXT_SSP1,
        .init = &adsp_sai_init, .ops = &sai_ops,
        .desc = {.base = ADSP_IMX8_SAI_1_BASE, .size = ADSP_IMX8_SAI_1_SIZE},},
    { .name = "sai4", .reg_count = ARRAY_SIZE(adsp_sai_map),
        .reg = adsp_sai_map, .irq = IRQ_NUM_EXT_SSP2,
        .init = &adsp_sai_init, .ops = &sai_ops,
        .desc = {.base = ADSP_IMX8_SAI_1_BASE, .size = ADSP_IMX8_SAI_1_SIZE},},
    { .name = "mu", .reg_count = ARRAY_SIZE(adsp_imx8_mu_map),
        .reg = adsp_imx8_mu_map, .init = &adsp_imx8_mu_init, .ops = &imx8_mu_ops,
        .desc = {.base = ADSP_IMX8_DSP_MU_BASE, .size = ADSP_IMX8_SHIM_SIZE},},
    { .name = "mbox", .reg_count = ARRAY_SIZE(adsp_imx8_mbox_map),
        .reg = adsp_imx8_mbox_map, .ops = &mbox_io_ops,
        .desc = {.base = ADSP_IMX8_DSP_MAILBOX_BASE, .size = ADSP_IMX8_DSP_MAILBOX_SIZE},},
};

/* hardware memory map */
static const struct adsp_desc imx8_dsp_desc = {
    .ia_irq = IRQ_NUM_EXT_IA,
    .ext_timer_irq = IRQ_NUM_EXT_TIMER,
    .pmc_irq = IRQ_NUM_EXT_PMC,

    .host_iram_offset = ADSP_IMX8_HOST_IRAM_OFFSET,
    .host_dram_offset = ADSP_IMX8_HOST_DRAM_OFFSET,

    .num_mem = ARRAY_SIZE(imx8_mem),
    .mem_region = imx8_mem,

    .num_io = ARRAY_SIZE(imx8_io),
    .io_dev = imx8_io,

   .iram_base = ADSP_IMX8_DSP_IRAM_BASE,
   .dram_base = ADSP_IMX8_DSP_DRAM_BASE,
};

static void imx8_adsp_init(MachineState *machine)
{
    adsp_init(&imx8_dsp_desc, machine, "i.MX8");
}

static void xtensa_imx8_machine_init(MachineClass *mc)
{
    mc->desc = "i.MX8";
    mc->is_default = true;
    mc->init = imx8_adsp_init;
    mc->max_cpus = 1;
    mc->default_cpu_type = XTENSA_DEFAULT_CPU_TYPE;
}

DEFINE_MACHINE("adsp_imx8", xtensa_imx8_machine_init)
