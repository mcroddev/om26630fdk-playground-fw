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

#include <stddef.h>

#include "drivers/clrc663/clrc663-spi.h"

#include "gpio.h"
#include "spi.h"

void drv_clrc663_nss_pin_set_low(void)
{
	gpio_pin_set_low(GPIO_PIN_LPC_SSEL);
}

void drv_clrc663_nss_pin_set_high(void)
{
	gpio_pin_set_high(GPIO_PIN_LPC_SSEL);
}

void drv_clrc663_spi_tx_blocking(const u8 *const src, const size_t src_size)
{
	spi_tx_blocking_u8(SPI_INST, src, src_size);
}

void drv_clrc663_spi_tx_rx_blocking(const u8 *const src, u8 *const dst,
				    const size_t size)
{
	spi_tx_rx_blocking_u8(SPI_INST, src, dst, size);
}