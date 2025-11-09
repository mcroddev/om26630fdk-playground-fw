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

#include "sysctl.h"

enum spi_instance {
	SPI_INSTANCE_SPI0,
	SPI_INSTANCE_SPI1,
};

enum spi_data_size {
	SPI_DATA_SIZE_4BIT = 3,
	SPI_DATA_SIZE_5BIT = 4,
	SPI_DATA_SIZE_6BIT = 5,
	SPI_DATA_SIZE_7BIT = 6,
	SPI_DATA_SIZE_8BIT = 7,
	SPI_DATA_SIZE_9BIT = 8,
	SPI_DATA_SIZE_10BIT = 9,
	SPI_DATA_SIZE_11BIT = 10,
	SPI_DATA_SIZE_12BIT = 11,
	SPI_DATA_SIZE_13BIT = 12,
	SPI_DATA_SIZE_14BIT = 13,
	SPI_DATA_SIZE_15BIT = 14,
	SPI_DATA_SIZE_16BIT = 15
};

enum spi_cfg_moto_spi_cpol {
	/** SSP controller maintains the bus clock low between frames. */
	SPI_CFG_MOTO_SPI_CPOL_LOW,

	/** SSP controller maintains the bus clock high between frames. */
	SPI_CFG_MOTO_SPI_CPOL_HIGH
};

enum spi_cfg_moto_spi_cpha {
	/**
	 * SSP controller captures serial data on the first clock transition of
	 * the frame, that is, the transition away from the inter-frame state
	 * of the clock line.
	 */
	SPI_CFG_MOTO_SPI_CPHA_FIRST,

	/**
	 * SSP controller captures serial data on the second clock transition of
	 * the frame, that is, the transition back to the inter-frame state of
	 * the clock line.
	 */
	SPI_CFG_MOTO_SPI_CPHA_SECOND
};

struct spi_cfg_moto_master {
	enum sysctl_pclksel_clk clk_speed;
	enum spi_data_size data_size;
	enum spi_cfg_moto_spi_cpol cpol;
	enum spi_cfg_moto_spi_cpha cpha;
	u8 prescaler;
	u8 serial_clk_rate;
};

void spi_init_moto_master(enum spi_instance inst,
			  const struct spi_cfg_moto_master *cfg);

void spi_tx_blocking_u8(enum spi_instance inst, const u8 *src, u32 src_size);

void spi_tx_rx_blocking_u8(enum spi_instance inst, const u8 *src, u8 *dst,
			   u32 size);