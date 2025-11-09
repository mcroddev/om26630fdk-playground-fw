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
#include "hal/nvic.h"

enum {
	LPC_USB_BASE = 0x5000C000,
};

// clang-format off

#define USB_IRQn	(NVIC_IRQ_USB)
#define LPC_USB		((LPC_USB_T *)LPC_USB_BASE)

#define __I		volatile const
#define __IO		volatile
#define __O		volatile

// clang-format on

/** @defgroup USB_17XX_40XX CHIP: LPC17xx/40xx USB Device, Host, & OTG driver
 * @ingroup CHIP_17XX_40XX_Drivers
 * @{
 */

/**
 * @brief USB register block structure
 */
typedef struct {
	__I uint32_t Revision; /* USB Host Registers                 */
	__IO uint32_t Control;
	__IO uint32_t CommandStatus;
	__IO uint32_t InterruptStatus;
	__IO uint32_t InterruptEnable;
	__IO uint32_t InterruptDisable;
	__IO uint32_t HCCA;
	__I uint32_t PeriodCurrentED;
	__IO uint32_t ControlHeadED;
	__IO uint32_t ControlCurrentED;
	__IO uint32_t BulkHeadED;
	__IO uint32_t BulkCurrentED;
	__I uint32_t DoneHead;
	__IO uint32_t FmInterval;
	__I uint32_t FmRemaining;
	__I uint32_t FmNumber;
	__IO uint32_t PeriodicStart;
	__IO uint32_t LSTreshold;
	__IO uint32_t RhDescriptorA;
	__IO uint32_t RhDescriptorB;
	__IO uint32_t RhStatus;
	__IO uint32_t RhPortStatus1;
	__IO uint32_t RhPortStatus2;
	uint32_t RESERVED0[40];
	__I uint32_t Module_ID;

	__I uint32_t IntSt; /* USB On-The-Go Registers            */
	__IO uint32_t IntEn;
	__O uint32_t IntSet;
	__O uint32_t IntClr;
	__IO uint32_t StCtrl;
	__IO uint32_t Tmr;
	uint32_t RESERVED1[58];

	__I uint32_t DevIntSt; /* USB Device Interrupt Registers     */
	__IO uint32_t DevIntEn;
	__O uint32_t DevIntClr;
	__O uint32_t DevIntSet;

	__O uint32_t CmdCode; /* USB Device SIE Command Registers   */
	__I uint32_t CmdData;

	__I uint32_t RxData; /* USB Device Transfer Registers      */
	__O uint32_t TxData;
	__I uint32_t RxPLen;
	__O uint32_t TxPLen;
	__IO uint32_t Ctrl;
	__O uint32_t DevIntPri;

	__I uint32_t EpIntSt; /* USB Device Endpoint Interrupt Regs */
	__IO uint32_t EpIntEn;
	__O uint32_t EpIntClr;
	__O uint32_t EpIntSet;
	__O uint32_t EpIntPri;

	__IO uint32_t ReEp; /* USB Device Endpoint Realization Reg*/
	__O uint32_t EpInd;
	__IO uint32_t MaxPSize;

	__I uint32_t DMARSt; /* USB Device DMA Registers           */
	__O uint32_t DMARClr;
	__O uint32_t DMARSet;
	uint32_t RESERVED2[9];
	__IO uint32_t UDCAH;
	__I uint32_t EpDMASt;
	__O uint32_t EpDMAEn;
	__O uint32_t EpDMADis;
	__I uint32_t DMAIntSt;
	__IO uint32_t DMAIntEn;
	uint32_t RESERVED3[2];
	__I uint32_t EoTIntSt;
	__O uint32_t EoTIntClr;
	__O uint32_t EoTIntSet;
	__I uint32_t NDDRIntSt;
	__O uint32_t NDDRIntClr;
	__O uint32_t NDDRIntSet;
	__I uint32_t SysErrIntSt;
	__O uint32_t SysErrIntClr;
	__O uint32_t SysErrIntSet;
	uint32_t RESERVED4[15];

	union {
		__I uint32_t I2C_RX; /* USB OTG I2C Registers              */
		__O uint32_t I2C_TX;
	};

	__IO uint32_t I2C_STS;
	__IO uint32_t I2C_CTL;
	__IO uint32_t I2C_CLKHI;
	__O uint32_t I2C_CLKLO;
	uint32_t RESERVED5[824];

	union {
		__IO uint32_t
			USBClkCtrl; /* USB Clock Control Registers        */
		__IO uint32_t OTGClkCtrl;
	};

	union {
		__I uint32_t USBClkSt;
		__I uint32_t OTGClkSt;
	};
} LPC_USB_T;

ALWAYS_INLINE void NVIC_EnableIRQ(const enum nvic_irq irq)
{
	nvic_irq_enable(irq);
}

ALWAYS_INLINE void NVIC_DisableIRQ(const enum nvic_irq irq)
{
	nvic_irq_disable(irq);
}

ALWAYS_INLINE void NVIC_ClearPendingIRQ(const enum nvic_irq irq)
{
	nvic_irq_clr_pending(irq);
}
