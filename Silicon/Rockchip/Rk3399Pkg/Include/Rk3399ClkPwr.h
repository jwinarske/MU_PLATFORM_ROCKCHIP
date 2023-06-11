/** @file
*
*  Copyright (c) 2023, Joel Winarske. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#ifndef _RK3399_CLK_PWR_H_
#define _RK3399_CLK_PWR_H_

#include "Rk3399.h"

#define PMUCRU_PPLL_CON(n)		((n) * 4)
#define CRU_PLL_CON(pll_id, n)	((pll_id)  * 0x20 + (n) * 4)
#define PLL_MODE_MSK			0x03
#define PLL_MODE_SHIFT			0x08
#define PLL_BYPASS_MSK			0x01
#define PLL_BYPASS_SHIFT		0x01
#define PLL_PWRDN_MSK			0x01
#define PLL_PWRDN_SHIFT			0x0
#define PLL_BYPASS			BIT(1)
#define PLL_PWRDN			BIT(0)


#define NO_PLL_BYPASS			(0x00)
#define NO_PLL_PWRDN			(0x00)

#define FBDIV(n)		((0xfff << 16) | n)
#define POSTDIV2(n)		((0x7 << (12 + 16)) | (n << 12))
#define POSTDIV1(n)		((0x7 << (8 + 16)) | (n << 8))
#define REFDIV(n)		((0x3F << 16) | n)
#define PLL_LOCK(n)		((n >> 31) & 0x1)

#define PLL_SLOW_MODE			BITS_WITH_WMASK(SLOW_MODE,\
						PLL_MODE_MSK, PLL_MODE_SHIFT)

#define PLL_NOMAL_MODE			BITS_WITH_WMASK(NORMAL_MODE,\
						PLL_MODE_MSK, PLL_MODE_SHIFT)

#define PLL_BYPASS_MODE			BIT_WITH_WMSK(PLL_BYPASS_SHIFT)
#define PLL_NO_BYPASS_MODE		WMSK_BIT(PLL_BYPASS_SHIFT)

#define PLL_CON_COUNT			0x06
#define CRU_CLKSEL_COUNT		108
#define CRU_CLKSEL_CON(n)		(0x100 + (n) * 4)

#define PMUCRU_CLKSEL_CONUT		0x06
#define PMUCRU_CLKSEL_OFFSET		0x080
#define REG_SIZE			0x04
#define REG_SOC_WMSK			0xffff0000
#define CLK_GATE_MASK			0x01

#define PMUCRU_GATE_COUNT	0x03
#define CRU_GATE_COUNT		0x23
#define PMUCRU_GATE_CON(n)	(0x100 + (n) * 4)
#define CRU_GATE_CON(n)	(0x300 + (n) * 4)

#define PMUCRU_RSTNHOLD_CON0	0x120
enum {
	PRESETN_NOC_PMU_HOLD = 1,
	PRESETN_INTMEM_PMU_HOLD,
	HRESETN_CM0S_PMU_HOLD,
	HRESETN_CM0S_NOC_PMU_HOLD,
	DRESETN_CM0S_PMU_HOLD,
	POESETN_CM0S_PMU_HOLD,
	PRESETN_SPI3_HOLD,
	RESETN_SPI3_HOLD,
	PRESETN_TIMER_PMU_0_1_HOLD,
	RESETN_TIMER_PMU_0_HOLD,
	RESETN_TIMER_PMU_1_HOLD,
	PRESETN_UART_M0_PMU_HOLD,
	RESETN_UART_M0_PMU_HOLD,
	PRESETN_WDT_PMU_HOLD
};

#define PMUCRU_RSTNHOLD_CON1	0x124
enum {
	PRESETN_I2C0_HOLD,
	PRESETN_I2C4_HOLD,
	PRESETN_I2C8_HOLD,
	PRESETN_MAILBOX_PMU_HOLD,
	PRESETN_RKPWM_PMU_HOLD,
	PRESETN_PMUGRF_HOLD,
	PRESETN_SGRF_HOLD,
	PRESETN_GPIO0_HOLD,
	PRESETN_GPIO1_HOLD,
	PRESETN_CRU_PMU_HOLD,
	PRESETN_INTR_ARB_HOLD,
	PRESETN_PVTM_PMU_HOLD,
	RESETN_I2C0_HOLD,
	RESETN_I2C4_HOLD,
	RESETN_I2C8_HOLD
};

enum plls_id {
	ALPLL_ID = 0,
	ABPLL_ID,
	DPLL_ID,
	CPLL_ID,
	GPLL_ID,
	NPLL_ID,
	VPLL_ID,
	PPLL_ID,
	END_PLL_ID,
};

#define CLST_L_CPUS_MSK (0xf)
#define CLST_B_CPUS_MSK (0x3)

enum pll_work_mode {
	SLOW_MODE = 0x00,
	NORMAL_MODE = 0x01,
	DEEP_SLOW_MODE = 0x02,
};

#if 0 //TODO
enum glb_sft_reset {
	PMU_RST_BY_FIRST_SFT,
	PMU_RST_BY_SECOND_SFT = BIT(2),
	PMU_RST_NOT_BY_SFT = BIT(3),
};
#endif
struct pll_div {
	UINT32 mhz;
	UINT32 refdiv;
	UINT32 fbdiv;
	UINT32 postdiv1;
	UINT32 postdiv2;
	UINT32 frac;
	UINT32 freq;
};

struct deepsleep_data_s {
	UINT32 plls_con[END_PLL_ID][PLL_CON_COUNT];
	UINT32 cru_gate_con[CRU_GATE_COUNT];
	UINT32 pmucru_gate_con[PMUCRU_GATE_COUNT];
};

struct pmu_sleep_data {
	UINT32 pmucru_rstnhold_con0;
	UINT32 pmucru_rstnhold_con1;
};

#define CRU_CLKSEL_CON0		0x0100
#define CRU_CLKSEL_CON6		0x0118
#define CRU_SDIO0_CON1		0x058c
#define PMUCRU_CLKSEL_CON0	0x0080
#define PMUCRU_CLKGATE_CON2	0x0108
#define PMUCRU_SOFTRST_CON0	0x0110
#define PMUCRU_GATEDIS_CON0 0x0130
#define PMUCRU_SOFTRST_CON(n)   (PMUCRU_SOFTRST_CON0 + (n) * 4)

/**************************************************
 * pmugrf reg, offset
 **************************************************/
#define PMUGRF_OSREG(n)		(0x300 + (n) * 4)
#define PMUGRF_GPIO0A_P		0x040
#define PMUGRF_GPIO1A_P		0x050

/**************************************************
 * DCF reg, offset
 **************************************************/
#define DCF_DCF_CTRL		0x0
#define DCF_DCF_ADDR		0x8
#define DCF_DCF_ISR		0xc
#define DCF_DCF_TOSET		0x14
#define DCF_DCF_TOCMD		0x18
#define DCF_DCF_CMD_CFG		0x1c

/* DCF_DCF_ISR */
#define DCF_TIMEOUT		(1 << 2)
#define DCF_ERR			(1 << 1)
#define	DCF_DONE		(1 << 0)

/* DCF_DCF_CTRL */
#define DCF_VOP_HW_EN		(1 << 2)
#define DCF_STOP		(1 << 1)
#define DCF_START		(1 << 0)

#define CYCL_24M_CNT_US(us)	(24 * us)
#define CYCL_24M_CNT_MS(ms)	(ms * CYCL_24M_CNT_US(1000))
#define CYCL_32K_CNT_MS(ms)	(ms * 32)


//
// Public functions
//
VOID
RkUngateActiveClock (
  VOID
  );

EFI_STATUS
RkSetSAI2ClockRate (
  UINT32 ClockRate
  );

#endif
