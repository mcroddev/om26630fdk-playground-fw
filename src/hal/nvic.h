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

#pragma once

#include "common/compiler.h"
#include "common/types.h"
#include "util.h"

enum nvic_irq {
	NVIC_IRQ_WDT,
	NVIC_IRQ_T0,
	NVIC_IRQ_T1,
	NVIC_IRQ_T2,
	NVIC_IRQ_T3,
	NVIC_IRQ_UART0,
	NVIC_IRQ_UART1,
	NVIC_IRQ_UART2,
	NVIC_IRQ_UART3,
	NVIC_IRQ_PWM1,
	NVIC_IRQ_I2C0,
	NVIC_IRQ_I2C1,
	NVIC_IRQ_I2C2,
	NVIC_IRQ_SPI,
	NVIC_IRQ_SSP0,
	NVIC_IRQ_SSP1,
	NVIC_IRQ_PLL0,
	NVIC_IRQ_RTC,
	NVIC_IRQ_EINT0,
	NVIC_IRQ_EINT1,
	NVIC_IRQ_EINT2,
	NVIC_IRQ_EINT3,
	NVIC_IRQ_ADC,
	NVIC_IRQ_BOD,
	NVIC_IRQ_USB,
	NVIC_IRQ_CAN,
	NVIC_IRQ_GPDMA,
	NVIC_IRQ_I2S,
	NVIC_IRQ_ETH,
	NVIC_IRQ_RIT,
	NVIC_IRQ_MCPWM,
	NVIC_IRQ_QEI,
	NVIC_IRQ_PLL1,
	NVIC_IRQ_USBACT,
	NVIC_IRQ_CANACT
};

enum nvic_iser {
	NVIC_ISER0 = 0xE000E100,
	NVIC_ISER1 = 0xE000E104,
	NVIC_ISER2 = 0xE000E108,
	NVIC_ISER3 = 0xE000E10C,
	NVIC_ISER4 = 0xE000E110,
	NVIC_ISER5 = 0xE000E114,
	NVIC_ISER6 = 0xE000E118,
	NVIC_ISER7 = 0xE000E11C,
	NVIC_ISER8 = 0xE000E120,
	NVIC_ISER9 = 0xE000E124,
	NVIC_ISER10 = 0xE000E128,
	NVIC_ISER11 = 0xE000E12C,
	NVIC_ISER12 = 0xE000E130,
	NVIC_ISER13 = 0xE000E134,
	NVIC_ISER14 = 0xE000E138,
	NVIC_ISER15 = 0xE000E13C
};

enum nvic_icer {
	NVIC_ICER0 = 0xE000E180,
	NVIC_ICER1 = 0xE000E184,
	NVIC_ICER2 = 0xE000E188,
	NVIC_ICER3 = 0xE000E18C,
	NVIC_ICER4 = 0xE000E190,
	NVIC_ICER5 = 0xE000E194,
	NVIC_ICER6 = 0xE000E198,
	NVIC_ICER7 = 0xE000E19C,
	NVIC_ICER8 = 0xE000E1A0,
	NVIC_ICER9 = 0xE000E1A4,
	NVIC_ICER10 = 0xE000E1A8,
	NVIC_ICER11 = 0xE000E1AC,
	NVIC_ICER12 = 0xE000E1B0,
	NVIC_ICER13 = 0xE000E1B4,
	NVIC_ICER14 = 0xE000E1B8,
	NVIC_ICER15 = 0xE000E1BC
};

enum nvic_icpr {
	NVIC_ICPR0 = 0xE000E200,
	NVIC_ICPR1 = 0xE000E204,
	NVIC_ICPR2 = 0xE000E208,
	NVIC_ICPR3 = 0xE000E20C,
	NVIC_ICPR4 = 0xE000E210,
	NVIC_ICPR5 = 0xE000E214,
	NVIC_ICPR6 = 0xE000E218,
	NVIC_ICPR7 = 0xE000E21C,
	NVIC_ICPR8 = 0xE000E220,
	NVIC_ICPR9 = 0xE000E224,
	NVIC_ICPR10 = 0xE000E228,
	NVIC_ICPR11 = 0xE000E22C,
	NVIC_ICPR12 = 0xE000E230,
	NVIC_ICPR13 = 0xE000E234,
	NVIC_ICPR14 = 0xE000E238,
	NVIC_ICPR15 = 0xE000E23C
};

ALWAYS_INLINE void nvic_irq_enable(const enum nvic_irq irq)
{
	const enum nvic_iser iser_addr =
		NVIC_ISER0 + ((irq >> 5) * sizeof(u32));

	mmio_set32(iser_addr, UINT32_C(1) << (irq & 0x1F));
}

ALWAYS_INLINE void nvic_irq_disable(const enum nvic_irq irq)
{
	const enum nvic_icer icer_addr =
		NVIC_ICER0 + ((irq >> 5) * sizeof(u32));

	mmio_set32(icer_addr, UINT32_C(1) << (irq & 0x1F));
}

ALWAYS_INLINE void nvic_irq_clr_pending(const enum nvic_irq irq)
{
	const enum nvic_icpr icpr_addr =
		NVIC_ICPR0 + ((irq >> 5) * sizeof(u32));

	mmio_set32(icpr_addr, UINT32_C(1) << (irq & 0x1F));
}