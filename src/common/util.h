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

#include <stdint.h>

enum {
	BIT_0 = 1 << 0,
	BIT_1 = 1 << 1,
	BIT_2 = 1 << 2,
	BIT_3 = 1 << 3,
	BIT_4 = 1 << 4,
	BIT_5 = 1 << 5,
	BIT_6 = 1 << 6,
	BIT_7 = 1 << 7,
	BIT_8 = 1 << 8,
	BIT_9 = 1 << 9,
	BIT_10 = 1 << 10,
	BIT_11 = 1 << 11,
	BIT_12 = 1 << 12,
	BIT_13 = 1 << 13,
	BIT_14 = 1 << 14,
	BIT_15 = 1 << 15,
	BIT_16 = 1 << 16,
	BIT_17 = 1 << 17,
	BIT_18 = 1 << 18,
	BIT_19 = 1 << 19,
	BIT_20 = 1 << 20,
	BIT_21 = 1 << 21,
	BIT_22 = 1 << 22,
	BIT_23 = 1 << 23,
	BIT_24 = 1 << 24,
	BIT_25 = 1 << 25,
	BIT_26 = 1 << 26,
	BIT_27 = 1 << 27,
	BIT_28 = 1 << 28,
	BIT_29 = 1 << 29,
	BIT_30 = 1 << 30,
	BIT_31 = UINT32_C(1) << 31
};

#define set_val_by_mask(dst, mask, val)                                  \
	({                                                               \
		const int shift = __builtin_ffs((mask)) - 1;             \
		(dst) = ((dst) & ~(mask)) | (((val) << shift) & (mask)); \
	})

#define get_val_by_mask(src, mask)                           \
	({                                                   \
		const int shift = __builtin_ffs((mask)) - 1; \
		((src) & (mask)) >> shift;                   \
	})

#define BITMASK_FROM_RANGE(start, end) \
	(((1 << (((end) - (start)) + 1)) - 1) << (start))

#define mhz_to_hz(mhz) ((mhz) * (1000000))
#define khz_to_hz(khz) ((khz) * (1000))

#ifdef NDEBUG
#define app_assert(expr)
#else
#define app_assert(expr)             \
	({                           \
		if (!(expr)) {       \
			bkpt();      \
			UNREACHABLE; \
		}                    \
	})
#endif // NDEBUG

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))