/* Virtualization support for SAI.
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
#include "hw/ssi/sai.h"
#include "hw/adsp/imx8.h"

const struct adsp_reg_desc adsp_sai_map[ADSP_SAI_REGS] = {
    {.name = "sai", .enable = LOG_SAI,
        .offset = 0x00000000, .size = ADSP_IMX8_SAI_1_SIZE},
};

static void sai_reset(void *opaque)
{
     struct adsp_io_info *info = opaque;
     struct adsp_reg_space *space = info->space;

     memset(info->region, 0, space->desc.size);
}

static uint64_t sai_read(void *opaque, hwaddr addr,
        unsigned size)
{
    struct adsp_io_info *info = opaque;
    struct adsp_reg_space *space = info->space;
    struct adsp_sai *sai = info->private;

    log_read(sai->log, space, addr, size,
        info->region[addr >> 2]);

    return info->region[addr >> 2];
}

static void sai_write(void *opaque, hwaddr addr,
        uint64_t val, unsigned size)
{
    struct adsp_io_info *info = opaque;
    struct adsp_reg_space *space = info->space;
    struct adsp_sai *sai = info->private;
    uint32_t set, clear;

    log_write(sai->log, space, addr, val, size,
        info->region[addr >> 2]);

    switch (addr) {
    case SSCR1:
        set = val & ~info->region[addr >> 2];
        clear = ~val & info->region[addr >> 2];

        info->region[addr >> 2] = val;

        /* open file if playback has been enabled */
        if (set & SSCR1_TSRE) {

            /* create filename */
            sprintf(sai->tx.file_name, "/tmp/%s-play%d.wav",
                sai->name, sai->tx.index++);

            unlink(sai->tx.file_name);
            sai->tx.fd = open(sai->tx.file_name, O_WRONLY | O_CREAT,
                S_IRUSR | S_IWUSR | S_IXUSR);

            if (sai->tx.fd < 0) {
                fprintf(stderr, "cant open file %s %d\n",
                    sai->tx.file_name, -errno);
                return;
            } else
                printf("%s opened %s for playback\n",
                    sai->name, sai->tx.file_name);

            sai->tx.total_frames = 0;
        }

        /* close file if playback has finished */
        if (clear & SSCR1_TSRE) {
            printf("%s closed %s for playback at %d frames\n",
                sai->name,
                sai->tx.file_name, sai->tx.total_frames);
            close(sai->tx.fd);
            sai->tx.fd = 0;
        }

        /* open file if capture has been enabled */
        if (set & SSCR1_RSRE) {

            /* create filename */
            sprintf(sai->rx.file_name, "/tmp/%s-capture.wav",
                sai->name);

            sai->rx.fd = open(sai->tx.file_name, O_RDONLY,
                S_IRUSR | S_IWUSR | S_IXUSR);

            if (sai->rx.fd < 0) {
                fprintf(stderr, "cant open file %s %d\n",
                    sai->rx.file_name, -errno);
                return;
            } else
                printf("%s opened %s for capture\n",
                    sai->name, sai->rx.file_name);

            sai->rx.total_frames = 0;
        }

        /* close file if capture has finished */
        if (clear & SSCR1_RSRE) {
            printf("%s closed %s for capture at %d frames\n", sai->name,
                sai->rx.file_name, sai->rx.total_frames);
            close(sai->rx.fd);
            sai->rx.fd = 0;
        }
        break;
    case SSDR:
        /* update counters */
        sai->tx.total_frames += size;
        info->region[addr >> 2] = val;

        break;
    default:
        info->region[addr >> 2] = val;
        break;
    }
}

const MemoryRegionOps sai_ops = {
    .read = sai_read,
    .write = sai_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

#define MAX_SSP     6
struct adsp_sai *_sai[MAX_SSP] = {NULL, NULL, NULL, NULL, NULL, NULL};

struct adsp_sai *sai_get_port(int port)
{
    if (port >= 0  && port < MAX_SSP)
        return _sai[port];

    // TODO find an alternative
    fprintf(stderr, "cant get SAI port %d\n", port);
    return NULL;
}

void adsp_sai_init(struct adsp_dev *adsp, MemoryRegion *parent,
        struct adsp_io_info *info)
{
    struct adsp_sai *sai;

    sai = g_malloc(sizeof(*sai));

    sai->tx.level = 0;
    sai->rx.level = 0;
    sprintf(sai->name, "%s.io", info->space->name);

    sai->log = log_init(NULL);
    info->private = sai;
    sai_reset(info);
    _sai[info->io_dev] = sai;
}
