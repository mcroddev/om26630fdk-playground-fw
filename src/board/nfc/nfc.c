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

#include "drivers/clrc663/clrc663.h"

#include "gpio.h"
#include "nfc.h"
#include "spi.h"

static struct protocol_pair {
	const enum drv_clrc663_protocol_tx tx;
	const enum drv_clrc663_protocol_rx rx;
} const protocol_tbl[NFC_PROTOCOL_NUM] = {
	// clang-format off

	[NFC_PROTOCOL_MIFARE_106] = {
		.tx	= DRV_CLRC663_PROTOCOL_TX_ISO_IEC_14443A_106_MILLER,
		.rx	= DRV_CLRC663_PROTOCOL_RX_ISO_IEC_14443A_106_MANCHESTER_SUBC
	}

	// clang-format on
};

void nfc_init(void)
{
	nfc_gpio_init();
	nfc_spi_init();

	nfc_enable();
}

void nfc_enable(void)
{
	gpio_pin_set_low(GPIO_PIN_CLRC_RST);
}

void nfc_disable(void)
{
	gpio_pin_set_high(GPIO_PIN_CLRC_RST);
}

void nfc_protocol_set(const enum nfc_protocol protocol)
{
	const struct protocol_pair *const proto = &protocol_tbl[protocol];
	drv_clrc663_cmd_LoadProtocol(proto->rx, proto->tx);
}

void nfc_rf_field_enable(void)
{
	drv_clrc663_reg_write(DRV_CLRC663_REG_DrvMode, 0x8E);
}

void nfc_rf_field_disable(void)
{
	drv_clrc663_reg_write(DRV_CLRC663_REG_DrvMode, 0x00);
}

u8 nfc_get_device_version(void)
{
	return drv_clrc663_reg_read(DRV_CLRC663_REG_Version);
}

u8 nfc_read_reg(const u8 byte)
{
	return drv_clrc663_reg_read(byte);
}
