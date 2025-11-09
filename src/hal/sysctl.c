// SPDX-License-Identifier: MIT
//
// Copyright 2025 Michael Rodriguez
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "sysctl.h"
#include "util.h"

enum {
	USBCLKCFG_MASK_USBSEL = BITMASK_FROM_RANGE(0, 3),
};

enum {
	PLL_FEED_SEQ_FIRST = 0xAA,
	PLL_FEED_SEQ_SECOND = 0x55,
};

enum {
	CLKOUTCFG_CLKOUTSEL = BITMASK_FROM_RANGE(0, 3),
	CLKOUTCFG_CLKOUTDIV = BITMASK_FROM_RANGE(4, 7),
	CLKOUTCFG_CLKOUT_EN = BIT_8
};

enum {
	SCS_OSCRANGE = BIT_4,
	SCS_OSCEN = BIT_5,
	SCS_OSCSTAT = BIT_6,
};

enum {
	PLL0CON_PLLE0 = BIT_0,
	PLL0CON_PLLC0 = BIT_1,
};

enum {
	FLASHCFG_FLASHTIM = BITMASK_FROM_RANGE(12, 15),
};

enum {
	CLKSRCSEL_CLKSRC = BIT_1 | BIT_0,
};

enum {
	CCLKCFG_CCLKSEL = BITMASK_FROM_RANGE(0, 7),
};

enum {
	PLL0CFG_MASK_MSEL0 = BITMASK_FROM_RANGE(0, 15),
	PLL0CFG_MASK_NSEL0 = BITMASK_FROM_RANGE(16, 23)
};

enum {
	PLL0STAT_PLOCK0 = BIT_26,
};

ALWAYS_INLINE void pll_feed_seq(const enum sysctl_reg pll_feed_reg)
{
	mmio_write32(pll_feed_reg, PLL_FEED_SEQ_FIRST);
	mmio_write32(pll_feed_reg, PLL_FEED_SEQ_SECOND);
}

void sysctl_peripheral_power_enable(const enum sysctl_pconp_bit mask)
{
	mmio_set32(SYSCTL_REG_PCONP, mask);
}

void sysctl_peripheral_power_disable(const enum sysctl_pconp_bit mask)
{
	mmio_clr32(SYSCTL_REG_PCONP, mask);
}

void sysctl_main_osc_enable(const enum sysctl_main_osc_range osc_range)
{
	u32 SCS = mmio_read32(SYSCTL_REG_SCS);

	if (osc_range == SYSCTL_MAIN_OSC_RANGE_1_TO_20_MHZ)
		SCS &= ~SCS_OSCRANGE;
	else if (osc_range == SYSCTL_MAIN_OSC_RANGE_15_TO_25_MHZ)
		SCS |= SCS_OSCRANGE;
	else
		UNREACHABLE;

	SCS |= SCS_OSCEN;

	mmio_write32(SYSCTL_REG_SCS, SCS);

	while (!(mmio_read32(SYSCTL_REG_SCS) & SCS_OSCSTAT))
		nop();
}

void sysctl_pll0_cclk_cfg(const struct sysctl_pll_cfg *const cfg)
{
	// The following sequence must be followed step by step in order to have
	// PLL0 initialized and running:

	// 1. Disconnect PLL0 with one feed sequence if PLL0 is already
	//    connected.
	u32 PLL0CON = mmio_read32(SYSCTL_REG_PLL0CON);
	if (PLL0CON & PLL0CON_PLLC0) {
		PLL0CON &= ~PLL0CON_PLLC0;
		mmio_write32(SYSCTL_REG_PLL0CON, PLL0CON);
		pll_feed_seq(SYSCTL_REG_PLL0FEED);
	}

	// 2. Disable PLL0 with one feed sequence.
	PLL0CON &= ~PLL0CON_PLLE0;
	mmio_write32(SYSCTL_REG_PLL0CON, PLL0CON);
	pll_feed_seq(SYSCTL_REG_PLL0FEED);

	// 3. Change the CPU Clock Divider setting to speed up operation without
	//    PLL0, if desired.
	//
	//    Will not implement.

	// 4. Write to the Clock Source Selection Control register to change the
	//    clock source if needed.
	mmio_rmw_mask32(SYSCTL_REG_CLKSRCSEL, CLKSRCSEL_CLKSRC, cfg->osc_src);

	// 5. Write to the PLL0CFG and make it effective with one feed sequence.
	//    The PLL0CFG can only be updated when PLL0 is disabled.
	u32 PLL0CFG = mmio_read32(SYSCTL_REG_PLL0CFG);
	set_val_by_mask(PLL0CFG, PLL0CFG_MASK_MSEL0, cfg->pll_mul - 1);
	set_val_by_mask(PLL0CFG, PLL0CFG_MASK_NSEL0, cfg->pll_div - 1);
	mmio_write32(SYSCTL_REG_PLL0CFG, PLL0CFG);
	pll_feed_seq(SYSCTL_REG_PLL0FEED);

	// 6. Enable PLL0 with one feed sequence.
	PLL0CON |= PLL0CON_PLLE0;
	mmio_write32(SYSCTL_REG_PLL0CON, PLL0CON);
	pll_feed_seq(SYSCTL_REG_PLL0FEED);

	// 7. Change the CPU Clock Divider setting for the operation with PLL0.
	//    It is critical to do this before connecting PLL0.
	mmio_rmw_mask32(SYSCTL_REG_CCLKCFG, CCLKCFG_CCLKSEL,
			cfg->cclkcfg_div - 1);

	// 8. Wait for PLL0 to achieve lock by monitoring the PLOCK0 bit in the
	//    PLL0STAT register, or using the PLOCK0 interrupt, or wait for a
	//    fixed time when the input clock to PLL0 is slow (i.e. 32 kHz).
	//    The value of PLOCK0 may not be stable when the PLL reference
	//    frequency (FREF, the frequency of REFCLK, which is equal to the
	//    PLL input frequency divided by the pre-divider value) is less than
	//    100 kHz or greater than 20 MHz. In these cases, the PLL may be
	//    assumed to be stable after a start-up time has passed. This time
	//    is 500us when FREF is greater than 400 kHz and 200 / FREF seconds
	//    when FREF is less than 400 kHz.
	while (!(mmio_read32(SYSCTL_REG_PLL0STAT) & PLL0STAT_PLOCK0))
		nop();

	// 9. Connect PLL0 with one feed sequence.
	PLL0CON |= PLL0CON_PLLC0;
	mmio_write32(SYSCTL_REG_PLL0CON, PLL0CON);
	pll_feed_seq(SYSCTL_REG_PLL0FEED);

	// It is very important not to merge any steps above. For example, do
	// not update the PLL0CFG and enable PLL0 simultaneously with the same
	// feed sequence.
}

void sysctl_flash_access_time_set(
	const enum sysctl_flash_access_time flash_access_time)
{
	mmio_rmw_mask32(SYSCTL_REG_FLASHCFG, FLASHCFG_FLASHTIM,
			flash_access_time);
}

void sysctl_usbclk_set(const enum sysctl_usbclk clk)
{
	u32 USBCLKCFG = 0;
	set_val_by_mask(USBCLKCFG, USBCLKCFG_MASK_USBSEL, clk);
	mmio_write32(SYSCTL_REG_USBCLKCFG, USBCLKCFG);
}

void sysctl_clkout_cfg_set(const enum sysctl_clkout_clk_src clk_src,
			   const enum sysctl_clkout_clk_div clk_div)
{
	u32 CLKOUTCFG = 0;
	set_val_by_mask(CLKOUTCFG, CLKOUTCFG_CLKOUTSEL, clk_src);
	set_val_by_mask(CLKOUTCFG, CLKOUTCFG_CLKOUTDIV, clk_div);
	CLKOUTCFG |= CLKOUTCFG_CLKOUT_EN;

	mmio_write32(SYSCTL_REG_CLKOUTCFG, CLKOUTCFG);
}
