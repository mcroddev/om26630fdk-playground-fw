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

/** @file startup.c Defines the startup code for the NXP LPC1769. */

#include "common/compiler.h"
#include "common/types.h"

enum {
	// ARMv6-M exception numbers
	VEC_SP_main = 0,
	VEC_Reset = 1,
	VEC_NMI = 2,
	VEC_HardFault = 3,
	VEC_MemManage = 4,
	VEC_BusFault = 5,
	VEC_UsageFault = 6,
	VEC_SVCall = 11,
	VEC_DebugMonitor = 12,
	VEC_PendSV = 14,
	VEC_SysTick = 15,

	// NXP LPC1769 exception numbers
	VEC_WDT = 16,
	VEC_T0 = 17,
	VEC_T1 = 18,
	VEC_T2 = 19,
	VEC_T3 = 20,
	VEC_UART0 = 21,
	VEC_UART1 = 22,
	VEC_UART2 = 23,
	VEC_UART3 = 24,
	VEC_PWM1 = 25,
	VEC_I2C0 = 26,
	VEC_I2C1 = 27,
	VEC_I2C2 = 28,
	VEC_SPI = 29,
	VEC_SSP0 = 30,
	VEC_SSP1 = 31,
	VEC_PLL0 = 32,
	VEC_RTC = 33,
	VEC_EINT0 = 34,
	VEC_EINT1 = 35,
	VEC_EINT2 = 36,
	VEC_EINT3 = 37,
	VEC_ADC = 38,
	VEC_BOD = 39,
	VEC_USB = 40,
	VEC_CAN = 41,
	VEC_GPDMA = 42,
	VEC_I2S = 43,
	VEC_ETH = 44,
	VEC_RIT = 45,
	VEC_MCPWM = 46,
	VEC_QEI = 47,
	VEC_PLL1 = 48,
	VEC_USBACT = 49,
	VEC_CANACT = 50
};

extern void _start(void);
extern u32 __stack;

#define ISR_FUNC WEAK_SYMBOL ALIAS("isr_unhandled")

// ARMv6-M vector entry functions
ISR_FUNC void isr_NMI(void);
ISR_FUNC void isr_HardFault(void);
ISR_FUNC void isr_MemManage(void);
ISR_FUNC void isr_BusFault(void);
ISR_FUNC void isr_UsageFault(void);
ISR_FUNC void isr_SVCall(void);
ISR_FUNC void isr_DebugMonitor(void);
ISR_FUNC void isr_PendSV(void);
ISR_FUNC void isr_SysTick(void);

// NXP LPC1769 vector entry functions
ISR_FUNC void isr_WDT(void);
ISR_FUNC void isr_T0(void);
ISR_FUNC void isr_T1(void);
ISR_FUNC void isr_T2(void);
ISR_FUNC void isr_T3(void);
ISR_FUNC void isr_UART0(void);
ISR_FUNC void isr_UART1(void);
ISR_FUNC void isr_UART2(void);
ISR_FUNC void isr_UART3(void);
ISR_FUNC void isr_PWM1(void);
ISR_FUNC void isr_I2C0(void);
ISR_FUNC void isr_I2C1(void);
ISR_FUNC void isr_I2C2(void);
ISR_FUNC void isr_SPI(void);
ISR_FUNC void isr_SSP0(void);
ISR_FUNC void isr_SSP1(void);
ISR_FUNC void isr_PLL0(void);
ISR_FUNC void isr_RTC(void);
ISR_FUNC void isr_EINT0(void);
ISR_FUNC void isr_EINT1(void);
ISR_FUNC void isr_EINT2(void);
ISR_FUNC void isr_EINT3(void);
ISR_FUNC void isr_ADC(void);
ISR_FUNC void isr_BOD(void);
ISR_FUNC void isr_USB(void);
ISR_FUNC void isr_CAN(void);
ISR_FUNC void isr_GPDMA(void);
ISR_FUNC void isr_I2S(void);
ISR_FUNC void isr_ETH(void);
ISR_FUNC void isr_RIT(void);
ISR_FUNC void isr_MCPWM(void);
ISR_FUNC void isr_QEI(void);
ISR_FUNC void isr_PLL1(void);
ISR_FUNC void isr_USBACT(void);
ISR_FUNC void isr_CANACT(void);

static void isr_unhandled(void)
{
	for (;;)
		;
}

PLACE_IN_SECTION(".init")
const void *const __interrupt_vector[] = {
	// ARMv6-M vector table entries
	[VEC_SP_main] = &__stack,
	[VEC_Reset] = _start,
	[VEC_NMI] = isr_NMI,
	[VEC_HardFault] = isr_HardFault,
	[VEC_MemManage] = isr_MemManage,
	[VEC_BusFault] = isr_BusFault,
	[VEC_UsageFault] = isr_UsageFault,
	[VEC_SVCall] = isr_SVCall,
	[VEC_DebugMonitor] = isr_DebugMonitor,
	[VEC_PendSV] = isr_PendSV,
	[VEC_SysTick] = isr_SysTick,

	// NXP LPC1769 vector table entries
	[VEC_WDT] = isr_WDT,
	[VEC_T0] = isr_T0,
	[VEC_T1] = isr_T1,
	[VEC_T2] = isr_T2,
	[VEC_T3] = isr_T3,
	[VEC_UART0] = isr_UART0,
	[VEC_UART1] = isr_UART1,
	[VEC_UART2] = isr_UART2,
	[VEC_UART3] = isr_UART3,
	[VEC_PWM1] = isr_PWM1,
	[VEC_I2C0] = isr_I2C0,
	[VEC_I2C1] = isr_I2C1,
	[VEC_I2C2] = isr_I2C2,
	[VEC_SPI] = isr_SPI,
	[VEC_SSP0] = isr_SSP0,
	[VEC_SSP1] = isr_SSP1,
	[VEC_PLL0] = isr_PLL0,
	[VEC_RTC] = isr_RTC,
	[VEC_EINT0] = isr_EINT0,
	[VEC_EINT1] = isr_EINT1,
	[VEC_EINT2] = isr_EINT2,
	[VEC_EINT3] = isr_EINT3,
	[VEC_ADC] = isr_ADC,
	[VEC_BOD] = isr_BOD,
	[VEC_USB] = isr_USB,
	[VEC_CAN] = isr_CAN,
	[VEC_GPDMA] = isr_GPDMA,
	[VEC_I2S] = isr_I2S,
	[VEC_ETH] = isr_ETH,
	[VEC_RIT] = isr_RIT,
	[VEC_MCPWM] = isr_MCPWM,
	[VEC_QEI] = isr_QEI,
	[VEC_PLL1] = isr_PLL1,
	[VEC_USBACT] = isr_USBACT,
	[VEC_CANACT] = isr_CANACT
};