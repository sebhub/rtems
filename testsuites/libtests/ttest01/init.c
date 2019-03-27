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

#include <t.h>

#include <sys/time.h>
#include <string.h>

#include <rtems.h>
#include <rtems/bspIo.h>

#include "t-self-test.h"

#include <tmacros.h>

const char rtems_test_name[] = "TTEST 1";

#define test_assert(e) (e) ? (void)0 : test_failed(__LINE__, #e)

RTEMS_LINKER_ROSET(t_self_test, const char *);

typedef struct {
	const char *c;
	size_t case_begin_count;
	size_t case_end_count;
} test_context;

static test_context test_instance;

static void
test_failed(int line, const char *e)
{
#if 0
	printk("FAILED:%i:%s\n", line, e);
	rtems_test_exit(1);
#endif
}

static void
test_putchar(int c, void *arg)
{
	test_context *ctx;

	ctx = arg;

	if (c != '\r' && ctx->c != NULL) {
		test_assert(*ctx->c == c);
		++ctx->c;
	}

	rtems_putc((char)c);
}

static void
case_early(const char *name)
{
	test_context *ctx;
	const char **item;
	ssize_t n;

	ctx = &test_instance;
	++ctx->case_begin_count;
	n = strlen(name);

	RTEMS_LINKER_SET_FOREACH(t_self_test, item) {
		const char *to;

		to = *item;

		if (strncmp(name, to, n) == 0 && to[n] == ':') {
			ctx->c = to + n + 1;
			return;
		}
	}

	test_assert(0);
}

static void
case_late(const char *name)
{
	test_context *ctx;

	ctx = &test_instance;
	++ctx->case_end_count;
	test_assert(ctx->c != NULL);
	test_assert(*ctx->c == '\0');
	ctx->c = NULL;
}

static void
test_action(T_event event, const char *name)
{
	(void)name;

	switch (event) {
	case T_EVENT_CASE_EARLY:
		case_early(name);
		break;
	case T_EVENT_CASE_LATE:
		case_late(name);
		break;
	default:
		break;
	};
}

static Atomic_Uint counter = ATOMIC_INITIALIZER_UINT(0);

static T_time
now(void)
{
	T_time t;

	t = _Atomic_Fetch_add_uint(&counter, 1, ATOMIC_ORDER_RELAXED);
	return t * SBT_1MS;
}

static const T_action actions[] = {
	T_report_hash_sha256,
	test_action,
	T_check_file_descriptors,
	T_check_rtems_barriers,
	T_check_rtems_extensions,
	T_check_rtems_message_queues,
	T_check_rtems_partitions,
	T_check_rtems_periods,
	T_check_rtems_regions,
	T_check_rtems_semaphores,
	T_check_rtems_tasks,
	T_check_rtems_timers,
	T_check_posix_keys
};

static const T_config config = {
	.name = "ttest01",
	.putchar = test_putchar,
	.putchar_arg = &test_instance,
	.verbosity = T_VERBOSE,
	.now = now,
	.action_count = T_ARRAY_SIZE(actions),
	.actions = actions
};

static void
Init(rtems_task_argument arg)
{
	test_context *ctx;
	int exit_code;
	size_t case_count;

	(void)arg;
	TEST_BEGIN();
	ctx = &test_instance;
	test_assert(!T_is_runner());
	T_register();
	test_assert(!T_is_runner());
	exit_code = T_main(&config);
	test_assert(exit_code == 1);
	test_assert(!T_is_runner());
	case_count = RTEMS_LINKER_SET_ITEM_COUNT(t_self_test);
	test_assert(ctx->case_begin_count == case_count);
	test_assert(ctx->case_end_count == case_count);
	TEST_END();
	rtems_test_exit(0);
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 4

#define CONFIGURE_MAXIMUM_BARRIERS 1
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES 1
#define CONFIGURE_MAXIMUM_PARTITIONS 1
#define CONFIGURE_MAXIMUM_PERIODS 1
#define CONFIGURE_MAXIMUM_REGIONS 1
#define CONFIGURE_MAXIMUM_SEMAPHORES 1
#define CONFIGURE_MAXIMUM_TASKS 2
#define CONFIGURE_MAXIMUM_TIMERS 1
#define CONFIGURE_MAXIMUM_USER_EXTENSIONS 1

#define CONFIGURE_MAXIMUM_POSIX_KEYS 1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
