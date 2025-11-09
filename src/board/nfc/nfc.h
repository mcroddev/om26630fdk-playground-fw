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

enum nfc_protocol {
	NFC_PROTOCOL_MIFARE_106,
	NFC_PROTOCOL_MIFARE_212,
	NFC_PROTOCOL_MIFARE_424,
	NFC_PROTOCOL_MIFARE_848,
	NFC_PROTOCOL_NUM
};

void nfc_init(void);

void nfc_enable(void);
void nfc_disable(void);

void nfc_protocol_set(enum nfc_protocol protocol);

void nfc_rf_field_enable(void);
void nfc_rf_field_disable(void);

u8 nfc_read_reg(u8 reg);

u8 nfc_get_device_version(void);