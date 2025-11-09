# SPDX-License-Identifier: MIT
#
# Copyright 2025 Michael Rodriguez
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the “Software”), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

set(SRCS
	main.c
	task-ccc.c
	board/board.c
	board/clk.c
	board/ccc/ccc.c
	board/ccc/gpio.c
	board/ccc/usb.c
	board/nfc/clrc663-spi-impl.c
	board/nfc/gpio.c
	board/nfc/nfc.c
	board/nfc/spi.c
	drivers/clrc663/clrc663.c
	drivers/clrc663/clrc663-cmd.c
	drivers/clrc663/clrc663-spi.c
	hal/gpio.c
	hal/pincm.c
	hal/spi.c
	hal/startup.c
	hal/sysctl.c
	hal/usb.c
	${CMAKE_SOURCE_DIR}/third-party/tinyusb/portable/nxp/lpc17_40/dcd_lpc17_40.c
)

set(HDRS
	common/compiler.h
	common/types.h
	common/util.h
	board/board.h
	board/clk.h
	board/ccc/ccc.h
	board/ccc/gpio.h
	board/ccc/tusb_config.h
	board/ccc/usb.h
	board/nfc/gpio.h
	board/nfc/nfc.h
	board/nfc/spi.h
	drivers/clrc663/clrc663.h
	drivers/clrc663/clrc663-cmd.h
	drivers/clrc663/clrc663-spi.h
	hal/gpio.h
	hal/nvic.h
	hal/pincm.h
	hal/spi.h
	hal/sysctl.h
	hal/usb.h
	hal/util.h
	task-ccc.h
)
add_executable(om26630fdk-playground-fw ${SRCS} ${HDRS})

tinyusb_target_add(om26630fdk-playground-fw)

target_compile_definitions(
	om26630fdk-playground-fw
	PRIVATE
	-DCFG_TUSB_MCU=OPT_MCU_LPC175X_6X
	-DCFG_TUSB_CONFIG_FILE="${CMAKE_SOURCE_DIR}/src/board/ccc/tusb_config.h"
)

target_include_directories(
	om26630fdk-playground-fw
	PRIVATE
	.
	${CMAKE_SOURCE_DIR}/src/board/ccc/tusb-compat
)

toolchain_set_linker_script(
	om26630fdk-playground-fw
	${CMAKE_SOURCE_DIR}/linker-script.ld
)

toolchain_print_target_size(om26630fdk-playground-fw)