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

#include "gpio.h"

static void pin_init_lpc_sck(void)
{
	const struct gpio_pin_cfg cfg = {
		// clang-format off

		.resistor	= PINMODE_RESISTOR_NONE,
		.pin_func	= PINSEL_FUNC_P_0_15_SCK0

		// clang-format on
	};
	gpio_pin_init(GPIO_PIN_LPC_SCK, &cfg);
}

static void pin_init_lpc_ssel(void)
{
	const struct gpio_pin_cfg cfg = {
		// clang-format off

		.resistor	= PINMODE_RESISTOR_NONE,
		.pin_func	= PINSEL_FUNC_GPIO,
		.dir		= GPIO_PIN_DIR_OUTPUT,
		.initial_state	= GPIO_PIN_INITIAL_STATE_HIGH

		// clang-format on
	};
	gpio_pin_init(GPIO_PIN_LPC_SSEL, &cfg);
}

static void pin_init_lpc_miso(void)
{
	const struct gpio_pin_cfg cfg = {
		// clang-format off

		.resistor	= PINMODE_RESISTOR_NONE,
		.pin_func	= PINSEL_FUNC_P_0_17_MISO0

		// clang-format on
	};
	gpio_pin_init(GPIO_PIN_LPC_MISO, &cfg);
}

static void pin_init_lpc_mosi(void)
{
	const struct gpio_pin_cfg cfg = {
		// clang-format off

		.resistor	= PINMODE_RESISTOR_NONE,
		.pin_func	= PINSEL_FUNC_P_0_18_MOSI0

		// clang-format on
	};
	gpio_pin_init(GPIO_PIN_LPC_MOSI, &cfg);
}

static void pin_init_clrc_rst(void)
{
	const struct gpio_pin_cfg cfg = {
		// clang-format off

		.resistor	= PINMODE_RESISTOR_NONE,
		.pin_func	= PINSEL_FUNC_GPIO,
		.dir		= GPIO_PIN_DIR_OUTPUT,
		.initial_state	= GPIO_PIN_INITIAL_STATE_HIGH

		// clang-format on
	};
	gpio_pin_init(GPIO_PIN_CLRC_RST, &cfg);
}

void nfc_gpio_init(void)
{
	pin_init_lpc_sck();
	pin_init_lpc_ssel();
	pin_init_lpc_miso();
	pin_init_lpc_mosi();

	pin_init_clrc_rst();
}