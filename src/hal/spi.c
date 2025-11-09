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

#include <stdbool.h>

#include "nvic.h"
#include "spi.h"
#include "sysctl.h"

enum ssp_base_addr {
	SSP0_BASE_ADDR = 0x40088000,
	SSP1_BASE_ADDR = 0x40030000,
};

enum ssp_reg {
	SSP_REG_CR0 = 0x0,
	SSP_REG_CR1 = 0x4,
	SSP_REG_DR = 0x8,
	SSP_REG_SR = 0xC,
	SSP_REG_CPSR = 0x10,
	SSP_REG_IMSC = 0x14,
	SSP_REG_RIS = 0x18,
	SSP_REG_MIS = 0x1C,
	SSP_REG_ICR = 0x20,
	SSP_REG_DMACR = 0x24
};

enum {
	CR0_FRF_SPI = 0,
	CR0_FRF_TI = 1,
	CR0_FRF_MICROWIRE = 2,

	CR0_DSS_MASK = BITMASK_FROM_RANGE(0, 3),
	CR0_FRF_MASK = BITMASK_FROM_RANGE(4, 5),
	CR0_SCR_MASK = BITMASK_FROM_RANGE(8, 15),

	CR0_CPOL = BIT_6,
	CR0_CPHA = BIT_7
};

enum {
	CR1_SSE = BIT_1,
	CR1_MS = BIT_2,
};

enum {
	IMSC_BIT_RORIM = BIT_0,
	IMSC_BIT_RTIM = BIT_1,
	IMSC_BIT_RXIM = BIT_2,
	IMSC_BIT_TXIM = BIT_3
};

enum {
	SR_TFE = BIT_0,
	SR_TNF = BIT_1,
	SR_RNE = BIT_2,
	SR_RFF = BIT_3,
	SR_BSY = BIT_4,
};

enum {
	CPSR_CPSDVSR_MASK = BITMASK_FROM_RANGE(0, 7),
};

static struct {
	const enum ssp_base_addr base_addr;
	const enum sysctl_pconp_bit pconp_bit;
	const enum sysctl_reg pclksel_reg;
	const enum sysctl_pclksel_mask pclksel_mask;
	const enum nvic_irq irq;
} spi_inst[] = {
	// clang-format off

	[SPI_INSTANCE_SPI0]	= {
		.base_addr	= SSP0_BASE_ADDR,
		.pconp_bit	= SYSCTL_PCONP_BIT_PCSSP0,
		.pclksel_reg	= SYSCTL_REG_PCLKSEL1,
		.pclksel_mask	= SYSCTL_PCLKSEL1_MASK_PCLK_SSP0,
		.irq		= NVIC_IRQ_SSP0
	},

	[SPI_INSTANCE_SPI1] = {
		.base_addr	= SSP1_BASE_ADDR,
		.pconp_bit	= SYSCTL_PCONP_BIT_PCSSP1,
		.pclksel_reg	= SYSCTL_REG_PCLKSEL0,
		.pclksel_mask	= SYSCTL_PCLKSEL0_MASK_PCLK_SSP1,
		.irq		= NVIC_IRQ_SSP1
	}

	// clang-format on
};

ALWAYS_INLINE u32 ssp_reg_read(const enum spi_instance inst,
			       const enum ssp_reg reg)
{
	return mmio_read32(spi_inst[inst].base_addr + reg);
}

ALWAYS_INLINE void ssp_reg_write(const enum spi_instance inst,
				 const enum ssp_reg reg, const u32 val)
{
	mmio_write32(spi_inst[inst].base_addr + reg, val);
}

ALWAYS_INLINE bool tx_fifo_not_full(const enum spi_instance inst)
{
	return ssp_reg_read(inst, SSP_REG_SR) & SR_TNF;
}

ALWAYS_INLINE bool rx_fifo_not_empty(const enum spi_instance inst)
{
	return ssp_reg_read(inst, SSP_REG_SR) & SR_RNE;
}

#ifndef NDEBUG
static void handle_isr(const enum spi_instance inst)
{
	const u32 MIS = ssp_reg_read(inst, SSP_REG_MIS);

	switch (MIS) {
	case IMSC_BIT_RORIM:
		// We're not processing data from the RXFIFO fast enough!
		app_assert(false);
		break;

	default:
		// Unhandled interrupt?
		app_assert(false);
		break;
	}
}

void isr_SSP0(void)
{
	handle_isr(SPI_INSTANCE_SPI0);
}

void isr_SSP1(void)
{
	handle_isr(SPI_INSTANCE_SPI1);
}
#endif // NDEBUG

static void moto_spi_cpol_mode_set(const enum spi_instance inst,
				   const enum spi_cfg_moto_spi_cpol cpol)
{
	u32 CR0 = ssp_reg_read(inst, SSP_REG_CR0);

	if (cpol == SPI_CFG_MOTO_SPI_CPOL_LOW)
		CR0 &= ~CR0_CPOL;
	else if (cpol == SPI_CFG_MOTO_SPI_CPOL_HIGH)
		CR0 |= CR0_CPOL;
	else
		UNREACHABLE;

	ssp_reg_write(inst, SSP_REG_CR0, CR0);
}

static void moto_spi_cpha_mode_set(const enum spi_instance inst,
				   const enum spi_cfg_moto_spi_cpha cpha)
{
	u32 CR0 = ssp_reg_read(inst, SSP_REG_CR0);

	if (cpha == SPI_CFG_MOTO_SPI_CPHA_FIRST)
		CR0 &= ~CR0_CPHA;
	else if (cpha == SPI_CFG_MOTO_SPI_CPHA_SECOND)
		CR0 |= CR0_CPHA;
	else
		UNREACHABLE;

	ssp_reg_write(inst, SSP_REG_CR0, CR0);
}

void spi_init_moto_master(const enum spi_instance inst,
			  const struct spi_cfg_moto_master *const cfg)
{
	// The two SSP interfaces, SSP0 and SSP1 are configured using the
	// following registers:

	// 1. Power: In the PCONP register, set bit PCSSP0 to enable SSP0 and
	//    bit PCSSP1 to enable SSP1.
	sysctl_peripheral_power_enable(spi_inst[inst].pconp_bit);

	// 2. Clock: In PCLKSEL0 select PCLK_SSP1; in PCLKSEL1 select PCLK_SSP0.
	//    In master mode, the clock must be scaled down.
	mmio_rmw_mask32(spi_inst[inst].pclksel_reg, spi_inst[inst].pclksel_mask,
			cfg->clk_speed);

	// 3. Pins: Select the SSP pins through the PINSEL registers and pin
	//    modes through the PINMODE registers.
	//
	// The application is responsible for this step.

	// 4. Interrupts: Interrupts are enabled in the SSP0IMSC register for
	//    SSP0 and SSP1IMSC register for SSP1. Interrupts are enabled in the
	//    NVIC using the appropriate Interrupt Set Enable register
#ifndef NDEBUG
	u32 IMSC = ssp_reg_read(inst, SSP_REG_IMSC);
	IMSC |= IMSC_BIT_RORIM;
	ssp_reg_write(inst, SSP_REG_IMSC, IMSC);
	nvic_irq_enable(spi_inst[inst].irq);
#endif // NDEBUG

	// 5. Initialization: There are two control registers for each of the
	//    SSP ports to be configured: SSP0CR0 and SSP0CR1 for SSP0, SSP1CR0
	//    and SSP1CR1 for SSP1.
	u32 CR0 = ssp_reg_read(inst, SSP_REG_CR0);
	set_val_by_mask(CR0, CR0_DSS_MASK, cfg->data_size);
	set_val_by_mask(CR0, CR0_FRF_MASK, CR0_FRF_SPI);
	set_val_by_mask(CR0, CR0_SCR_MASK, cfg->serial_clk_rate);
	ssp_reg_write(inst, SSP_REG_CR0, CR0);

	u32 CPSR = 0;
	set_val_by_mask(CPSR, CPSR_CPSDVSR_MASK, cfg->prescaler);
	ssp_reg_write(inst, SSP_REG_CPSR, CPSR);

	moto_spi_cpol_mode_set(inst, cfg->cpol);
	moto_spi_cpha_mode_set(inst, cfg->cpha);

	u32 CR1 = ssp_reg_read(inst, SSP_REG_CR1);
	CR1 &= ~CR1_MS;
	CR1 |= CR1_SSE;
	ssp_reg_write(inst, SSP_REG_CR1, CR1);
}

void spi_tx_blocking_u8(const enum spi_instance inst, const u8 *const src,
			const u32 src_size)
{
	for (u32 i = 0; i < src_size;) {
		while (!tx_fifo_not_full(inst))
			nop();

		ssp_reg_write(inst, SSP_REG_DR, src[i++]);

		while (!rx_fifo_not_empty(inst))
			nop();

		ssp_reg_read(inst, SSP_REG_DR);
	}

	while (ssp_reg_read(inst, SSP_REG_SR) & SR_BSY)
		nop();
}

void spi_tx_rx_blocking_u8(const enum spi_instance inst, const u8 *const src,
			   u8 *const dst, const u32 size)
{
	u32 src_idx = 0;
	u32 rx_idx = 0;

	while ((src_idx < size) && (rx_idx < size)) {
		while (!tx_fifo_not_full(inst))
			nop();

		ssp_reg_write(inst, SSP_REG_DR, src[src_idx++]);

		while (!rx_fifo_not_empty(inst))
			nop();

		dst[rx_idx++] = ssp_reg_read(inst, SSP_REG_DR);
	}

	while (ssp_reg_read(inst, SSP_REG_SR) & SR_BSY)
		nop();
}
