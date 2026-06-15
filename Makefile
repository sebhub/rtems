# SPDX-License-Identifier: BSD-2-Clause

# Copyright (C) 2026 embedded brains GmbH & Co. KG

RTEMS_VERSION ?= 6

TOOLS_ARCH ?= aarch64
TOOLS_PREFIX ?= $(CURDIR)/tools/$(RTEMS_VERSION)

PKG_BSP ?= gr740

WORK_TOOLS ?= $(CURDIR)/work-tools/$(RTEMS_VERSION)
WORK_ARCH ?= aarch64
WORK_BSP ?= zynqmp_apu
WORK_INI ?= work-config.ini
WORK_RTEMS ?= work-rtems
WORK_BUILD ?= work-build
WORK_TOOLS_MARKER ?= $(WORK_TOOLS)/bin/$(WORK_ARCH)-rtems$(RTEMS_VERSION)-gcc
WORK_MAKEFILE ?= Makefile.work

define WORK_CONFIG_INI =
[$(WORK_ARCH)/$(WORK_BSP)]
OPTIMIZATION_FLAGS = -O0 -g -fdata-sections -ffunction-sections
endef

GIT_OPTIONS ?= --do-not-use-git

VENV ?= .venv

VENV_MARKER = $(VENV)/venv-marker

.ONESHELL:

all: work-tools work-rtems $(WORK_MAKEFILE) work

pkg: | prepare
	. $(VENV)/bin/activate
	specbuild $(GIT_OPTIONS) spec config-bsps/spec config-bsps/$(TOOLS_ARCH)/$(PKG_BSP)

pkg-clean:
	if test -d workspace/.git ; then cd workspace && git clean -xdf . && git checkout -- . ; fi

bsps: | prepare
	. $(VENV)/bin/activate
	echo '[DEFAULT]' >config.ini
	echo 'OPTIMIZATION_FLAGS = -O2' >>config.ini
	./waf bsplist "--rtems-bsps=$(TOOLS_ARCH)/.*" | sed 's,\(.*\),[\1],' >>config.ini
	./waf configure "--rtems-tools=$(TOOLS_PREFIX)"
	./waf
	./waf install
.PHONY: bsps

tools: | prepare
	mkdir -p src
	. $(VENV)/bin/activate
	./build_tools.py --rtems-version=$(RTEMS_VERSION) $(TOOLS_ARCH)

$(WORK_TOOLS_MARKER): | prepare
	mkdir -p src
	uv run ./build_tools.py --rtems-version=$(RTEMS_VERSION) --tools-directory=work-tools $(WORK_ARCH)
	$@ --version

work-tools: $(WORK_TOOLS_MARKER)

work-rtems: $(WORK_INI) $(WORK_TOOLS_MARKER) | prepare
	uv run ./waf configure "--rtems-tools=$(WORK_TOOLS)" "--prefix=$(WORK_RTEMS)" "--out=$(WORK_BUILD)" "--rtems-config=$(WORK_INI)"
	uv run ./waf
	uv run ./waf install

$(WORK_MAKEFILE): src/work-template/$(WORK_MAKEFILE)
	mkdir -p work
	cp -r src/work-template/* work
	mv work/$(WORK_MAKEFILE) $(WORK_MAKEFILE)

work: $(WORK_MAKEFILE)
	$(MAKE) -f $<
.PHONY: work

view: $(WORK_MAKEFILE)
	$(MAKE) -f $< view

run: $(WORK_MAKEFILE)
	$(MAKE) -f $< run

qemu: $(WORK_MAKEFILE)
	$(MAKE) -f $< qemu

gdb: $(WORK_MAKEFILE)
	$(MAKE) -f $< gdb

coverage: $(WORK_MAKEFILE)
	$(MAKE) -f $< coverage

clean:
	if test -f $(WORK_MAKEFILE); then $(MAKE) -f $(WORK_MAKEFILE) clean; fi

$(WORK_INI):
	echo "$(WORK_CONFIG_INI)" >$@

work-clean:
	rm -rf $(WORK_INI) $(WORK_RTEMS) $(WORK_BUILD)
.PHONY: work-clean

distclean: work-clean
	rm -rf config-cache config-tools tools $(WORK_TOOLS)
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
