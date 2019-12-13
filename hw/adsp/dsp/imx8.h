/* Core DSP support for Imx8 audio DSP.
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

#ifndef __ADSP_IMX8_H__
#define __ADSP_IMX8_H__

/* IRQ numbers */
//#define IRQ_NUM_SOFTWARE0    0
//#define IRQ_NUM_TIMER1       1
//#define IRQ_NUM_SOFTWARE1    2
//#define IRQ_NUM_SOFTWARE2    3
//#define IRQ_NUM_TIMER2       5
//#define IRQ_NUM_SOFTWARE3    6
//#define IRQ_NUM_TIMER3       7
//#define IRQ_NUM_SOFTWARE4    8
//#define IRQ_NUM_SOFTWARE5    9
#define IRQ_NUM_EXT_IA       10
#define IRQ_NUM_EXT_PMC      11
//#define IRQ_NUM_SOFTWARE6    12
#define IRQ_NUM_EXT_DMAC0    13
#define IRQ_NUM_EXT_DMAC1    14
#define IRQ_NUM_EXT_TIMER    15
#define IRQ_NUM_EXT_SSP0     16
#define IRQ_NUM_EXT_SSP1     17
#define IRQ_NUM_EXT_SSP2     18
//#define IRQ_NUM_EXT_DMAC2    19
//#define IRQ_NUM_NMI          20

//#if CONFIG_INTERRUPT_LEVEL_1

#define IRQ_NUM_SOFTWARE0    8      /* Level 1 */

#define IRQ_MASK_SOFTWARE0    BIT(IRQ_NUM_SOFTWARE0)

//#endif

//#if CONFIG_INTERRUPT_LEVEL_2

#define IRQ_NUM_TIMER0       2      /* Level 2 */
#define IRQ_NUM_MU           7      /* Level 2 */
#define IRQ_NUM_SOFTWARE1    9      /* Level 2 */
#define IRQ_NUM_IRQSTR_DSP0  19     /* Level 2 */
#define IRQ_NUM_IRQSTR_DSP1  20     /* Level 2 */
#define IRQ_NUM_IRQSTR_DSP2  21     /* Level 2 */
#define IRQ_NUM_IRQSTR_DSP3  22     /* Level 2 */
#define IRQ_NUM_IRQSTR_DSP4  23     /* Level 2 */
#define IRQ_NUM_IRQSTR_DSP5  24     /* Level 2 */
#define IRQ_NUM_IRQSTR_DSP6  25     /* Level 2 */
#define IRQ_NUM_IRQSTR_DSP7  26     /* Level 2 */

#define IRQ_MASK_TIMER0        BIT(IRQ_NUM_TIMER0)
#define IRQ_MASK_MU            BIT(IRQ_NUM_MU)
#define IRQ_MASK_SOFTWARE1     BIT(IRQ_NUM_SOFTWARE1)
#define IRQ_MASK_IRQSTR_DSP0   BIT(IRQ_NUM_IRQSTR_DSP0)
#define IRQ_MASK_IRQSTR_DSP1   BIT(IRQ_NUM_IRQSTR_DSP1)
#define IRQ_MASK_IRQSTR_DSP2   BIT(IRQ_NUM_IRQSTR_DSP2)
#define IRQ_MASK_IRQSTR_DSP3   BIT(IRQ_NUM_IRQSTR_DSP3)
#define IRQ_MASK_IRQSTR_DSP4   BIT(IRQ_NUM_IRQSTR_DSP4)
#define IRQ_MASK_IRQSTR_DSP5   BIT(IRQ_NUM_IRQSTR_DSP5)
#define IRQ_MASK_IRQSTR_DSP6   BIT(IRQ_NUM_IRQSTR_DSP6)
#define IRQ_MASK_IRQSTR_DSP7   BIT(IRQ_NUM_IRQSTR_DSP7)

//#endif

//#if CONFIG_INTERRUPT_LEVEL_3

#define IRQ_NUM_TIMER1       3      /* Level 3 */

#define IRQ_MASK_TIMER1       BIT(IRQ_NUM_TIMER1)

//#endif


struct adsp_dev;

void adsp_imx8_shim_init(struct adsp_dev *adsp, MemoryRegion *parent,
        struct adsp_io_info *info);
void adsp_imx8_shim_msg(struct adsp_dev *adsp, struct qemu_io_msg *msg);
void adsp_imx8_irq_msg(struct adsp_dev *adsp, struct qemu_io_msg *msg);
void imx8_ext_timer_cb(void *opaque);
extern const MemoryRegionOps imx8_shim_ops;
extern const MemoryRegionOps adsp_mbox_ops;

#endif
