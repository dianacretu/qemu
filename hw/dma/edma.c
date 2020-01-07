/*
 * DMA device simulation for NXP eDMA controller
 *
 * Copyright (C) 2010-2012 Guan Xuetao
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation, or any later version.
 * See the COPYING file in the top-level directory.
 */
#include "qemu/osdep.h"
#include "hw/hw.h"
#include "hw/sysbus.h"


#include "qemu/osdep.h"
#include <mqueue.h>
#include "sysemu/sysemu.h"
#include "hw/boards.h"
#include "hw/loader.h"
#include "elf.h"
#include "exec/memory.h"
#include "exec/address-spaces.h"
#include "hw/sysbus.h"
#include "qemu/error-report.h"
#include "qemu/io-bridge.h"
#include "hw/pci/pci.h"

#include <hw/adsp/shim.h>
#include <hw/adsp/log.h>
#include "hw/dma/edma.h"
#include "hw/audio/adsp-host.h"


#define EDMA_DEVICE_NAME "edma"


const struct adsp_reg_desc adsp_edma_map[] = {
    {.name = "dma", .enable = LOG_DMA,
        .offset = 0x00000000, .size = 0x4000},
};

#if 0
#define PUV3_DMA(obj) OBJECT_CHECK(PUV3DMAState, (obj), TYPE_PUV3_DMA)
#endif

struct edma_host {
    SysBusDevice parent_obj;
    MemoryRegion iomem;
};

static uint64_t edma_read(void *opaque, hwaddr offset,
        unsigned size)
{
	return 0;
}

static void edma_write(void *opaque, hwaddr offset,
        uint64_t value, unsigned size)
{

}

const MemoryRegionOps edma_ops = {
    .read = edma_read,
    .write = edma_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};
void edma_init_dev(struct adsp_dev *adsp, MemoryRegion *parent,
        struct adsp_io_info *info)
{

}

static int edma_init(SysBusDevice *dev)
{
#if 0
    int i;

    for (i = 0; i < PUV3_DMA_CH_NR; i++) {
        s->reg_CFG[i] = 0x0;
    }

    memory_region_init_io(&s->iomem, OBJECT(s), &puv3_dma_ops, s, "puv3_dma",
            PUV3_REGS_OFFSET);
    sysbus_init_mmio(dev, &s->iomem);
#endif
    return 0;
}

static void edma_class_init(ObjectClass *klass, void *data)
{
    SysBusDeviceClass *sdc = SYS_BUS_DEVICE_CLASS(klass);

    sdc->init = edma_init;
}

static const TypeInfo edma_info = {
    .name = EDMA_DEVICE_NAME,
    .parent = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(struct edma_host),
    .class_init = edma_class_init,
};

static void edma_register_type(void)
{
    type_register_static(&edma_info);
}

type_init(edma_register_type)
