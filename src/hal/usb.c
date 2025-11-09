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

#include "usb.h"
#include "sysctl.h"

enum {
	USB_REG_USBClkCtrl_DEV_CLK_EN = BIT_1,
	USB_REG_USBClkCtrl_AHB_CLK_EN = BIT_4,
};

enum {
	USB_REG_USBClkSt_DEV_CLK_ON = BIT_1,
	USB_REG_USBClkSt_AHB_CLK_ON = BIT_4
};

enum usb_reg {
	USB_REG_USBDevIntSt = 0x5000C200,
	USB_REG_USBDevIntEn = 0x5000C204,
	USB_REG_USBDevIntClr = 0x5000C208,
	USB_REG_USBDevIntSet = 0x5000C20C,
	USB_REG_USBDevIntPri = 0x5000C22C,
	USB_REG_USBEpIntSt = 0x5000C230,
	USB_REG_USBEpIntEn = 0x5000C234,
	USB_REG_USBEpIntClr = 0x5000C238,
	USB_REG_USBEpIntSet = 0x5000C23C,
	USB_REG_USBEpIntPri = 0x5000C240,
	USB_REG_USBReEp = 0x5000C244,
	USB_REG_USBEpIn = 0x5000C248,
	USB_REG_USBMaxPSize = 0x5000C24C,
	USB_REG_USBRxData = 0x5000C218,
	USB_REG_USBRxPLen = 0x5000C220,
	USB_REG_USBClkCtrl = 0x5000CFF4,
	USB_REG_USBClkSt = 0x5000CFF8
};

void usb_init(void)
{
	// The LPC176x/5x USB device controller initialization includes the
	// following steps:

	// 1. Enable the device controller by setting the PCUSB bit of PCONP.
	sysctl_peripheral_power_enable(SYSCTL_PCONP_BIT_PCUSB);

	// 2. Configure and enable the PLL and Clock Dividers to provide 48 MHz
	//    for usbclk and the desired frequency for cclk.
	sysctl_usbclk_set(SYSCTL_USBCLK_PLL0_DIV_6);

	// 3. Enable the device controller clocks by setting DEV_CLK_EN and
	//    AHB_CLK_EN bits in the USBClkCtrl register. Poll the respective
	//    clock bits in the USBClkSt register until they are set.
	mmio_set32(USB_REG_USBClkCtrl, USB_REG_USBClkCtrl_DEV_CLK_EN |
					       USB_REG_USBClkCtrl_AHB_CLK_EN);

	while (!(mmio_read32(USB_REG_USBClkSt) &
		 (USB_REG_USBClkSt_DEV_CLK_ON | USB_REG_USBClkSt_AHB_CLK_ON)))
		nop();

	// 4. Enable the USB pin functions by writing to the corresponding
	//    PINSEL register.
	//
	// 5. Disable the pull-ups and pull-downs on the VBUS pin using the
	//    corresponding PINMODE register by putting the pin in the “pin has
	//    neither pull-up nor pull-down resistor enabled” mode.
	//
	// This should've been done by the application.
}