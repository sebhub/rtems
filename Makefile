# SPDX-License-Identifier: BSD-2-Clause

# Copyright (C) 2026 embedded brains GmbH & Co. KG

override RTEMS_VERSION := 6
TOOLS_ARCH ?= sparc
TOOLS_PREFIX ?= $(CURDIR)/tools/$(RTEMS_VERSION)

PKG_BSP ?= gr740

ZYNQMP_ARCH := aarch64
ZYNQMP_BSP := zynqmp_apu
ZYNQMP_INI := config.ini
ZYNQMP_PREFIX := rtems-zynqmp
ZYNQMP_BUILD := build-zynqmp
ZYNQMP_TOOL_MARKER :=$(TOOLS_PREFIX)/bin/$(ZYNQMP_ARCH)-rtems$(RTEMS_VERSION)-gcc

GIT_OPTIONS ?= --do-not-use-git

VENV ?= .venv

VENV_MARKER = $(VENV)/venv-marker

.ONESHELL:

define CONFIG-INI =
[DEFAULT]
RTEMS_SMP = True
RTEMS_PPS_SYNC = False
BUILD_MEMBENCH = True
BUILD_UNITTESTS = True
BUILD_SAMPLES = False
BUILD_VALIDATIONTESTS = True
BSP_PRINT_EXCEPTION_CONTEXT = 0
OPTIMIZATION_FLAGS = -O0 -g -fdata-sections -ffunction-sections -frandom-seed=0
[$(ZYNQMP_ARCH)/$(ZYNQMP_BSP)-extra]
INHERIT = $(ZYNQMP_BSP)
RTEMS_BUILD_LABEL = $(ZYNQMP_ARCH)/$(ZYNQMP_BSP)/extra
RTEMS_QUAL = False
# Note: 'qual-only' can currently not build for ZynqMP
# [$(ZYNQMP_ARCH)/$(ZYNQMP_BSP)-qual-only]
# INHERIT = $(ZYNQMP_BSP)
# RTEMS_BUILD_LABEL = $(ZYNQMP_ARCH)/$(ZYNQMP_BSP)/qual-only
# RTEMS_QUAL = True
[$(ZYNQMP_ARCH)/$(ZYNQMP_BSP)-extra-coverage]
INHERIT = $(ZYNQMP_BSP)
RTEMS_BUILD_LABEL = $(ZYNQMP_ARCH)/$(ZYNQMP_BSP)/extra-coverage
RTEMS_GCOV_COVERAGE = True
endef

all: tools pkg

pkg: | prepare
	. $(VENV)/bin/activate
	specbuild $(GIT_OPTIONS) spec config-bsps/spec config-bsps/$(TOOLS_ARCH)/$(PKG_BSP)

pkg-clean:
	if test -d workspace/.git ; then cd workspace && git clean -xdf . && git checkout -- . ; fi

.PHONY: bsps

bsps: | prepare
	. $(VENV)/bin/activate
	echo '[DEFAULT]' >config.ini
	echo 'OPTIMIZATION_FLAGS = -O2' >>config.ini
	./waf bsplist "--rtems-bsps=$(TOOLS_ARCH)/.*" | sed 's,\(.*\),[\1],' >>config.ini
	./waf configure "--rtems-tools=$(PWD)/tools/6"
	./waf
	./waf install

tools: | prepare
	mkdir -p src
	. $(VENV)/bin/activate
	./build_tools.py $(TOOLS_ARCH)

$(ZYNQMP_TOOL_MARKER): | prepare
	mkdir -p src
	uv run ./build_tools.py $(ZYNQMP_ARCH)
	$@ --version

tools-zynqmp: $(ZYNQMP_TOOL_MARKER)
.PHONY: tools-zynqmp

bsp-zynqmp: $(ZYNQMP_INI) $(ZYNQMP_TOOL_MARKER) | prepare
	uv run ./waf configure "--rtems-tools=$(TOOLS_PREFIX)" "--prefix=$(ZYNQMP_PREFIX)" "--out=$(ZYNQMP_BUILD)"
	uv run ./waf
	uv run ./waf install
.PHONY: bsp-zynqmp

$(ZYNQMP_INI):
	@echo "$(CONFIG-INI)" >$@

clean-zynqmp:
	rm -rf $(ZYNQMP_INI) $(ZYNQMP_PREFIX) $(ZYNQMP_BUILD)
.PHONY: clean-zynqmp

distclean: clean-zynqmp
	rm -rf config-cache config-tools src tools
.PHONY: distclean

prepare: $(VENV_MARKER)

$(VENV_MARKER): uv.lock
	uv sync --all-groups
	touch $@

ifndef CI
uv.lock: pyproject.toml
	uv lock
	touch $@
endif
