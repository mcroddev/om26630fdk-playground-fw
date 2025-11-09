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

#ifndef DRV_CLRC663_H
#define DRV_CLRC663_H

#include <stddef.h>
#include <stdint.h>

#include "clrc663-cmd.h"

enum {
	DRV_CLRC663_FIFOControl_FIFOSize = UINT8_C(1) << 7,
	DRV_CLRC663_FIFOControl_FIFOFlush = UINT8_C(1) << 4,
	DRV_CLRC663_FIFOControl_MASK_FIFOLengthExtBits = (UINT8_C(1) << 1) |
							 (UINT8_C(1) << 0),

	DRV_CLRC663_FIFOControl_SHIFT_FIFOLengthExtBits = 0
};

enum {
	DRV_CLRC663_FIFO_NUM_BYTES_MAX = 512,
	DRV_CLRC663_MIFARE_CLASSIC_KEY_NUM_BYTES = 6
};

enum drv_clrc663_fifo_mode {
	DRV_CLRC663_FIFO_MODE_255,
	DRV_CLRC663_FIFO_MODE_512
};

enum drv_clrc663_reg {
	/** Starts and stops command execution */
	DRV_CLRC663_REG_Command = UINT8_C(0x00),

	/** Host control register */
	DRV_CLRC663_REG_HostCtrl = UINT8_C(0x01),

	/** Control register of the FIFO */
	DRV_CLRC663_REG_FIFOControl = UINT8_C(0x02),

	/** Level of the FIFO underflow and overflow warning */
	DRV_CLRC663_REG_WaterLevel = UINT8_C(0x03),

	/** Length of the FIFO */
	DRV_CLRC663_REG_FIFOLength = UINT8_C(0x04),

	/** Data In/Out exchange register of FIFO buffer */
	DRV_CLRC663_REG_FIFOData = UINT8_C(0x05),

	/** Interrupt register 0 */
	DRV_CLRC663_REG_IRQ0 = UINT8_C(0x06),

	/** Interrupt register 1 */
	DRV_CLRC663_REG_IRQ1 = UINT8_C(0x07),

	/** Interrupt enable register 0 */
	DRV_CLRC663_REG_IRQ0En = UINT8_C(0x08),

	/** Interrupt enable register 1 */
	DRV_CLRC663_REG_IRQ1En = UINT8_C(0x09),

	/** Error bits showing the error status of the last command execution */
	DRV_CLRC663_REG_Error = UINT8_C(0x0A),

	/** Contains status of the communication */
	DRV_CLRC663_REG_Status = UINT8_C(0x0B),

	/**
	 * Control register for anticollision adjustments for bit oriented
	 * protocols
	 */
	DRV_CLRC663_REG_RxBitCtrl = UINT8_C(0x0C),

	/** Collision position register */
	DRV_CLRC663_REG_RxColl = UINT8_C(0x0D),

	/** Control of Timer 0..3 */
	DRV_CLRC663_REG_TControl = UINT8_C(0x0E),

	/** Control of Timer0 */
	DRV_CLRC663_REG_T0Control = UINT8_C(0x0F),

	/** High register of the reload value of Timer0 */
	DRV_CLRC663_REG_T0ReloadHi = UINT8_C(0x10),

	/** Low register of the reload value of Timer0 */
	DRV_CLRC663_REG_T0ReloadLo = UINT8_C(0x11),

	/** Counter value high register of Timer0 */
	DRV_CLRC663_REG_T0CounterValHi = UINT8_C(0x12),

	/** Counter value low register of Timer0 */
	DRV_CLRC663_REG_T0CounterValLo = UINT8_C(0x13),

	/** Control of Timer1 */
	DRV_CLRC663_REG_T1Control = UINT8_C(0x14),

	/** High register of the reload value of Timer1 */
	DRV_CLRC663_REG_T1ReloadHi = UINT8_C(0x15),

	/** Low register of the reload value of Timer1 */
	DRV_CLRC663_REG_T1ReloadLo = UINT8_C(0x16),

	/** Counter value high register of Timer1 */
	DRV_CLRC663_REG_T1CounterValHi = UINT8_C(0x17),

	/** Counter value low register of Timer1 */
	DRV_CLRC663_REG_T1CounterValLo = UINT8_C(0x18),

	/** Control of Timer2 */
	DRV_CLRC663_REG_T2Control = UINT8_C(0x19),

	/** High byte of the reload value of Timer2 */
	DRV_CLRC663_REG_T2ReloadHi = UINT8_C(0x1A),

	/** Low byte of the reload value of Timer2 */
	DRV_CLRC663_REG_T2ReloadLo = UINT8_C(0x1B),

	/** Counter value high byte of Timer2 */
	DRV_CLRC663_REG_T2CounterValHi = UINT8_C(0x1C),

	/** Counter value low byte of Timer2 */
	DRV_CLRC663_REG_T2CounterValLo = UINT8_C(0x1D),

	/** Control of Timer3 */
	DRV_CLRC663_REG_T3Control = UINT8_C(0x1E),

	/** High byte of the reload value of Timer3 */
	DRV_CLRC663_REG_T3ReloadHi = UINT8_C(0x1F),

	/** Low byte of the reload value of Timer3 */
	DRV_CLRC663_REG_T3ReloadLo = UINT8_C(0x20),

	/** Counter value high byte of Timer3 */
	DRV_CLRC663_REG_T3CounterValHi = UINT8_C(0x21),

	/** Counter value low byte of Timer3 */
	DRV_CLRC663_REG_T3CounterValLo = UINT8_C(0x22),

	/** Control of Timer4 */
	DRV_CLRC663_REG_T4Control = UINT8_C(0x23),

	/** High byte of the reload value of Timer4 */
	DRV_CLRC663_REG_T4ReloadHi = UINT8_C(0x24),

	/** Low byte of the reload value of Timer4 */
	DRV_CLRC663_REG_T4ReloadLo = UINT8_C(0x25),

	/** Counter value high byte of Timer4 */
	DRV_CLRC663_REG_T4CounterValHi = UINT8_C(0x26),

	/** Counter value low byte of Timer4 */
	DRV_CLRC663_REG_T4CounterValLo = UINT8_C(0x27),

	/** Driver mode register */
	DRV_CLRC663_REG_DrvMode = UINT8_C(0x28),

	/** Transmitter amplifier register */
	DRV_CLRC663_REG_TxAmp = UINT8_C(0x29),

	/** Driver configuration register */
	DRV_CLRC663_REG_DrvCon = UINT8_C(0x2A),

	/** Transmitter register */
	DRV_CLRC663_REG_Txl = UINT8_C(0x2B),

	/** Transmitter CRC control register, preset value */
	DRV_CLRC663_REG_TxCrcPreset = UINT8_C(0x2C),

	/** Receiver CRC control register, preset value */
	DRV_CLRC663_REG_RxCrcPreset = UINT8_C(0x2D),

	/** Transmitter data number register */
	DRV_CLRC663_REG_TxDataNum = UINT8_C(0x2E),

	/** Transmitter modulation width register */
	DRV_CLRC663_REG_TxModWidth = UINT8_C(0x2F),

	/** Transmitter symbol 1 + symbol 0 burst length register */
	DRV_CLRC663_REG_TxSym10BurstLen = UINT8_C(0x30),

	/** Transmitter wait control */
	DRV_CLRC663_REG_TXWaitCtrl = UINT8_C(0x31),

	/** Transmitter wait low */
	DRV_CLRC663_REG_TxWaitLo = UINT8_C(0x32),

	/** Transmitter frame control */
	DRV_CLRC663_REG_FrameCon = UINT8_C(0x33),

	/** Receiver start of frame detection */
	DRV_CLRC663_REG_RxSofD = UINT8_C(0x34),

	/** Receiver control register */
	DRV_CLRC663_REG_RxCtrl = UINT8_C(0x35),

	/** Receiver wait register */
	DRV_CLRC663_REG_RxWait = UINT8_C(0x36),

	/** Receiver threshold register */
	DRV_CLRC663_REG_RxThreshold = UINT8_C(0x37),

	/** Receiver register */
	DRV_CLRC663_REG_Rcv = UINT8_C(0x38),

	/** Receiver analog register */
	DRV_CLRC663_REG_RxAna = UINT8_C(0x39),

	/**
	 * Options for LPCD configuration
	 *
	 * @note This register only exists in the CLRC66303 variant, otherwise
	 * it is RFU.
	 */
	DRV_CLRC663_REG_LPCD_Options = UINT8_C(0x3A),

	/** Serial speed register */
	DRV_CLRC663_REG_SerialSpeed = UINT8_C(0x3B),

	/** Low-power oscillator trimming register */
	DRV_CLRC663_REG_LFO_Trimm = UINT8_C(0x3C),

	/**
	 * IntegerN PLL control register, for microcontroller clock output
	 * adjustment
	 */
	DRV_CLRC663_REG_PLL_Ctrl = UINT8_C(0x3D),

	/**
	 * IntegerN PLL control register, for microcontroller clock output
	 * adjustment
	 */
	DRV_CLRC663_REG_PLL_DivOut = UINT8_C(0x3E),

	/** Low-power card detection Q channel minimum threshold */
	DRV_CLRC663_REG_LPCD_QMin = UINT8_C(0x3F),

	/** Low-power card detection Q channel maximum threshold */
	DRV_CLRC663_REG_LPCD_QMax = UINT8_C(0x40),

	/** Low-power card detection I channel minimum threshold */
	DRV_CLRC663_REG_LPCD_IMin = UINT8_C(0x41),

	/** Low-power card detection I channel result register */
	DRV_CLRC663_REG_LPCD_I_Result = UINT8_C(0x42),

	/** Low-power card detection Q channel result register */
	DRV_CLRC663_REG_LPCD_Q_Result = UINT8_C(0x43),

	/** PIN enable register */
	DRV_CLRC663_REG_PadEn = UINT8_C(0x44),

	/** PIN out register */
	DRV_CLRC663_REG_PadOut = UINT8_C(0x45),

	/** PIN in register */
	DRV_CLRC663_REG_PadIn = UINT8_C(0x46),

	/** Enables and controls the SIGOUT Pin */
	DRV_CLRC663_REG_SigOut = UINT8_C(0x47),

	/** Transmitter bit mode register */
	DRV_CLRC663_REG_TxBitMod = UINT8_C(0x48),

	/** Transmitter data configuration register */
	DRV_CLRC663_REG_TxDataCon = UINT8_C(0x4A),

	/** Transmitter data modulation register */
	DRV_CLRC663_REG_TxDataMod = UINT8_C(0x4B),

	/** Transmitter symbol frequency */
	DRV_CLRC663_REG_TxSymFreq = UINT8_C(0x4C),

	/** Transmitter symbol 0 high register */
	DRV_CLRC663_REG_TxSym0H = UINT8_C(0x4D),

	/** Transmitter symbol 1 low register */
	DRV_CLRC663_REG_TxSym0L = UINT8_C(0x4E),

	/** Transmitter symbol 1 high register */
	DRV_CLRC663_REG_TxSym1H = UINT8_C(0x4F),

	/** Transmitter symbol 1 low register */
	DRV_CLRC663_REG_TxSym1L = UINT8_C(0x50),

	/** Transmitter symbol 2 register */
	DRV_CLRC663_REG_TxSym2 = UINT8_C(0x51),

	/** Transmitter symbol 3 register */
	DRV_CLRC663_REG_TxSym3 = UINT8_C(0x52),

	/** Transmitter symbol 1 + symbol 0 length register */
	DRV_CLRC663_REG_TxSym10Len = UINT8_C(0x53),

	/** Transmitter symbol 3 + symbol 2 length register */
	DRV_CLRC663_REG_TxSym32Len = UINT8_C(0x54),

	/** Transmitter symbol 1 + symbol 0 burst control register */
	DRV_CLRC663_REG_TxSym10BurstCtrl = UINT8_C(0x55),

	/** Transmitter symbol 1 + symbol 0 modulation register */
	DRV_CLRC663_REG_TxSym10Mod = UINT8_C(0x56),

	/** Transmitter symbol 3 + symbol 2 modulation register */
	DRV_CLRC663_REG_TxSym32Mod = UINT8_C(0x57),

	/** Receiver bit modulation register */
	DRV_CLRC663_REG_RxBitMod = UINT8_C(0x58),

	/** Receiver end of frame symbol register */
	DRV_CLRC663_REG_RxEofSym = UINT8_C(0x59),

	/** Receiver synchronisation value high register */
	DRV_CLRC663_REG_RxSyncValH = UINT8_C(0x5A),

	/** Receiver synchronisation value low register */
	DRV_CLRC663_REG_RxSyncValL = UINT8_C(0x5B),

	/** Receiver synchronisation mode register */
	DRV_CLRC663_REG_RxSyncMod = UINT8_C(0x5C),

	/** Receiver modulation register */
	DRV_CLRC663_REG_RxMod = UINT8_C(0x5D),

	/** Receiver correlation register */
	DRV_CLRC663_REG_RxCorr = UINT8_C(0x5E),

	/**
	 * Calibration register of the receiver, calibration performed at
	 * production
	 */
	DRV_CLRC663_REG_FabCal = UINT8_C(0x5F),

	/** Version and subversion register */
	DRV_CLRC663_REG_Version = UINT8_C(0x7F)
};

void drv_clrc663_fifo_flush(void);
void drv_clrc663_fifo_mode_set(enum drv_clrc663_fifo_mode fifo_mode);
void drv_clrc663_fifo_write(const uint8_t *src, size_t size);
void drv_clrc663_fifo_read(uint8_t *dst, size_t size);
size_t drv_clrc663_fifo_size(void);

static inline unsigned int drv_clrc663_field_get(const uint8_t val,
						 const unsigned int mask,
						 const unsigned int shift)
{
	return (val & mask) >> shift;
}

static inline uint8_t drv_clrc663_field_set(const uint8_t dst,
					    const unsigned int mask,
					    const unsigned int shift,
					    const unsigned int val)
{
	return (dst & ~mask) | ((val << shift) & mask);
}

extern uint8_t drv_clrc663_reg_read(enum drv_clrc663_reg reg);
extern void drv_clrc663_reg_write(enum drv_clrc663_reg reg, uint8_t val);

#endif // DRV_CLRC663_H