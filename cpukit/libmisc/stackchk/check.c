/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSImplStackChecker
 *
 * @brief This source file contains the implementation of the stack checker.
 *
 * @note This extension set uses conditional compilation to account for
 *       stack growth direction.
 */

/*
 *  COPYRIGHT (c) 1989-2024 On-Line Applications Research Corporation (OAR).
 *  COPYRIGHT (c) 2024 Mohamed Hassan <muhammad.hamdy.hassan@gmail.com>
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
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <inttypes.h>

#include <string.h>
#include <stdlib.h>

#include <rtems/printer.h>
#include <rtems/stackchk.h>
#include <rtems/stackchkimpl.h>
#include <rtems/sysinit.h>
#include <rtems/score/address.h>
#include <rtems/score/percpu.h>
#include <rtems/score/smp.h>
#include <rtems/score/threadimpl.h>

/*
 *  The pattern used to fill the entire stack.
 */

#define BYTE_PATTERN 0xA5
#define U32_PATTERN  0xA5A5A5A5

/*
 *  Variable to indicate when the stack checker has been initialized.
 */
static bool Stack_check_Initialized;

/*
 *  The "magic pattern" used to mark the end of the stack.
 */
static const uint32_t
  Stack_check_Sanity_pattern[] = CPU_STACK_CHECK_PATTERN_INITIALIZER;

#define SANITY_PATTERN_SIZE_WORDS \
  RTEMS_ARRAY_SIZE( Stack_check_Sanity_pattern )

/*
 * Helper function to report if the actual stack pointer is in range.
 *
 * NOTE: This uses a GCC specific method.
 */
static inline bool Stack_check_Frame_pointer_in_range(
  const Thread_Control *the_thread
)
{
  #if defined( __GNUC__ )
  void                *sp = __builtin_frame_address( 0 );
  const Stack_Control *the_stack = &the_thread->Start.Initial_stack;

  if ( sp < the_stack->area ) {
    return false;
  }
  if ( sp > ( the_stack->area + the_stack->size ) ) {
    return false;
  }
  #else
    #error "How do I check stack bounds on a non-GNU compiler?"
  #endif
  return true;
}

/*
 *  Where the pattern goes in the stack area is dependent upon
 *  whether the stack grow to the high or low area of the memory.
 */
#if ( CPU_STACK_GROWS_UP == TRUE )
  #define Stack_check_Calculate_used( _low, _size, _high_water ) \
  ( (char *) ( _high_water ) - (char *) ( _low ) )

  #define Stack_check_Usable_stack_start( _the_stack ) ( ( _the_stack )->area )

#else
  #define Stack_check_Calculate_used( _low, _size, _high_water ) \
  ( ( (char *) ( _low ) + ( _size ) ) - (char *) ( _high_water ) )

  #define Stack_check_Usable_stack_start( _the_stack ) \
  ( (char *) ( _the_stack )->area + STACK_CHECK_PATTERN_SIZE )

#endif

/*
 *  The assumption is that if the pattern gets overwritten, the task
 *  is too close. This defines the usable stack memory.
 */
#define Stack_check_Usable_stack_size( _the_stack ) \
  ( ( _the_stack )->size - STACK_CHECK_PATTERN_SIZE )

#if defined( RTEMS_SMP )
static Stack_Control Stack_check_Interrupt_stack[ CPU_MAXIMUM_PROCESSORS ];
#else
static Stack_Control Stack_check_Interrupt_stack[ 1 ];
#endif

/*
 *  Fill an entire stack area with BYTE_PATTERN. This will be used
 *  to check for amount of actual stack used.
 */
static void Stack_check_Dope_stack( Stack_Control *stack )
{
  memset(
    Stack_check_Usable_stack_start( stack ),
    BYTE_PATTERN,
    Stack_check_Usable_stack_size( stack )
  );
}

static void Stack_check_Add_sanity_pattern( Stack_Control *stack )
{
  memcpy(
    _Stack_check_Get_pattern( stack ),
    Stack_check_Sanity_pattern,
    STACK_CHECK_PATTERN_SIZE
  );
}

static bool Stack_check_Is_sanity_pattern_valid( const Stack_Control *stack )
{
  return memcmp(
           _Stack_check_Get_pattern( stack ),
           Stack_check_Sanity_pattern,
           STACK_CHECK_PATTERN_SIZE
         ) == 0;
}

/*
 *  rtems_stack_checker_create_extension
 */
bool rtems_stack_checker_create_extension(
  Thread_Control *running RTEMS_UNUSED,
  Thread_Control         *the_thread
)
{
  Stack_check_Initialized = true;

  Stack_check_Dope_stack( &the_thread->Start.Initial_stack );
  Stack_check_Add_sanity_pattern( &the_thread->Start.Initial_stack );

  return true;
}

void rtems_stack_checker_begin_extension( Thread_Control *executing )
{
  (void) executing;

  Per_CPU_Control *cpu_self;
  uint32_t         cpu_self_index;
  Stack_Control   *stack;

  /*
   * If appropriate, set up the interrupt stack of the current processor for
   * high water testing also. This must be done after multi-threading started,
   * since the initialization stacks may reuse the interrupt stacks. Disable
   * thread dispatching in SMP configurations to prevent thread migration.
   * Writing to the interrupt stack is only safe if done from the corresponding
   * processor in thread context.
   */

#if defined( RTEMS_SMP )
  cpu_self = _Thread_Dispatch_disable();
#else
  cpu_self = _Per_CPU_Get();
#endif

  cpu_self_index = _Per_CPU_Get_index( cpu_self );
  stack = &Stack_check_Interrupt_stack[ cpu_self_index ];

  if ( stack->area == NULL ) {
    stack->area = cpu_self->interrupt_stack_low;
    stack->size = (size_t) ( (char *) cpu_self->interrupt_stack_high -
                             (char *) cpu_self->interrupt_stack_low );

    /*
     * Sanity pattern has been added by Stack_check_Prepare_interrupt_stack()
     */
    if ( !Stack_check_Is_sanity_pattern_valid( stack ) ) {
      rtems_fatal(
        RTEMS_FATAL_SOURCE_STACK_CHECKER,
        rtems_build_name( 'I', 'N', 'T', 'R' )
      );
    }

    Stack_check_Dope_stack( stack );
  }

#if defined( RTEMS_SMP )
  _Thread_Dispatch_enable( cpu_self );
#endif
}

void rtems_stack_checker_reporter_quiet(
  const Thread_Control *running,
  bool                  pattern_ok
)
{
  (void) pattern_ok;

  rtems_fatal(
    RTEMS_FATAL_SOURCE_STACK_CHECKER,
    running->Object.name.name_u32
  );
}

/*
 *  rtems_stack_checker_switch_extension
 */
void rtems_stack_checker_switch_extension(
  Thread_Control *running,
  Thread_Control *heir
)
{
  (void) heir;

  bool                 sp_ok;
  bool                 pattern_ok;
  const Stack_Control *stack;

  /*
   *  Check for an out of bounds stack pointer or an overwrite
   */
#if defined( RTEMS_SMP )
  sp_ok = Stack_check_Frame_pointer_in_range( heir );

  if ( !sp_ok ) {
    pattern_ok = Stack_check_Is_sanity_pattern_valid(
      &heir->Start.Initial_stack
    );
    Stack_checker_Reporter( heir, pattern_ok );
  }

  pattern_ok = Stack_check_Is_sanity_pattern_valid(
    &running->Start.Initial_stack
  );

  if ( !pattern_ok ) {
    Stack_checker_Reporter( running, pattern_ok );
  }
#else
  sp_ok = Stack_check_Frame_pointer_in_range( running );

  pattern_ok = Stack_check_Is_sanity_pattern_valid(
    &running->Start.Initial_stack
  );

  if ( !sp_ok || !pattern_ok ) {
    Stack_checker_Reporter( running, pattern_ok );
  }
#endif

  stack = &Stack_check_Interrupt_stack[ _SMP_Get_current_processor() ];

  if ( stack->area != NULL && !Stack_check_Is_sanity_pattern_valid( stack ) ) {
    rtems_fatal(
      RTEMS_FATAL_SOURCE_STACK_CHECKER,
      rtems_build_name( 'I', 'N', 'T', 'R' )
    );
  }
}

/*
 *  Check if blown
 */
bool rtems_stack_checker_is_blown( void )
{
  Thread_Control *executing;

  executing = _Thread_Get_executing();
  rtems_stack_checker_switch_extension( executing, executing );

  /*
   * The Stack Pointer and the Pattern Area are OK so return false.
   */
  return false;
}

/*
 * Stack_check_find_high_water_mark
 */
static inline void *Stack_check_Find_high_water_mark( const void *s, size_t n )
{
  const uint32_t *base, *ebase;
  uint32_t        length;

  base = s;
  length = n / 4;

  #if ( CPU_STACK_GROWS_UP == TRUE )
  /*
   * start at higher memory and find first word that does not
   * match pattern
   */

  base += length - 1;
  for ( ebase = s; base > ebase; base-- ) {
    if ( *base != U32_PATTERN ) {
      return (void *) base;
    }
  }
  #else
  /*
   * start at lower memory and find first word that does not
   * match pattern
   */

  for ( ebase = base + length; base < ebase; base++ ) {
    if ( *base != U32_PATTERN ) {
      return (void *) base;
    }
  }
  #endif

  return NULL;
}

static void Stack_check_Visit_stack(
  const Stack_Control        *stack,
  const void                 *current,
  const char                 *name,
  rtems_id                    id,
  rtems_stack_checker_visitor visit,
  void                       *arg
)
{
  rtems_stack_checker_info info;

  /* This is likely to occur if the stack checker is not actually enabled */
  if ( stack->area == NULL ) {
    return;
  }

  info.id = id;
  info.name = name;
  info.current = current;
  info.begin = Stack_check_Usable_stack_start( stack );
  info.size = Stack_check_Usable_stack_size( stack );

  if ( Stack_check_Initialized ) {
    void *high_water_mark;

    high_water_mark = Stack_check_Find_high_water_mark(
      info.begin,
      info.size
    );

    if ( high_water_mark != NULL ) {
      info.used = Stack_check_Calculate_used(
        info.begin,
        info.size,
        high_water_mark
      );
    } else {
      info.used = 0;
    }
  } else {
    info.used = UINTPTR_MAX;
  }

  ( *visit )( &info, arg );
}

typedef struct {
  rtems_stack_checker_visitor visit;
  void                       *arg;
} Stack_check_Visitor;

static bool Stack_check_Visit_thread( Thread_Control *the_thread, void *arg )
{
  Stack_check_Visitor *visitor;
  char                 name[ 22 ];
  uintptr_t            sp = _CPU_Context_Get_SP( &the_thread->Registers );

  visitor = arg;
  _Thread_Get_name( the_thread, name, sizeof( name ) );
  Stack_check_Visit_stack(
    &the_thread->Start.Initial_stack,
    (void *) sp,
    name,
    the_thread->Object.id,
    visitor->visit,
    visitor->arg
  );
  return false;
}

static void Stack_check_Visit_interrupt_stack(
  const Stack_Control        *stack,
  uint32_t                    id,
  rtems_stack_checker_visitor visit,
  void                       *arg
)
{
  Stack_check_Visit_stack( stack, NULL, "Interrupt Stack", id, visit, arg );
}

static void Stack_check_Print_info(
  const rtems_stack_checker_info *info,
  void                           *arg
)
{
  const rtems_printer *printer;

  printer = arg;

  rtems_printf(
    printer,
    "0x%08" PRIx32 " %-21s 0x%08" PRIxPTR " 0x%08" PRIxPTR " 0x%08" PRIxPTR
    " %6" PRIuPTR " ",
    info->id,
    info->name,
    (uintptr_t) info->begin,
    (uintptr_t) info->begin + info->size - 1,
    (uintptr_t) info->current,
    info->size
  );

  if ( info->used != UINTPTR_MAX ) {
    rtems_printf( printer, "%6" PRIuPTR "\n", info->used );
  } else {
    rtems_printf( printer, "N/A\n" );
  }
}

void rtems_stack_checker_report_usage_with_plugin(
  const rtems_printer *printer
)
{
  rtems_printf(
    printer,
    "                             STACK USAGE BY THREAD\n"
    "ID         NAME                  LOW        HIGH       CURRENT     AVAIL   USED\n"
  );

  rtems_stack_checker_iterate(
    Stack_check_Print_info,
    RTEMS_DECONST( rtems_printer *, printer )
  );
}

void rtems_stack_checker_report_usage( void )
{
  rtems_printer printer;
  rtems_print_printer_printk( &printer );
  rtems_stack_checker_report_usage_with_plugin( &printer );
}

void rtems_stack_checker_iterate(
  rtems_stack_checker_visitor visit,
  void                       *arg
)
{
  Stack_check_Visitor visitor;
  uint32_t            cpu_max;
  uint32_t            cpu_index;

  visitor.visit = visit;
  visitor.arg = arg;
  rtems_task_iterate( Stack_check_Visit_thread, &visitor );

  cpu_max = rtems_scheduler_get_processor_maximum();

  for ( cpu_index = 0; cpu_index < cpu_max; ++cpu_index ) {
    Stack_check_Visit_interrupt_stack(
      &Stack_check_Interrupt_stack[ cpu_index ],
      cpu_index,
      visit,
      arg
    );
  }
}

static void Stack_check_Prepare_interrupt_stacks( void )
{
  Stack_Control stack;
  uint32_t      cpu_index;
  uint32_t      cpu_max;

  stack.size = rtems_configuration_get_interrupt_stack_size();
  stack.area = _ISR_Stack_area_begin;
  cpu_max = rtems_configuration_get_maximum_processors();

  for ( cpu_index = 0; cpu_index < cpu_max; ++cpu_index ) {
    Stack_check_Add_sanity_pattern( &stack );
    stack.area = _Addresses_Add_offset( stack.area, stack.size );
  }
}

RTEMS_SYSINIT_ITEM(
  Stack_check_Prepare_interrupt_stacks,
  RTEMS_SYSINIT_ISR_STACK,
  RTEMS_SYSINIT_ORDER_MIDDLE
);
