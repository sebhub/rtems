/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * Copyright (C) 2013, 2018 embedded brains GmbH & Co. KG
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#define ARM_CP15_TEXT_SECTION BSP_START_TEXT_SECTION

#include <bsp.h>
#include <bsp/bootcard.h>
#include <bsp/fdt.h>
#include <bsp/linker-symbols.h>
#include <bsp/start.h>
#include <bsp/arm-cp15-start.h>
#include <bsp/arm-a9mpcore-start.h>

#include <rtems/sysinit.h>

#include <libfdt.h>

BSP_START_DATA_SECTION static arm_cp15_start_section_config
imx_mmu_config_table[] = {
  ARMV7_CP15_START_DEFAULT_SECTIONS,
  {
    .begin = 0x00a00000U,
    .end = 0x70000000U,
    .flags = ARMV7_MMU_DEVICE
  }
};

BSP_START_DATA_SECTION static char memory_path[] = "/memory";

BSP_START_TEXT_SECTION static void setup_mmu_and_cache(void)
{
  const void *fdt;
  int node;
  uint32_t ctrl;

  fdt = bsp_fdt_get();
  node = fdt_path_offset_namelen(
    fdt,
    memory_path,
    (int) sizeof(memory_path) - 1
  );

  if (node >= 0) {
    int len;
    const void *val;

    val = fdt_getprop(fdt, node, "reg", &len);
    if (len == 8) {
      uint32_t begin;
      uint32_t size;

      begin = fdt32_to_cpu(((fdt32_t *) val)[0]);
      size = fdt32_to_cpu(((fdt32_t *) val)[1]);

      /* The heap code does not like an end address of zero */
      if (begin + size == 0) {
        size -= 4;
      }

      imx_mmu_config_table[ARMV7_CP15_START_WORKSPACE_ENTRY_INDEX].end =
        begin + size;
    }
  }

  ctrl = arm_cp15_start_setup_mmu_and_cache(
    ARM_CP15_CTRL_A,
    ARM_CP15_CTRL_AFE | ARM_CP15_CTRL_Z
  );

  arm_cp15_start_setup_translation_table_and_enable_mmu_and_cache(
    ctrl,
    (uint32_t *) bsp_translation_table_base,
    ARM_MMU_DEFAULT_CLIENT_DOMAIN,
    &imx_mmu_config_table[0],
    RTEMS_ARRAY_SIZE(imx_mmu_config_table)
  );
}

BSP_START_TEXT_SECTION void bsp_start_hook_0(void)
{
#ifdef RTEMS_SMP
  uint32_t cpu_id = arm_cortex_a9_get_multiprocessor_cpu_id();

  arm_a9mpcore_start_enable_smp_in_auxiliary_control();

  if (cpu_id != 0) {
    arm_a9mpcore_start_on_secondary_processor();
  }
#endif
}

BSP_START_TEXT_SECTION void bsp_start_hook_1(void)
{
  bsp_start_copy_sections();
  setup_mmu_and_cache();
  bsp_start_clear_bss();
}

static Memory_Area _Memory_Areas[1];

static void bsp_memory_initialize(void)
{
  const arm_cp15_start_section_config *section;

  section = &imx_mmu_config_table[ARMV7_CP15_START_WORKSPACE_ENTRY_INDEX];
  _Memory_Initialize(
    &_Memory_Areas[0],
    (void *) section->begin,
    (void *) section->end
  );
}

RTEMS_SYSINIT_ITEM(
  bsp_memory_initialize,
  RTEMS_SYSINIT_MEMORY,
  RTEMS_SYSINIT_ORDER_MIDDLE
);

static const Memory_Information _Memory_Information =
  MEMORY_INFORMATION_INITIALIZER(_Memory_Areas);

const Memory_Information *_Memory_Get(void)
{
  return &_Memory_Information;
}
