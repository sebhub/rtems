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
	  workspace/doc/riscv/gr765/smp/test-logs/*-simulator.json \
	  workspace/doc/riscv/gr765/uni/test-logs/*-simulator.json
	. $(VENV)/bin/activate && specupdateperf --spec-directory config-bsps/spec \
	  --spec-directory spec --lazy /target/riscv/gr765/sis/perf-smp \
	  workspace/doc/riscv/gr765/smp/test-logs/*-simulator.json
	. $(VENV)/bin/activate && specupdateperf --spec-directory config-bsps/spec \
	  --spec-directory spec --lazy /target/riscv/gr765/sis/perf-default \
	  workspace/doc/riscv/gr765/uni/test-logs/*-simulator.json

ENABLED_UNI = riscv/gr765,riscv,bsps/riscv/noel,target/simulator,RTEMS_QUAL
ENABLED_SMP = $(ENABLED_UNI),RTEMS_SMP

specview:
	. $(VENV)/bin/activate && specwareview --validated=no \
	  --enabled=$(ENABLED_UNI)
	. $(VENV)/bin/activate && specwareview --validated=no \
	  --enabled=$(ENABLED_SMP)

action-maps:
	. $(VENV)/bin/activate && specwareview --format=commonmark \
	  --filter=action-compact-table --enabled=$(ENABLED_SMP) $(UIDS)

linkhub:
	. $(VENV)/bin/activate && specbuild $(GIT_OPTIONS) \
	  --only '/pkg/riscv/gr765/*/link-hub' \
	  spec config-bsps/spec config-bsps/linkhub config-bsps/$(PKG_ARCH)/$(PKG_BSP)
