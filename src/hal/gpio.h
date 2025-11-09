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

#include "common/compiler.h"

#include "pincm.h"
#include "util.h"

enum gpio_pin_dir {
	GPIO_PIN_DIR_AUTO,
	GPIO_PIN_DIR_OUTPUT,
	GPIO_PIN_DIR_INPUT
};

enum gpio_pin_initial_state {
	GPIO_PIN_INITIAL_STATE_AUTO,
	GPIO_PIN_INITIAL_STATE_LOW,
	GPIO_PIN_INITIAL_STATE_HIGH
};

enum gpio_reg_fiomask {
	GPIO_REG_FIO0MASK = 0x2009C010,
	GPIO_REG_FIO1MASK = 0x2009C030,
	GPIO_REG_FIO2MASK = 0x2009C050,
	GPIO_REG_FIO3MASK = 0x2009C070,
	GPIO_REG_FIO4MASK = 0x2009C090
};

enum gpio_reg_fiodir {
	GPIO_REG_FIO0DIR = 0x2009C000,
	GPIO_REG_FIO1DIR = 0x2009C020,
	GPIO_REG_FIO2DIR = 0x2009C040,
	GPIO_REG_FIO3DIR = 0x2009C060,
	GPIO_REG_FIO4DIR = 0x2009C080
};

enum gpio_reg_fioset {
	GPIO_REG_FIO0SET = 0x2009C018,
	GPIO_REG_FIO1SET = 0x2009C038,
	GPIO_REG_FIO2SET = 0x2009C058,
	GPIO_REG_FIO3SET = 0x2009C078,
	GPIO_REG_FIO4SET = 0x2009C098
};

enum gpio_reg_fioclr {
	GPIO_REG_FIO0CLR = 0x2009C01C,
	GPIO_REG_FIO1CLR = 0x2009C03C,
	GPIO_REG_FIO2CLR = 0x2009C05C,
	GPIO_REG_FIO3CLR = 0x2009C07C,
	GPIO_REG_FIO4CLR = 0x2009C09C
};

struct gpio_pin {
	const enum pinsel_reg pinsel_reg;
	const enum pinmode_reg pinmode_reg;
	const enum gpio_reg_fiodir fiodir_reg;
	const enum gpio_reg_fioset fioset_reg;
	const enum gpio_reg_fioclr fioclr_reg;
	const u32 fio_bit;
	const u32 pinsel_pinmode_mask;
};

struct gpio_pin_cfg {
	enum pinmode_resistor resistor;
	enum pinsel_func pin_func;
	enum gpio_pin_dir dir;
	enum gpio_pin_initial_state initial_state;
};

void gpio_init(void);

void gpio_pin_init(const struct gpio_pin *pin, const struct gpio_pin_cfg *cfg);

ALWAYS_INLINE void gpio_pin_set_low(const struct gpio_pin *const pin)
{
	mmio_write32(pin->fioclr_reg, pin->fio_bit);
}

ALWAYS_INLINE void gpio_pin_set_high(const struct gpio_pin *const pin)
{
	mmio_write32(pin->fioset_reg, pin->fio_bit);
}

ALWAYS_INLINE void gpio_pin_toggle(const struct gpio_pin *const pin)
{
	u32 FIOSET = mmio_read32(pin->fioset_reg);

	if (FIOSET & pin->fio_bit) {
		u32 FIOCLR = mmio_read32(pin->fioclr_reg);
		FIOCLR |= pin->fio_bit;
		mmio_write32(pin->fioclr_reg, FIOCLR);
	} else {
		FIOSET |= pin->fio_bit;
		mmio_write32(pin->fioset_reg, FIOSET);
	}
}

extern const struct gpio_pin GPIO_PIN_P_0_15;
extern const struct gpio_pin GPIO_PIN_P_0_16;
extern const struct gpio_pin GPIO_PIN_P_0_17;
extern const struct gpio_pin GPIO_PIN_P_0_18;
extern const struct gpio_pin GPIO_PIN_P_0_29;
extern const struct gpio_pin GPIO_PIN_P_0_30;
extern const struct gpio_pin GPIO_PIN_P_1_27;
extern const struct gpio_pin GPIO_PIN_P_2_5;
extern const struct gpio_pin GPIO_PIN_P_2_9;