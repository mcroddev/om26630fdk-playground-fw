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

#include "hal/gpio.h"
#include "gpio.h"

// clang-format off

#define PIN_USB_P	(&GPIO_PIN_P_0_29)
#define PIN_USB_N	(&GPIO_PIN_P_0_30)
#define PIN_USB_CON	(&GPIO_PIN_P_2_9)

// clang-format on

static void pin_init_usb_p(void)
{
	const struct gpio_pin_cfg cfg = {
		// clang-format off

		.resistor	= PINMODE_RESISTOR_NONE,
		.pin_func	= PINSEL_FUNC_P_0_29_USB_D_POS,

		// clang-format on
	};
	gpio_pin_init(PIN_USB_P, &cfg);
}

static void pin_init_usb_n(void)
{
	const struct gpio_pin_cfg cfg = {
		// clang-format off

		.resistor	= PINMODE_RESISTOR_NONE,
		.pin_func	= PINSEL_FUNC_P_0_30_USB_D_NEG

		// clang-format on
	};
	gpio_pin_init(PIN_USB_N, &cfg);
}

static void pin_init_usb_con(void)
{
	const struct gpio_pin_cfg cfg = {
		// clang-format off

		.resistor	= PINMODE_RESISTOR_NONE,
		.pin_func	= PINSEL_FUNC_P_2_9_USB_CON

		// clang-format on
	};
	gpio_pin_init(PIN_USB_CON, &cfg);
}

void ccc_gpio_init(void)
{
	pin_init_usb_p();
	pin_init_usb_n();
	pin_init_usb_con();
}