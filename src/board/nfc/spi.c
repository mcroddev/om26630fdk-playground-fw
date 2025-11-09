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

#include "spi.h"

void nfc_spi_init(void)
{
	const struct spi_cfg_moto_master cfg = {
		// clang-format off

		.clk_speed		= SYSCTL_PCLKSEL_CCLK_DIV_1,
		.data_size		= SPI_DATA_SIZE_8BIT,
		.cpol			= SPI_CFG_MOTO_SPI_CPOL_LOW,
		.cpha			= SPI_CFG_MOTO_SPI_CPHA_FIRST,
		.prescaler		= 24,
		.serial_clk_rate	= 0

		// clang-format on
	};
	spi_init_moto_master(SPI_INST, &cfg);
}