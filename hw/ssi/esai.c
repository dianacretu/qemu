/* Virtualization support for ESAI.
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

#include "qemu/osdep.h"
#include "qapi/error.h"
#include "qemu-common.h"
#include "sysemu/sysemu.h"
#include "hw/boards.h"
#include "hw/loader.h"

#include "qemu/io-bridge.h"
#include "hw/adsp/shim.h"
#include "hw/adsp/log.h"
#include "hw/ssi/ssp.h"
#include "hw/ssi/esai.h"
#include "hw/adsp/imx8.h"

const struct adsp_reg_desc adsp_esai_map[ADSP_ESAI_REGS] = {
    {.name = "esai", .enable = LOG_ESAI,
        .offset = 0x00000000, .size = ADSP_IMX8_ESAI_SIZE},
};

static void esai_reset(void *opaque)
{
     struct adsp_io_info *info = opaque;
     struct adsp_reg_space *space = info->space;

     memset(info->region, 0, space->desc.size);
}

static uint64_t esai_read(void *opaque, hwaddr addr,
        unsigned size)
{
    struct adsp_io_info *info = opaque;
    struct adsp_reg_space *space = info->space;
    struct adsp_esai *esai = info->private;

    log_read(esai->log, space, addr, size,
        info->region[addr >> 2]);

    return info->region[addr >> 2];
}

static void esai_write(void *opaque, hwaddr addr,
        uint64_t val, unsigned size)
{
    struct adsp_io_info *info = opaque;
    struct adsp_reg_space *space = info->space;
    struct adsp_esai *esai = info->private;
    uint32_t set, clear;

    log_write(esai->log, space, addr, val, size,
        info->region[addr >> 2]);

    switch (addr) {
    case REG_ESAI_TCR:
        set = val & ~info->region[addr >> 2];
        clear = ~val & info->region[addr >> 2];

        info->region[addr >> 2] = val;

        /* open file if playback has been enabled */
        if (set & ESAI_xCR_TE(1)) {

            /* create filename */
            sprintf(esai->tx.file_name, "/tmp/%s-play%d.wav",
                esai->name, esai->tx.index++);

            unlink(esai->tx.file_name);
            esai->tx.fd = open(esai->tx.file_name, O_WRONLY | O_CREAT,
                S_IRUSR | S_IWUSR | S_IXUSR);

            if (esai->tx.fd < 0) {
                fprintf(stderr, "cant open file %s %d\n",
                    esai->tx.file_name, -errno);
                return;
            } else
                printf("%s opened %s for playback\n",
                    esai->name, esai->tx.file_name);

            esai->tx.total_frames = 0;
        }

        /* close file if playback has finished */
        if (clear & ESAI_xCR_TE(1)) {
            printf("%s closed %s for playback at %d frames\n",
                esai->name,
                esai->tx.file_name, esai->tx.total_frames);
            close(esai->tx.fd);
            esai->tx.fd = 0;
        }
	
    	//case REG_ESAI_RCR:
        /* open file if capture has been enabled */
        if (set & SSCR1_RSRE) {

            /* create filename */
            sprintf(esai->rx.file_name, "/tmp/%s-capture.wav",
                esai->name);

            esai->rx.fd = open(esai->tx.file_name, O_RDONLY,
                S_IRUSR | S_IWUSR | S_IXUSR);

            if (esai->rx.fd < 0) {
                fprintf(stderr, "cant open file %s %d\n",
                    esai->rx.file_name, -errno);
                return;
            } else
                printf("%s opened %s for capture\n",
                    esai->name, esai->rx.file_name);

            esai->rx.total_frames = 0;
        }

        /* close file if capture has finished */
        if (clear & SSCR1_RSRE) {
            printf("%s closed %s for capture at %d frames\n", esai->name,
                esai->rx.file_name, esai->rx.total_frames);
            close(esai->rx.fd);
            esai->rx.fd = 0;
        }
        break;
    case SSDR:
        /* update counters */
        esai->tx.total_frames += size;
        info->region[addr >> 2] = val;

        break;
    default:
        info->region[addr >> 2] = val;
        break;
    }
}

const MemoryRegionOps esai_ops = {
    .read = esai_read,
    .write = esai_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

#define MAX_ESAI     1
struct adsp_esai *_esai[MAX_ESAI] = {NULL};

struct adsp_esai *esai_get_port(int port)
{
    if (port >= 0  && port < MAX_ESAI)
        return _esai[port];

    // TODO find an alternative
    fprintf(stderr, "cant get ESAI port %d\n", port);
    return NULL;
}

void adsp_esai_init(struct adsp_dev *adsp, MemoryRegion *parent,
        struct adsp_io_info *info)
{
    struct adsp_esai *esai;

    esai = g_malloc(sizeof(*esai));

    esai->tx.level = 0;
    esai->rx.level = 0;
    sprintf(esai->name, "%s.io", info->space->name);

    esai->log = log_init(NULL);
    info->private = esai;
    esai_reset(info);
    _esai[info->io_dev] = esai;
}
