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

#include "board/ccc/ccc.h"
#include "board/nfc/nfc.h"
#include "common/types.h"
#include "common/util.h"
#include "hal/util.h"
#include "task-ccc.h"

static void cmd_reg_read(void);
static void cmd_reg_write(void);

static void cmd_protocol_set(void);

static void cmd_rf_field_on(void);
static void cmd_rf_field_off(void);

enum ccc_state {
	TASK_STATE_WAITING_FOR_CMD,
	TASK_STATE_WAITING_FOR_PARAMS,
};

enum {
	CMD_NUM_PARAMS_MAX = 30,
};

enum ccc_cmd {
	CMD_REG_READ,
	CMD_REG_WRITE,
	CMD_PROTOCOL_SET,
	CMD_RF_FIELD_ON,
	CMD_RF_FIELD_OFF,
	CMD_NUM_MAX,
};

enum { CMD_UNKNOWN = 0xAA, CMD_ACK = 0xBB, CMD_NAK = 0xFF };

static struct {
	void (*const cmd)(void);
	const u32 num_params;
} ccc_cmd[] = {
	// clang-format off

	[CMD_REG_READ] = {
		.cmd		= cmd_reg_read,
		.num_params	= 1
	},

	[CMD_REG_WRITE] = {
		.cmd		= cmd_reg_write,
		.num_params	= 2
	},

	[CMD_PROTOCOL_SET] = {
		.cmd		= cmd_protocol_set,
		.num_params	= 1
	},

	[CMD_RF_FIELD_ON] = {
		.cmd		= cmd_rf_field_on,
		.num_params	= 0,
	},

	[CMD_RF_FIELD_OFF] = {
		.cmd		= cmd_rf_field_off,
		.num_params	= 0
	}

	// clang-format on
};

static struct {
	enum ccc_state state;

	struct {
		enum ccc_cmd cmd;
		u8 params[CMD_NUM_PARAMS_MAX];
		u32 num_params;
	} curr_cmd;
} ccc_task;

static void cmd_reg_read(void)
{
	const u8 byte = nfc_read_reg(ccc_task.curr_cmd.params[0]);
	ccc_cdc_write_byte(byte);

	ccc_task.state = TASK_STATE_WAITING_FOR_CMD;
}

static void cmd_reg_write(void)
{
	ccc_task.state = TASK_STATE_WAITING_FOR_CMD;
}

static void cmd_protocol_set(void)
{
	nfc_protocol_set(ccc_task.curr_cmd.params[0]);
	ccc_cdc_write_byte(CMD_ACK);

	ccc_task.state = TASK_STATE_WAITING_FOR_CMD;
}

static void cmd_rf_field_on(void)
{
	nfc_rf_field_enable();
	ccc_cdc_write_byte(CMD_ACK);

	ccc_task.state = TASK_STATE_WAITING_FOR_CMD;
}

static void cmd_rf_field_off(void)
{
	nfc_rf_field_disable();
	ccc_cdc_write_byte(CMD_ACK);

	ccc_task.state = TASK_STATE_WAITING_FOR_CMD;
}

static void handle_waiting_for_cmd(const u8 byte)
{
	if (byte >= CMD_NUM_MAX) {
		ccc_cdc_write_byte(CMD_NAK);
		ccc_cdc_write_byte(CMD_UNKNOWN);
		return;
	}

	if (!ccc_cmd[byte].num_params)
		ccc_cmd[byte].cmd();
	else {
		ccc_task.curr_cmd.cmd = byte;
		ccc_task.curr_cmd.num_params = 0;

		ccc_task.state = TASK_STATE_WAITING_FOR_PARAMS;
	}
}

static void handle_waiting_for_params(const u8 byte)
{
	ccc_task.curr_cmd.params[ccc_task.curr_cmd.num_params++] = byte;

	if (ccc_task.curr_cmd.num_params >=
	    ccc_cmd[ccc_task.curr_cmd.cmd].num_params)
		ccc_cmd[ccc_task.curr_cmd.cmd].cmd();
}

static void process_byte(const u8 byte)
{
	switch (ccc_task.state) {
	case TASK_STATE_WAITING_FOR_CMD:
		handle_waiting_for_cmd(byte);
		return;

	case TASK_STATE_WAITING_FOR_PARAMS:
		handle_waiting_for_params(byte);
		return;

	default:
		app_assert(false);
		return;
	}
}

void task_ccc_tick(void)
{
	u8 byte;
	if (ccc_cdc_read_byte(&byte))
		process_byte(byte);
}