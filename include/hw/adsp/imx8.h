/* Core Audio DSP
 *
 * Copyright (C) 2016 Intel Corporation
 *
 * Author: Liam Girdwood <liam.r.girdwood@linux.intel.com>
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

#ifndef __HW_ADSP_IMX8_H__
#define __HW_ADSP_IMX8_H__

#include <sys/time.h>
#include <stdio.h>
#include <stdint.h>
#include "qemu/osdep.h"
#include "qapi/error.h"
#include "qemu-common.h"
#include "exec/hwaddr.h"
#include "hw.h"

/* Baytrail, Cherrytrail and Braswell */
#define ADSP_IMX8_PCI_BASE           0xF1200000
#define ADSP_IMX8_MMIO_BASE          0xF1400000
#define ADSP_IMX8_HOST_IRAM_OFFSET   0x000c0000
#define ADSP_IMX8_HOST_DRAM_OFFSET   0x00100000
#define ADSP_IMX8_HOST_IRAM_BASE     (ADSP_IMX8_MMIO_BASE + ADSP_IMX8_HOST_IRAM_OFFSET)
#define ADSP_IMX8_HOST_DRAM_BASE     (ADSP_IMX8_MMIO_BASE + ADSP_IMX8_HOST_DRAM_OFFSET)
#define ADSP_IMX8_HOST_SHIM_BASE     (ADSP_IMX8_MMIO_BASE + 0x00140000)
#define ADSP_IMX8_HOST_MAILBOX_BASE  (ADSP_IMX8_MMIO_BASE + 0x00144000)

#define ADSP_IMX8_DSP_SHIM_BASE      0xFF340000
#define ADSP_IMX8_SHIM_SIZE          0x00001000

#define ADSP_IMX8_DSP_MAILBOX_BASE   0xFF344000

#define ADSP_IMX8_DMA0_BASE          0xFF298000
#define ADSP_IMX8_DMA1_BASE          0xFF29C000
#define ADSP_IMX8_DMA2_BASE          0xFF294000
#define ADSP_IMX8_DMA0_SIZE          0x00001000
#define ADSP_IMX8_DMA1_SIZE          0x00001000
#define ADSP_IMX8_DMA2_SIZE          0x00001000

#define ADSP_IMX8_SSP0_BASE          0xFF2A0000
#define ADSP_IMX8_SSP1_BASE          0xFF2A1000
#define ADSP_IMX8_SSP2_BASE          0xFF2A2000
#define ADSP_IMX8_SSP3_BASE          0xFF2A4000
#define ADSP_IMX8_SSP4_BASE          0xFF2A5000
#define ADSP_IMX8_SSP5_BASE          0xFF2A6000
#define ADSP_IMX8_SSP0_SIZE          0x00001000
#define ADSP_IMX8_SSP1_SIZE          0x00001000
#define ADSP_IMX8_SSP2_SIZE          0x00001000
#define ADSP_IMX8_SSP3_SIZE          0x00001000
#define ADSP_IMX8_SSP4_SIZE          0x00001000
#define ADSP_IMX8_SSP5_SIZE          0x00001000

#define ADSP_IMX8_DSP_IRAM_BASE      0x596f8000
#define ADSP_IMX8_DSP_DRAM_BASE      0x596e8000 
#define ADSP_IMX8_IRAM_SIZE          0x800
#define ADSP_IMX8_DRAM_SIZE          0x8000

#endif