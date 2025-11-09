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

#include "clrc663.h"
#include "clrc663-cmd.h"

void drv_clrc663_cmd_Idle(void)
{
	drv_clrc663_reg_write(DRV_CLRC663_REG_Command, DRV_CLRC663_CMD_Idle);
}

void drv_clrc663_cmd_LPCD(void)
{
	drv_clrc663_cmd_Idle();
	drv_clrc663_reg_write(DRV_CLRC663_REG_Command, DRV_CLRC663_CMD_LPCD);
}

void drv_clrc663_cmd_LoadKey(const uint8_t *const key)
{
	drv_clrc663_cmd_Idle();
	drv_clrc663_fifo_flush();
	drv_clrc663_fifo_write(key, DRV_CLRC663_MIFARE_CLASSIC_KEY_NUM_BYTES);
	drv_clrc663_reg_write(DRV_CLRC663_REG_Command, DRV_CLRC663_CMD_LoadKey);
}

void drv_clrc663_cmd_LoadProtocol(const enum drv_clrc663_protocol_rx rx,
				  const enum drv_clrc663_protocol_tx tx)
{
	drv_clrc663_cmd_Idle();
	drv_clrc663_fifo_flush();

	const uint8_t tx_buf[] = {
		[0] = rx,
		[1] = tx,
	};

	drv_clrc663_fifo_write(tx_buf, sizeof(tx_buf));

	drv_clrc663_reg_write(DRV_CLRC663_REG_Command,
			      DRV_CLRC663_CMD_LoadProtocol);
}