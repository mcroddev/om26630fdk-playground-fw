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

# Inspired by https://github.com/vpetrigo/arm-cmake-toolchains

set(CMAKE_SYSTEM_NAME Generic-ELF)
set(CMAKE_SYSTEM_PROCESSOR ARM)

find_program(BINUTILS_PATH arm-none-eabi-gcc)

if (NOT BINUTILS_PATH)
	message(FATAL_ERROR
		"Unable to find arm-none-eabi-gcc; please make sure it is in "
		"your PATH.")
endif ()

get_filename_component(
	ARM_TOOLCHAIN_DIR
	${BINUTILS_PATH}
	DIRECTORY
)

# Without this flag, CMake is not able to pass the test compilation.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER arm-none-eabi-gcc CACHE STRING "")
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc CACHE STRING "")
set(CMAKE_AR arm-none-eabi-gcc-ar CACHE STRING "")
set(CMAKE_RANLIB arm-none-eabi-gcc-ranlib CACHE STRING "")

execute_process(
	COMMAND ${CMAKE_C_COMPILER} -print-sysroot
	OUTPUT_VARIABLE ARM_GCC_SYSROOT
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

# These flags are set regardless of the build type.
set(COMPILER_FLAGS
	--crt0=minimal
	-Wall
	-Walloc-zero
	-Walloca
	-Wcast-qual
	-Wdate-time
	-Wdisabled-optimization
	-Wdouble-promotion
	-Wduplicated-branches
	-Wduplicated-cond
	-Wextra
	-Wfloat-equal
	-Wformat-overflow=2
	-Wformat-signedness
	-Wformat-truncation=2
	-Wformat=2
	-Winit-self
	-Winline
	-Winvalid-pch
	-Wjump-misses-init
	-Wl,--gc-sections,-Map=om26630fdk-playground-fw.map,--no-warn-rwx-segments
	-Wlogical-op
	-Wmain
	-Wmissing-declarations
	-Wmissing-include-dirs
	-Wmissing-prototypes
	-Wno-aggressive-loop-optimizations
	-Wnull-dereference
	-Wpacked
	-Wpointer-sign
	-Wredundant-decls
	-Wshadow
	-Wshift-overflow=2
	-Wstrict-prototypes
	-Wswitch-default
	-Wtrampolines
	-Wundef
	-Wunknown-pragmas
	-Wunsafe-loop-optimizations
	-Wunsuffixed-float-constants
	-Wvector-operation-performance
	-Wvla
	-Wwrite-strings
	-fdata-sections
	-ffunction-sections
	-mcpu=cortex-m3
	-specs=picolibc.specs
	-std=gnu17
)

list(JOIN COMPILER_FLAGS " " COMPILER_FLAGS)

# Default C compiler flags
set(CMAKE_C_FLAGS_DEBUG_INIT "${COMPILER_FLAGS} -Og -ggdb3")

set(
	CMAKE_C_FLAGS_DEBUG
	"${CMAKE_C_FLAGS_DEBUG_INIT}"
	CACHE STRING "" FORCE
)

set(CMAKE_C_FLAGS_RELEASE_INIT "${COMPILER_FLAGS} -Os -DNDEBUG")

set(
	CMAKE_C_FLAGS_RELEASE
	"${CMAKE_C_FLAGS_RELEASE_INIT}"
	CACHE STRING "" FORCE
)

set(
	CMAKE_OBJCOPY
	${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}objcopy
	CACHE INTERNAL "objcopy tool"
)

set(
	CMAKE_SIZE_UTIL
	${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}size
	CACHE INTERNAL "size tool"
)

set(CMAKE_SYSROOT ${ARM_GCC_SYSROOT})
set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

macro(toolchain_set_linker_script TARGET LINKER_SCRIPT)
	string(APPEND CMAKE_EXE_LINKER_FLAGS " -T ${LINKER_SCRIPT}")

	get_target_property(_cur_link_deps ${TARGET} LINK_DEPENDS)

	if (_cur_link_deps)
		string(APPEND _cur_link_deps " ${LINKER_SCRIPT}")
	else()
		set(_cur_link_deps ${LINKER_SCRIPT})
	endif()

	set_target_properties(
		${target} PROPERTIES
		LINK_DEPENDS ${_cur_link_deps}
	)
endmacro()

macro(toolchain_print_target_size TARGET)
	add_custom_command(
		TARGET ${TARGET}
		POST_BUILD COMMAND ${CMAKE_SIZE_UTIL} -B $<TARGET_FILE:${TARGET}>
	)
endmacro()