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

#include <tusb.h>

#include "common/compiler.h"
#include "common/types.h"
#include "common/util.h"

#include "usb.h"
#include "hal/usb.h"

enum {
	CONFIG_TOTAL_LEN =
		TUD_CONFIG_DESC_LEN + (CFG_TUD_CDC * TUD_CDC_DESC_LEN)
};

enum {
	STRID_LANGID = 0,
	STRID_MANUFACTURER = 1,
	STRID_PRODUCT = 2,
	STRID_SERIAL = 3,
	STRID_INTERFACE = 4,
	STRID_NUM_MAX
};

enum {
	USB_BCD = 0x0100,
	USB_PID = 0x4001,
	USB_VID = 0xFFFF,
};

enum {
	EPNUM_CDC_0_NOTIF = 0x81,
	EPNUM_CDC_0_OUT = 0x02,
	EPNUM_CDC_0_IN = 0x82
};

enum {
	ITF_NUM_CDC_0,
	ITF_NUM_CDC_0_DATA,
	ITF_NUM_TOTAL,
};

static const tusb_desc_device_t dev = {
	// clang-format off

	.bLength			= sizeof(tusb_desc_device_t),
	.bDescriptorType		= TUSB_DESC_DEVICE,
	.bcdUSB				= 0x0200,
	.bDeviceClass			= TUSB_CLASS_MISC,
	.bDeviceSubClass		= MISC_SUBCLASS_COMMON,
	.bDeviceProtocol		= MISC_PROTOCOL_IAD,
	.bMaxPacketSize0		= CFG_TUD_ENDPOINT0_SIZE,
	.idVendor			= USB_VID,
	.idProduct			= USB_PID,
	.bcdDevice			= USB_BCD,
	.iManufacturer			= STRID_MANUFACTURER,
	.iProduct			= STRID_PRODUCT,
	.iSerialNumber			= STRID_SERIAL,
	.bNumConfigurations		= 0x01

	// clang-format on
};

static const u8 dev_desc_cfg[] = {
	TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),
	TUD_CDC_DESCRIPTOR(ITF_NUM_CDC_0, 4, EPNUM_CDC_0_NOTIF, 16,
			   EPNUM_CDC_0_OUT, EPNUM_CDC_0_IN, 64)
};

// array of pointer to string descriptors
const char *const string_desc_arr[STRID_NUM_MAX] = {
	// clang-format off

	[STRID_LANGID]		= (const char[]){ 0x09, 0x04 },
	[STRID_MANUFACTURER]	= "mcrod",
	[STRID_PRODUCT]		= "OM26630FDK-Playground",
	[STRID_SERIAL]		= "AllYourBaseAreBelongToUs",
	[STRID_INTERFACE]	= "OM26630FDK-Playground CDC"

	// clang-format on
};

void isr_USB(void)
{
	tud_int_handler(ITF_NUM_CDC_0);
}

void ccc_usb_init(void)
{
	usb_init();

	const tusb_rhport_init_t dev_init = {
		// clang-format off

		.role	= TUSB_ROLE_DEVICE,
		.speed	= TUSB_SPEED_AUTO

		// clang-format on
	};
	tusb_init(ITF_NUM_CDC_0, &dev_init);
}

void ccc_usb_tick(void)
{
	tud_task();
}

bool ccc_cdc_read_byte(u8 *const dst)
{
	const int32_t ch = tud_cdc_n_read_char(ITF_NUM_CDC_0);

	if (ch != -1) {
		*dst = ch;
		return true;
	}
	return false;
}

bool ccc_cdc_write_byte(const u8 byte)
{
	if (tud_cdc_n_connected(ITF_NUM_CDC_0)) {
		tud_cdc_n_write_char(ITF_NUM_CDC_0, byte);
		tud_cdc_n_write_flush(ITF_NUM_CDC_0);

		return true;
	}
	return false;
}

u8 const *tud_descriptor_device_cb(void)
{
	return (u8 const *)&dev;
}

u8 const *tud_descriptor_configuration_cb(const u8 index)
{
	(void)index;
	return dev_desc_cfg;
}

u16 const *tud_descriptor_string_cb(const u8 index, const u16 langid)
{
	(void)langid;
	static u16 desc_str[32];

	if (unlikely(index >= ARRAY_SIZE(string_desc_arr)))
		return NULL;

	if (index == STRID_LANGID) {
		memcpy(&desc_str[1], string_desc_arr[STRID_LANGID], 2);
		desc_str[0] = (TUSB_DESC_STRING << 8) | 4;
		return desc_str;
	}

	const char *const ascii_str = string_desc_arr[index];

	u8 len = strlen(ascii_str);
	if (len > sizeof(desc_str) - 1)
		len = sizeof(desc_str) - 1;

	for (size_t i = 0; i < len; ++i)
		desc_str[i + 1] = ascii_str[i];

	desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * len + 2);
	return desc_str;
}