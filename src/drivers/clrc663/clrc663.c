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

#include "clrc663.h"

void drv_clrc663_fifo_flush(void)
{
	uint8_t FIFOControl = drv_clrc663_reg_read(DRV_CLRC663_REG_FIFOControl);
	FIFOControl |= DRV_CLRC663_FIFOControl_FIFOFlush;
	drv_clrc663_reg_write(DRV_CLRC663_REG_FIFOControl, FIFOControl);
}

void drv_clrc663_fifo_mode_set(const enum drv_clrc663_fifo_mode fifo_mode)
{
	// It is recommended to change the FIFO size only, when the FIFO content
	// had been cleared.
	drv_clrc663_fifo_flush();

	uint8_t FIFOControl = drv_clrc663_reg_read(DRV_CLRC663_REG_FIFOControl);

	if (fifo_mode == DRV_CLRC663_FIFO_MODE_255)
		FIFOControl |= DRV_CLRC663_FIFOControl_FIFOSize;
	else if (fifo_mode == DRV_CLRC663_FIFO_MODE_512)
		FIFOControl &= ~DRV_CLRC663_FIFOControl_FIFOSize;

	drv_clrc663_reg_write(DRV_CLRC663_REG_FIFOControl, FIFOControl);
}

void drv_clrc663_fifo_write(const uint8_t *const src, const size_t size)
{
	for (size_t i = 0; i < size; ++i)
		drv_clrc663_reg_write(DRV_CLRC663_REG_FIFOData, src[i]);
}

void drv_clrc663_fifo_read(uint8_t *const dst, const size_t size)
{
	for (size_t i = 0; i < size; ++i)
		dst[i] = drv_clrc663_reg_read(DRV_CLRC663_REG_FIFOData);
}

size_t drv_clrc663_fifo_size(void)
{
	const uint8_t FIFOLength =
		drv_clrc663_reg_read(DRV_CLRC663_REG_FIFOLength);
	const uint8_t FIFOControl =
		drv_clrc663_reg_read(DRV_CLRC663_REG_FIFOControl);

	if (FIFOControl & DRV_CLRC663_FIFOControl_FIFOSize)
		return FIFOLength;

	const uint8_t FIFOLengthExtBits = drv_clrc663_field_get(
		FIFOControl, DRV_CLRC663_FIFOControl_MASK_FIFOLengthExtBits,
		DRV_CLRC663_FIFOControl_SHIFT_FIFOLengthExtBits);

	return (FIFOLengthExtBits << 8) | FIFOLength;
}