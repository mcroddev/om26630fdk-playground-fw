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
#include "clrc663-spi.h"

enum {
	REG_WRITE,
	REG_READ,
	DUMMY_BYTE = UINT8_C(0xDB),
};

uint8_t drv_clrc663_reg_read(const enum drv_clrc663_reg reg)
{
	const uint8_t tx[] = {
		[0] = (reg << 1) | REG_READ,
		[1] = DUMMY_BYTE,
	};

	uint8_t rx[2];

	drv_clrc663_nss_pin_set_low();
	drv_clrc663_spi_tx_rx_blocking(tx, rx, 2);
	drv_clrc663_nss_pin_set_high();

	return rx[1];
}

void drv_clrc663_reg_write(const enum drv_clrc663_reg reg, const uint8_t val)
{
	const uint8_t tx[] = {
		[0] = (reg << 1) | REG_WRITE,
		[1] = val,
	};

	drv_clrc663_nss_pin_set_low();
	drv_clrc663_spi_tx_blocking(tx, 2);
	drv_clrc663_nss_pin_set_high();
}