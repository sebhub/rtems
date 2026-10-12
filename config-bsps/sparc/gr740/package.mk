# SPDX-License-Identifier: BSD-2-Clause

# Copyright (C) 2026 embedded brains GmbH & Co. KG

# The link hub needs the specification of a branch with a specitems.yml.  The
# package build and the link hub check load the same items.
LINKHUB_SPEC = $(if $(wildcard specitems.yml),config-bsps/linkhub)

pkg:
	. $(VENV)/bin/activate && specbuild $(GIT_OPTIONS) spec config-bsps/spec \
	  $(LINKHUB_SPEC) config-bsps/$(PKG_ARCH)/$(PKG_BSP)

pkg-update:
	. $(VENV)/bin/activate && specupdatetimeouts --spec-directory config-bsps/spec \
	  workspace/doc/sparc/gr740/test-logs/*-simulator.json
	. $(VENV)/bin/activate && specupdateperf --spec-directory config-bsps/spec \
	  --spec-directory spec --lazy /target/sparc/gr740/sis/perf-smp \
	  workspace/doc/sparc/gr740/test-logs/*-simulator.json

specview:
	. $(VENV)/bin/activate && specwareview --validated=no \
	  --enabled=sparc/gr740,sparc,bsps/sparc/leon3,target/simulator,RTEMS_QUAL,RTEMS_SMP

linkhub:
	. $(VENV)/bin/activate && specbuild $(GIT_OPTIONS) \
	  --only '/pkg/sparc/gr740/*/link-hub' \
	  spec config-bsps/spec config-bsps/linkhub config-bsps/$(PKG_ARCH)/$(PKG_BSP)
