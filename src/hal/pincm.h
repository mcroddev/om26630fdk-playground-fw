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

#include "common/types.h"

enum pinsel_reg {
	PINSEL_REG_PINSEL0 = 0x4002C000,
	PINSEL_REG_PINSEL1 = 0x4002C004,
	PINSEL_REG_PINSEL2 = 0x4002C008,
	PINSEL_REG_PINSEL3 = 0x4002C00C,
	PINSEL_REG_PINSEL4 = 0x4002C010,
	PINSEL_REG_PINSEL7 = 0x4002C01C,
	PINSEL_REG_PINSEL8 = 0x4002C020,
	PINSEL_REG_PINSEL9 = 0x4002C024,
	PINSEL_REG_PINSEL10 = 0x4002C028
};

enum pinmode_reg {
	PINMODE_REG_PINMODE0 = 0x4002C040,
	PINMODE_REG_PINMODE1 = 0x4002C044,
	PINMODE_REG_PINMODE2 = 0x4002C048,
	PINMODE_REG_PINMODE3 = 0x4002C04C,
	PINMODE_REG_PINMODE4 = 0x4002C050
};

enum pinmode_resistor {
	PINMODE_RESISTOR_PULL_UP,
	PINMODE_RESISTOR_REPEATER,
	PINMODE_RESISTOR_NONE,
	PINMODE_RESISTOR_PULL_DOWN
};

enum pinsel_func {
	PINSEL_FUNC_GPIO = 0,

	PINSEL_FUNC_P_0_15_SCK0 = 2,
	PINSEL_FUNC_P_0_16_SSEL0 = 2,
	PINSEL_FUNC_P_0_17_MISO0 = 2,
	PINSEL_FUNC_P_0_18_MOSI0 = 2,

	PINSEL_FUNC_P_0_29_USB_D_POS = 1,
	PINSEL_FUNC_P_0_30_USB_D_NEG = 1,

	PINSEL_FUNC_P_1_27_CLKOUT = 1,

	PINSEL_FUNC_P_2_9_USB_CON = 1
};

void pincm_set_func(enum pinsel_reg reg, u32 pinsel_mask,
		    enum pinsel_func func);

void pincm_set_resistor(enum pinmode_reg reg, u32 pinmode_mask,
			enum pinmode_resistor resistor);