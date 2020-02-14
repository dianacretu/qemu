/* Core IA host support for Intel audio DSPs.
 *
 * Copyright (C) 2016 Intel Corporation
 *
 * Author: Liam Girdwood <liam.r.girdwood@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __HW_I386_ADSP_HOST_H__
#define __HW_I386_ADSP_HOST_H__

#include "qemu/osdep.h"
#include <mqueue.h>
#include "sysemu/sysemu.h"
#include "hw/acpi/aml-build.h"
#include "hw/boards.h"
#include "hw/loader.h"
#include "elf.h"
#include "exec/memory.h"
#include "exec/address-spaces.h"
#include "exec/hwaddr.h"
#include "hw/char/serial.h"
#include "net/net.h"
#include "hw/sysbus.h"
#include "hw/block/flash.h"
#include "sysemu/block-backend.h"
#include "sysemu/device_tree.h"
#include "qemu/error-report.h"
#include "qemu/io-bridge.h"

#include "hw/i386/pc.h"
#include "hw/i386/ioapic.h"
#include "hw/i386/ioapic_internal.h"
#include "hw/adsp/hw.h"
#include "hw/adsp/log.h"

#define SST_DEV_ID_BAYTRAIL          0x0F28
#define SST_DEV_ID_CHERRYTRAIL       0x22A8
#define SST_DEV_ID_LYNX_POINT        0x33C8
#define SST_DEV_ID_WILDCAT_POINT     0x3438
#define SST_DEV_ID_SUNRISE_POINT     0x9d70
#define SST_DEV_ID_BROXTON_P         0x5a98

#define ADSP_MAX_GP_DMAC        3

typedef struct IntelHDAState IntelHDAState;

struct adsp_dma_buffer {
    int chan;
    uint32_t *io;
    struct adsp_mem_desc shm_desc;
    char *name;
};

struct adsp_host {

    PCIDevice dev;
    int shm_idx;

    /* IO mapped from ACPI tables */
    uint32_t *pci_io;
    uint32_t *ram;

    /* shim IO, offsets derived */
    uint32_t *shim_io;

    uint32_t *mbox_io;

    struct adsp_dma_buffer dma_shm_buffer[ADSP_MAX_GP_DMAC][8];

    /* runtime CPU */
    MemoryRegion *system_memory;
    QemuOpts *machine_opts;
    qemu_irq irq;

    /* logging options */
    struct adsp_log *log;

    /* machine init data */
    const struct adsp_desc *desc;
    const char *cpu_model;
    const char *kernel_filename;
};

#define adsp_get_pdata(obj, type) \
    OBJECT_CHECK(struct adsp_host, (obj), type)

#define ADSP_HOST_MBOX_COUNT    6
extern const struct adsp_reg_desc adsp_host_mbox_map[ADSP_HOST_MBOX_COUNT];

void adsp_host_init(struct adsp_host *adsp, const struct adsp_desc *board);
void adsp_host_do_dma(struct adsp_host *adsp, struct qemu_io_msg *msg);
void adsp_host_init_mbox(struct adsp_host *adsp, MemoryRegion *parent,
        struct adsp_io_info *info);

#define ADSP_HOST_BYT_NAME        "adsp-byt"
#define ADSP_HOST_CHT_NAME        "adsp-cht"

void adsp_byt_init_pci(struct adsp_host *adsp, MemoryRegion *parent,
        struct adsp_io_info *info);
void adsp_byt_pci_realize(PCIDevice *pci_dev, Error **errp);
void adsp_cht_pci_realize(PCIDevice *pci_dev, Error **errp);
void adsp_byt_pci_exit(PCIDevice *pci_dev);
void adsp_byt_init_shim(struct adsp_host *adsp, MemoryRegion *parent,
        struct adsp_io_info *info);
void adsp_byt_host_init(struct adsp_host *adsp, const char *name);
void build_acpi_byt_adsp_devices(Aml *table);

#define ADSP_HOST_IMX8_NAME        "adsp-imx8"

void adsp_imx8_init_pci(struct adsp_host *adsp, MemoryRegion *parent,
        struct adsp_io_info *info);
void adsp_imx8_pci_realize(PCIDevice *pci_dev, Error **errp);
void adsp_imx8_pci_exit(PCIDevice *pci_dev);
void adsp_imx8_init_shim(struct adsp_host *adsp, MemoryRegion *parent,
        struct adsp_io_info *info);
void adsp_imx8_host_init(struct adsp_host *adsp, const char *name);
void build_acpi_imx8_adsp_devices(Aml *table);


#define ADSP_HOST_HSW_NAME        "adsp-hsw"
#define ADSP_HOST_BDW_NAME        "adsp-bdw"

void adsp_hsw_init_pci(struct adsp_host *adsp, MemoryRegion *parent,
        struct adsp_io_info *info);
void adsp_hsw_pci_realize(PCIDevice *pci_dev, Error **errp);
void adsp_bdw_pci_realize(PCIDevice *pci_dev, Error **errp);
void adsp_hsw_pci_exit(PCIDevice *pci_dev);
void adsp_hsw_init_shim(struct adsp_host *adsp, MemoryRegion *parent,
        struct adsp_io_info *info);
void adsp_hsw_host_init(struct adsp_host *adsp, const char *name);
void adsp_bdw_host_init(struct adsp_host *adsp, const char *name);
void build_acpi_hsw_adsp_devices(Aml *table);

#define ADSP_HOST_BXT_NAME        "adsp-bxt"

void adsp_bxt_init_pci(struct adsp_host *adsp, MemoryRegion *parent,
        struct adsp_io_info *info);
void adsp_bxt_pci_realize(PCIDevice *pci_dev, Error **errp);
void adsp_bxt_pci_exit(PCIDevice *pci_dev);
void adsp_bxt_init_shim(struct adsp_host *adsp, MemoryRegion *parent,
        struct adsp_io_info *info);
void adsp_bxt_host_init(struct adsp_host *adsp, const char *name);
void build_acpi_bxt_adsp_devices(Aml *table);

void adsp_hda_init(IntelHDAState *d, int version, const char *name);

#endif
