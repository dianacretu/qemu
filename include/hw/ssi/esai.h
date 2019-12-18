/*
 * BSD 3 Clause - See LICENCE file for details.
 *
 * Copyright (c) 2015, Intel Corporation
 * All rights reserved.
 *
 */

#ifndef __ADSP_ESAI_H__
#define __ADSP_ESAI_H__

/* ESAI registers start */
/* ESAI register offsets */
#define SSCR0       0x00
#define SSCR1       0x04
#define SSSR        0x08
#define SSITR       0x0C
#define SSDR        0x10
#define SSTO        0x28
#define SSPSP       0x2C
#define SSTSA       0x30
#define SSRSA       0x34
#define SSTSS       0x38

/* ESAI Register Map */
#define REG_ESAI_ETDR           0x00
#define REG_ESAI_ERDR           0x04
#define REG_ESAI_ECR            0x08
#define REG_ESAI_ESR            0x0C
#define REG_ESAI_TFCR           0x10
#define REG_ESAI_TFSR           0x14
#define REG_ESAI_RFCR           0x18
#define REG_ESAI_RFSR           0x1C
#define REG_ESAI_xFCR(rx)       (rx ? REG_ESAI_RFCR : REG_ESAI_TFCR)
#define REG_ESAI_xFSR(rx)       (rx ? REG_ESAI_RFSR : REG_ESAI_TFSR)
#define REG_ESAI_TX0            0x80
#define REG_ESAI_TX1            0x84
#define REG_ESAI_TX2            0x88
#define REG_ESAI_TX3            0x8C
#define REG_ESAI_TX4            0x90
#define REG_ESAI_TX5            0x94
#define REG_ESAI_TSR            0x98
#define REG_ESAI_RX0            0xA0
#define REG_ESAI_RX1            0xA4
#define REG_ESAI_RX2            0xA8
#define REG_ESAI_RX3            0xAC
#define REG_ESAI_SAISR          0xCC
#define REG_ESAI_SAICR          0xD0
#define REG_ESAI_TCR            0xD4
#define REG_ESAI_TCCR           0xD8
#define REG_ESAI_RCR            0xDC
#define REG_ESAI_RCCR           0xE0
#define REG_ESAI_xCR(rx)        (rx ? REG_ESAI_RCR : REG_ESAI_TCR)
#define REG_ESAI_xCCR(rx)       (rx ? REG_ESAI_RCCR : REG_ESAI_TCCR)
#define REG_ESAI_TSMA           0xE4
#define REG_ESAI_TSMB           0xE8
#define REG_ESAI_RSMA           0xEC
#define REG_ESAI_RSMB           0xF0
#define REG_ESAI_xSMA(rx)       (rx ? REG_ESAI_RSMA : REG_ESAI_TSMA)
#define REG_ESAI_xSMB(rx)       (rx ? REG_ESAI_RSMB : REG_ESAI_TSMB)
#define REG_ESAI_PRRC           0xF8
#define REG_ESAI_PCRC           0xFC

#define ESAI_ECR_ETI»   »       BIT(19)
#define ESAI_ECR_ETO»   »       BIT(18)
#define ESAI_ECR_ERI»   »       BIT(17)
#define ESAI_ECR_ERO»   »       BIT(16)
#define ESAI_ECR_ERST»  »       BIT(1)
#define ESAI_ECR_ESAIEN»»       BIT(0)

#define ESAI_ESR_TINIT» »       BIT(10)
#define ESAI_ESR_RFF»   »       BIT(9)
#define ESAI_ESR_TFE»   »       BIT(8)
#define ESAI_ESR_TLS»   »       BIT(7)
#define ESAI_ESR_TDE»   »       BIT(6)
#define ESAI_ESR_TED»   »       BIT(5)
#define ESAI_ESR_TD»    »       BIT(4)
#define ESAI_ESR_RLS»   »       BIT(3)
#define ESAI_ESR_RDE»   »       BIT(2)
#define ESAI_ESR_RED»   »       BIT(1)
#define ESAI_ESR_RD»    »       BIT(0)

#define ESAI_xFCR_TIEN» »       BIT(19)
#define ESAI_xFCR_xWA_SHIFT»    16
#define ESAI_xFCR_xWA_WIDTH»    3
#define ESAI_xFCR_xWA_MASK»     MASK(18, 16)
#define ESAI_xFCR_xWA(v)»       SET_BITS(18, 16, 8 - ((v) >> 2))
#define ESAI_xFCR_xFWM_SHIFT»   8
#define ESAI_xFCR_xFWM_WIDTH»   8
#define ESAI_xFCR_xFWM_MASK»    MASK(15, 8)
#define ESAI_xFCR_xFWM(v)»      SET_BITS(15, 8, (v) - 1)
#define ESAI_xFCR_xE_SHIFT»     2
#define ESAI_xFCR_TE_WIDTH»     6
#define ESAI_xFCR_RE_WIDTH»     4
#define ESAI_xFCR_TE_MASK»      MASK(7, 2)
#define ESAI_xFCR_RE_MASK»      MASK(5, 2)
#define ESAI_xFCR_xE_MASK(rx)»  (rx ? ESAI_xFCR_RE_MASK : ESAI_xFCR_TE_MASK)
#define ESAI_xFCR_TE(x)»»       SET_BITS(7, 2, MASK((x) - 1, 0))
#define ESAI_xFCR_RE(x)»»       SET_BITS(5, 2, MASK((x) - 1, 0))
#define ESAI_xFCR_xE(rx, x)»    (rx ? ESAI_xFCR_RE(x) : ESAI_xFCR_TE(x))
#define ESAI_xFCR_xFR_SHIFT»    1
#define ESAI_xFCR_xFR_MASK»     BIT(1)
#define ESAI_xFCR_xFR»  »       BIT(1)
#define ESAI_xFCR_xFEN_SHIFT»   0
#define ESAI_xFCR_xFEN» »       BIT(0)
#define ESAI_xFCR_xFEN_MASK»    BIT(0)

#define ESAI_xFSR_NTFO_MASK»    MASK(14, 12)
#define ESAI_xFSR_NTFI_MASK»    MASK(10, 8)
#define ESAI_xFSR_NRFO_MASK»    MASK(9, 8)
#define ESAI_xFSR_NRFI_MASK»    MASK(13, 12)
#define ESAI_xFSR_xFCNT_MASK»   MASK(7, 0)

#define ESAI_SAISR_TODFE»       BIT(17)
#define ESAI_SAISR_TEDE»»       BIT(16)
#define ESAI_SAISR_TDE» »       BIT(15)
#define ESAI_SAISR_TUE» »       BIT(14)
#define ESAI_SAISR_TFS» »       BIT(13)
#define ESAI_SAISR_RODF»»       BIT(10)
#define ESAI_SAISR_REDF»»       BIT(9)
#define ESAI_SAISR_RDF» »       BIT(8)
#define ESAI_SAISR_ROE» »       BIT(7)
#define ESAI_SAISR_RFS» »       BIT(6)
#define ESAI_SAISR_IF2» »       BIT(2)
#define ESAI_SAISR_IF1» »       BIT(1)
#define ESAI_SAISR_IF0» »       BIT(0)

#define ESAI_SAICR_ALC» »       BIT(8)
#define ESAI_SAICR_TEBE»»       BIT(7)
#define ESAI_SAICR_SYN» »       BIT(6)
#define ESAI_SAICR_OF2» »       BIT(2)
#define ESAI_SAICR_OF1» »       BIT(1)
#define ESAI_SAICR_OF0» »       BIT(0)

#define ESAI_xCR_xLIE»  »       BIT(23)
#define ESAI_xCR_xIE»   »       BIT(22)
#define ESAI_xCR_xEDIE» »       BIT(21)
#define ESAI_xCR_xEIE»  »       BIT(20)
#define ESAI_xCR_xPR»   »       BIT(19)
#define ESAI_xCR_PADC»  »       BIT(17) /* only valid on tx */
#define ESAI_xCR_xFSR»  »       BIT(16)
#define ESAI_xCR_xFSL»  »       BIT(15)
#define ESAI_xCR_xSWS_MASK»     MASK(14, 10)

/* The following macro is actually made to fit the table 16-44 in the i.MX8QXP
 * manual, ESAI chapter.
 */
#define ESAI_xCR_xSWS_VAL(s, w)»((w) < 24 ? ((s) - (w) + (((w) - 8) >> 2)) \
»       »       »       »       »         : ((s) < 32 ? 0x1e : 0x1f))
#define ESAI_xCR_xSWS(s, w)»    SET_BITS(14, 10, ESAI_xCR_xSWS_VAL(s, w))
#define ESAI_xCR_xMOD_MASK»     MASK(9, 8)
#define ESAI_xCR_xMOD_NORMAL»   SET_BITS(9, 8, 0)
#define ESAI_xCR_xMOD_NETWORK»  SET_BITS(9, 8, 1)
#define ESAI_xCR_xMOD_AC97»     SET_BITS(9, 8, 3)
#define ESAI_xCR_xWA»   »       BIT(7)
#define ESAI_xCR_xSHFD» »       BIT(6)
#define ESAI_xCR_TE_MASK»       MASK(5, 0)
#define ESAI_xCR_TE_SET(v)»     SET_BITS(5, 0, v)
#define ESAI_xCR_TE(v)» »       ESAI_xCR_TE_SET(MASK((v) - 1, 0))
#define ESAI_xCR_RE_MASK»       MASK(3, 0)
#define ESAI_xCR_RE_SET(v)»     SET_BITS(3, 0, v)
#define ESAI_xCR_RE(v)» »       ESAI_xCR_RE_SET(MASK((v) - 1, 0))
#define ESAI_xCR_xE_MASK(rx)»   (rx ? ESAI_xCR_RE_MASK : ESAI_xCR_TE_MASK)
#define ESAI_xCR_xE(rx, v)»     (rx ? ESAI_xCR_RE(v) : ESAI_xCR_TE(v))

#define ESAI_xCCR_xHCKD»»       BIT(23)
#define ESAI_xCCR_xFSD» »       BIT(22)
#define ESAI_xCCR_xCKD» »       BIT(21)
#define ESAI_xCCR_xHCKP»»       BIT(20)
#define ESAI_xCCR_xFSP» »       BIT(19)
#define ESAI_xCCR_xCKP» »       BIT(18)
#define ESAI_xCCR_xFP_MASK»     MASK(17, 14)
#define ESAI_xCCR_xFP(div)»     SET_BITS(17, 14, (div) - 1)
#define ESAI_xCCR_xDC_MASK»     MASK(13, 9)
#define ESAI_xCCR_xDC(v)»       SET_BITS(13, 9, (v) - 1)
#define ESAI_xCCR_xDC_AC97»     SET_BITS(13, 9, 0x0C)
#define ESAI_xCCR_xPSR_MASK»    MASK(8, 8)
#define ESAI_xCCR_xPSR_BYPASS»  SET_BITS(8, 8, 1)
#define ESAI_xCCR_xPSR_DIV8»    SET_BITS(8, 8, 0)
#define ESAI_xCCR_xPM_MASK»     MASK(7, 0)
#define ESAI_xCCR_xPM(v)»       SET_BITS(7, 0, (v) - 1)

#define ESAI_xSMA_xS_MASK»      MASK(15, 0)
#define ESAI_xSMA_xS(v)»»       SET_BITS(15, 0, v)
#define ESAI_xSMB_xS_MASK»      MASK(15, 0)
#define ESAI_xSMB_xS(v)»»       SET_BITS(15, 0, (v) >> 16)
#define ESAI_xSMA_xS_CHANS(ch)» ESAI_xSMA_xS(MASK((ch) - 1, 0))
#define ESAI_xSMB_xS_CHANS(ch)» ESAI_xSMB_xS(MASK((ch) - 1, 0))

#define ESAI_PRRC_PDC_MASK»     MASK(11, 0)
#define ESAI_PRRC_PDC(v)»       SET_BITS(11, 0, v)

#define ESAI_PCRC_PC_MASK»      MASK(11, 0)
#define ESAI_PCRC_PC(v)»»       SET_BITS(11, 0, v)

#define ESAI_GPIO»      »       MASK(11, 0)

#define EDMA_ESAI_IRQ»  »       442

#define EDMA_ESAI_TX_CHAN»      7
#define EDMA_ESAI_RX_CHAN»      6


/* SSCR0 bits */
#define SSCR0_DSS_MASK  (0x0000000f)
#define SSCR0_DSIZE(x)  ((x) - 1)
#define SSCR0_FRF   (0x00000030)
#define SSCR0_MOT   (00 << 4)
#define SSCR0_TI    (1 << 4)
#define SSCR0_NAT   (2 << 4)
#define SSCR0_PSP   (3 << 4)
#define SSCR0_ECS   (1 << 6)
#define SSCR0_SSE   (1 << 7)
#define SSCR0_SCR(x)    ((x) << 8)
#define SSCR0_EDSS  (1 << 20)
#define SSCR0_NCS   (1 << 21)
#define SSCR0_RIM   (1 << 22)
#define SSCR0_TUM   (1 << 23)
#define SSCR0_FRDC  (0x07000000)
#define SSCR0_ACS   (1 << 30)
#define SSCR0_MOD   (1 << 31)

/* SSCR1 bits */
#define SSCR1_RIE   (1 << 0)
#define SSCR1_TIE   (1 << 1)
#define SSCR1_LBM   (1 << 2)
#define SSCR1_SPO   (1 << 3)
#define SSCR1_SPH   (1 << 4)
#define SSCR1_MWDS  (1 << 5)
#define SSCR1_TFT_MASK  (0x000003c0)
#define SSCR1_TX(x) (((x) - 1) << 6)
#define SSCR1_RFT_MASK  (0x00003c00)
#define SSCR1_RX(x) (((x) - 1) << 10)
#define SSCR1_EFWR  (1 << 14)
#define SSCR1_STRF  (1 << 15)
#define SSCR1_IFS   (1 << 16)
#define SSCR1_PINTE (1 << 18)
#define SSCR1_TINTE (1 << 19)
#define SSCR1_RSRE  (1 << 20)
#define SSCR1_TSRE  (1 << 21)
#define SSCR1_TRAIL (1 << 22)
#define SSCR1_RWOT  (1 << 23)
#define SSCR1_SFRMDIR   (1 << 24)
#define SSCR1_SCLKDIR   (1 << 25)
#define SSCR1_ECRB  (1 << 26)
#define SSCR1_ECRA  (1 << 27)
#define SSCR1_SCFR  (1 << 28)
#define SSCR1_EBCEI (1 << 29)
#define SSCR1_TTE   (1 << 30)
#define SSCR1_TTELP (1 << 31)

/* SSR bits */
#define SSSR_TNF    (1 << 2)
#define SSSR_RNE    (1 << 3)
#define SSSR_BSY    (1 << 4)
#define SSSR_TFS    (1 << 5)
#define SSSR_RFS    (1 << 6)
#define SSSR_ROR    (1 << 7)

/* SSPSP bits */
#define SSPSP_SCMODE(x)     ((x) << 0)
#define SSPSP_SFRMP     (1 << 2)
#define SSPSP_ETDS      (1 << 3)
#define SSPSP_STRTDLY(x)    ((x) << 4)
#define SSPSP_DMYSTRT(x)    ((x) << 7)
#define SSPSP_SFRMDLY(x)    ((x) << 9)
#define SSPSP_SFRMWDTH(x)   ((x) << 16)
#define SSPSP_DMYSTOP(x)    ((x) << 23)
#define SSPSP_FSRT      (1 << 25)


/* ESAI registers end */

/* ESAI register offsets */
#define SSCR0       0x00
#define SSCR1       0x04
#define SSSR        0x08
#define SSITR       0x0C
#define SSDR        0x10
#define SSTO        0x28
#define SSPSP       0x2C
#define SSTSA       0x30
#define SSRSA       0x34
#define SSTSS       0x38

struct adsp_dev;
struct adsp_gp_dmac;
struct adsp_log;
struct adsp_reg_space;

struct esai_fifo {
	uint32_t total_frames;
	uint32_t index;
	int fd;
	char file_name[64];
	uint32_t data[128];
	uint32_t level;
};

struct adsp_esai {
	char name[32];
	uint32_t *io;

	struct esai_fifo tx;
	struct esai_fifo rx;

	struct adsp_log *log;
	const struct adsp_reg_space *esai_dev;
};

#define ADSP_ESAI_REGS		1
extern const struct adsp_reg_desc adsp_esai_map[ADSP_ESAI_REGS];

struct adsp_esai *esai_get_port(int port);
void adsp_esai_init(struct adsp_dev *adsp, MemoryRegion *parent,
        struct adsp_io_info *info);
extern const MemoryRegionOps esai_ops;

#endif
