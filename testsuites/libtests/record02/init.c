/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 2018, 2019 embedded brains GmbH
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/record.h>
#include <rtems/recordclient.h>
#include <rtems.h>

#include <string.h>

#include "tmacros.h"

const char rtems_test_name[] = "RECORD 2";

typedef struct {
  rtems_record_client_context client;
} test_context;

static test_context test_instance;

static rtems_record_client_status client_handler(
  uint32_t            seconds,
  uint32_t            nanoseconds,
  uint32_t            cpu,
  rtems_record_event  event,
  uint64_t            data,
  void               *arg
)
{
  (void) arg;

  if ( seconds != 0 && nanoseconds != 0 ) {
    printf( "%" PRIu32 ".%09" PRIu32 ":", seconds, nanoseconds );
  } else {
    printf( "*:" );
  }

  printf(
    "%" PRIu32 ":%s:%" PRIx64 "\n",
    cpu,
    rtems_record_event_text( event ),
    data
  );

  return RTEMS_RECORD_CLIENT_SUCCESS;
}

static void drain_visitor(
  const rtems_record_item *items,
  size_t                   count,
  void                    *arg
)
{
  test_context *ctx;
  rtems_record_client_status cs;

  ctx = arg;
  cs = rtems_record_client_run(&ctx->client, items, count * sizeof(*items));
  rtems_test_assert(cs == RTEMS_RECORD_CLIENT_SUCCESS);
}

static void Init(rtems_task_argument arg)
{
  test_context *ctx;
  Record_Stream_header header;
  rtems_record_client_status cs;
  int i;

  TEST_BEGIN();
  ctx = &test_instance;

  for (i = 0; i < 10; ++i) {
    rtems_task_wake_after(1);
  }

  rtems_record_client_init(&ctx->client, client_handler, NULL);
  _Record_Stream_header_initialize(&header);
  cs = rtems_record_client_run(&ctx->client, &header, sizeof(header));
  rtems_test_assert(cs == RTEMS_RECORD_CLIENT_SUCCESS);
  rtems_record_drain(drain_visitor, ctx);

  TEST_END();
  rtems_test_exit(0);
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_RECORD_PER_PROCESSOR_ITEMS 128

#define CONFIGURE_RECORD_EXTENSIONS_ENABLED

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
