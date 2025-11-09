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

#ifndef DRV_CLRC663_CMD_H
#define DRV_CLRC663_CMD_H

enum drv_clrc663_cmd {
	/** no action, cancels current command execution */
	DRV_CLRC663_CMD_Idle = UINT8_C(0x00),

	/** low-power card detection */
	DRV_CLRC663_CMD_LPCD = UINT8_C(0x01),

	/**
	 * reads a MIFARE Classic key (size of 6 bytes) from FIFO buffer ant
	 * puts it into Key buffer
	 */
	DRV_CLRC663_CMD_LoadKey = UINT8_C(0x02),

	/** performs the MIFARE Classic authentication */
	DRV_CLRC663_CMD_MFAuthent = UINT8_C(0x03),

	/**
	 * performs a query, an Ack and a Req-Rn for ISO/IEC 18000-3 mode 3/ EPC
	 * Class-1 HF
	 */
	DRV_CLRC663_CMD_AckReq = UINT8_C(0x04),

	/** activates the receive circuit */
	DRV_CLRC663_CMD_Receive = UINT8_C(0x05),

	/** transmits data from the FIFO buffer */
	DRV_CLRC663_CMD_Transmit = UINT8_C(0x06),

	/**
	 * transmits data from the FIFO buffer and automatically activates the
	 * receiver after transmission finished
	 */
	DRV_CLRC663_CMD_Transceive = UINT8_C(0x07),

	/**
	 * gets one byte from FIFO buffer and writes it to the internal EEPROM
	 */
	DRV_CLRC663_CMD_WriteE2 = UINT8_C(0x08),

	/**
	 * gets up to 64 bytes (one EEPROM page) from the FIFO buffer and writes
	 * it to the EEPROM
	 */
	DRV_CLRC663_CMD_WriteE2Page = UINT8_C(0x09),

	/** reads data from the EEPROM and copies it into the FIFO buffer */
	DRV_CLRC663_CMD_ReadE2 = UINT8_C(0x0A),

	/**
	 * reads data from the internal EEPROM and initializes the CLRC663
	 * registers. EEPROM address needs to be within EEPROM sector 2
	 */
	DRV_CLRC663_CMD_LoadReg = UINT8_C(0x0C),

	/**
	 * reads data from the internal EEPROM and initializes the CLRC663
	 * registers needed for a Protocol change
	 */
	DRV_CLRC663_CMD_LoadProtocol = UINT8_C(0x0D),

	/** copies a key from the EEPROM into the key buffer */
	DRV_CLRC663_CMD_LoadKeyE2 = UINT8_C(0x0E),

	/** stores a MIFARE Classic key (size of 6 bytes) into the EEPROM */
	DRV_CLRC663_CMD_StoreKeyE2 = UINT8_C(0x0F),

	/**
	 * Copies bytes from the Random Number generator into the FIFO until the
	 * FiFo is full
	 */
	DRV_CLRC663_CMD_ReadRNR = UINT8_C(0x1C),

	/** resets the CLRC663 */
	DRV_CLRC663_CMD_SoftReset = UINT8_C(0x1F)
};

enum drv_clrc663_protocol_rx {
	DRV_CLRC663_PROTOCOL_RX_ISO_IEC_14443A_106_MANCHESTER_SUBC
};

enum drv_clrc663_protocol_tx {
	DRV_CLRC663_PROTOCOL_TX_ISO_IEC_14443A_106_MILLER
};

void drv_clrc663_cmd_Idle(void);
void drv_clrc663_cmd_LPCD(void);
void drv_clrc663_cmd_LoadKey(const uint8_t *key);
void drv_clrc663_cmd_LoadProtocol(enum drv_clrc663_protocol_rx rx,
				  enum drv_clrc663_protocol_tx tx);

#endif // DRV_CLRC663_CMD_H