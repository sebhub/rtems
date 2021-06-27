/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief This header file defines the Interrupt Manager Extension API.
 */

/*
 * Copyright (C) 2008, 2021 embedded brains GmbH (http://www.embedded-brains.de)
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

/*
 * This file is part of the RTEMS quality process and was automatically
 * generated.  If you find something that needs to be fixed or
 * worded better please post a report or patch to an RTEMS mailing list
 * or raise a bug report:
 *
 * https://www.rtems.org/bugs.html
 *
 * For information on updating and regenerating please refer to the How-To
 * section in the Software Requirements Engineering chapter of the
 * RTEMS Software Engineering manual.  The manual is provided as a part of
 * a release.  For development sources please refer to the online
 * documentation at:
 *
 * https://docs.rtems.org
 */

/* Generated from spec:/rtems/intr/if/header-2 */

#ifndef _RTEMS_IRQ_EXTENSION_H
#define _RTEMS_IRQ_EXTENSION_H

#include <stddef.h>
#include <stdint.h>
#include <sys/cpuset.h>
#include <rtems/rtems/attr.h>
#include <rtems/rtems/intr.h>
#include <rtems/rtems/modes.h>
#include <rtems/rtems/options.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/types.h>
#include <rtems/score/chain.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Generated from spec:/rtems/intr/if/handler */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Interrupt handler routines shall have this type.
 */
typedef void ( *rtems_interrupt_handler )( void * );

/* Generated from spec:/rtems/intr/if/per-handler-routine */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Visitor routines invoked by rtems_interrupt_handler_iterate() shall
 *   have this type.
 */
typedef void ( *rtems_interrupt_per_handler_routine )(
  void *,
  const char *,
  rtems_option,
  rtems_interrupt_handler,
  void *
);

/* Generated from spec:/rtems/intr/if/shared */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief This interrupt handler install option allows that the interrupt
 *   handler may share the interrupt vector with other handler.
 */
#define RTEMS_INTERRUPT_SHARED ( (rtems_option) 0x00000000 )

/* Generated from spec:/rtems/intr/if/unique */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief This interrupt handler install option ensures that the interrupt
 *   handler is unique.
 *
 * This option prevents other handler from using the same interrupt vector.
 */
#define RTEMS_INTERRUPT_UNIQUE ( (rtems_option) 0x00000001 )

/* Generated from spec:/rtems/intr/if/replace */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief This interrupt handler install option requests that the interrupt
 *   handler replaces the first handler with the same argument.
 */
#define RTEMS_INTERRUPT_REPLACE ( (rtems_option) 0x00000002 )

/* Generated from spec:/rtems/intr/if/is-shared */

/**
 * @brief Checks if the interrupt handler shared option is set.
 *
 * @param _options is the interrupt handler option set to check.
 *
 * @return Returns true, if the interrupt handler shared option
 *   #RTEMS_INTERRUPT_SHARED is set, otherwise false.
 */
#define RTEMS_INTERRUPT_IS_SHARED( _options ) \
  ( ( _options ) & RTEMS_INTERRUPT_SHARED )

/* Generated from spec:/rtems/intr/if/is-unique */

/**
 * @brief Checks if the interrupt handler unique option is set.
 *
 * @param _options is the interrupt handler option set to check.
 *
 * @return Returns true, if the interrupt handler unique option
 *   #RTEMS_INTERRUPT_UNIQUE is set, otherwise false.
 */
#define RTEMS_INTERRUPT_IS_UNIQUE( _options ) \
  ( ( _options ) & RTEMS_INTERRUPT_UNIQUE )

/* Generated from spec:/rtems/intr/if/is-replace */

/**
 * @brief Checks if the interrupt handler replace option is set.
 *
 * @param _options is the interrupt handler option set to check.
 *
 * @return Returns true, if the interrupt handler replace option
 *   #RTEMS_INTERRUPT_REPLACE is set, otherwise false.
 */
#define RTEMS_INTERRUPT_IS_REPLACE( _options ) \
  ( ( _options ) & RTEMS_INTERRUPT_REPLACE )

/* Generated from spec:/rtems/intr/if/handler-install */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Installs the interrupt handler routine and argument at the interrupt
 *   vector.
 *
 * @param vector is the interrupt vector number.
 *
 * @param info is the descriptive information of the interrupt handler to
 *   install.
 *
 * @param options is the interrupt handler install option set.
 *
 * @param routine is the interrupt handler routine to install.
 *
 * @param arg is the interrupt handler argument to install.
 *
 * One of the following mutually exclusive options
 *
 * * #RTEMS_INTERRUPT_UNIQUE,
 *
 * * #RTEMS_INTERRUPT_SHARED, and
 *
 * * #RTEMS_INTERRUPT_REPLACE
 *
 * shall be set in the ``options`` parameter.
 *
 * The handler routine will be called with the argument specified by ``arg``
 * when dispatched.  The order in which shared interrupt handlers are
 * dispatched for one vector is defined by the installation order.  The first
 * installed handler is dispatched first.
 *
 * If the option #RTEMS_INTERRUPT_UNIQUE is set, then it will be ensured that
 * the handler will be the only one for the interrupt vector.
 *
 * If the option #RTEMS_INTERRUPT_SHARED is set, then multiple handler may be
 * installed for the interrupt vector.
 *
 * If the option #RTEMS_INTERRUPT_REPLACE is set, then the handler specified by
 * ``routine`` will replace the first handler with the same argument for the
 * interrupt vector if it exists, otherwise an error status will be returned.
 * A second handler with the same argument for the interrupt vector will remain
 * unchanged.  The new handler will inherit the unique or shared options from
 * the replaced handler.
 *
 * An informative description may be provided in ``info``.  It may be used for
 * system debugging and diagnostic tools.  The referenced string has to be
 * persistent as long as the handler is installed.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INCORRECT_STATE The service was not initialized.
 *
 * @retval ::RTEMS_INVALID_ADDRESS The ``routine`` parameter was NULL.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt vector associated with the
 *   number specified by ``vector``.
 *
 * @retval ::RTEMS_CALLED_FROM_ISR The directive was called from within
 *   interrupt context.
 *
 * @retval ::RTEMS_NO_MEMORY There was not enough memory available to allocate
 *   data structures to install the handler.
 *
 * @retval ::RTEMS_RESOURCE_IN_USE The #RTEMS_INTERRUPT_UNIQUE option was set
 *   in ``options`` and the interrupt vector was already occupied by a handler.
 *
 * @retval ::RTEMS_RESOURCE_IN_USE The #RTEMS_INTERRUPT_SHARED option was set
 *   in ``options`` and the interrupt vector was already occupied by a unique
 *   handler.
 *
 * @retval ::RTEMS_TOO_MANY The handler specified by ``routine`` was already
 *   installed for the interrupt vector specified by ``vector`` with an
 *   argument equal to the argument specified by ``arg``.
 *
 * @retval ::RTEMS_UNSATISFIED The #RTEMS_INTERRUPT_REPLACE option was set in
 *   ``options`` and no handler to replace was installed.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive may obtain and release the object allocator mutex.  This may
 *   cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_handler_install(
  rtems_vector_number     vector,
  const char             *info,
  rtems_option            options,
  rtems_interrupt_handler routine,
  void                   *arg
);

/* Generated from spec:/rtems/intr/if/handler-remove */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Removes the interrupt handler routine and argument from the interrupt
 *   vector.
 *
 * @param vector is the interrupt vector number.
 *
 * @param routine is the interrupt handler routine to remove.
 *
 * @param arg is the interrupt handler argument to remove.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INCORRECT_STATE The service was not initialized.
 *
 * @retval ::RTEMS_INVALID_ADDRESS The ``routine`` parameter was NULL.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt vector associated with the
 *   number specified by ``vector``.
 *
 * @retval ::RTEMS_CALLED_FROM_ISR The directive was called from within
 *   interrupt context.
 *
 * @retval ::RTEMS_UNSATISFIED There was no handler routine and argument pair
 *   installed specified by ``routine`` and ``arg``.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive may obtain and release the object allocator mutex.  This may
 *   cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_handler_remove(
  rtems_vector_number     vector,
  rtems_interrupt_handler routine,
  void                   *arg
);

/* Generated from spec:/rtems/intr/if/vector-is-enabled */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Checks if the interrupt vector is enabled.
 *
 * @param vector is the interrupt vector number.
 *
 * @param[out] enabled is the pointer to a ``bool`` object.  When the directive
 *   call is successful, the enabled status of the interrupt associated with
 *   the interrupt vector specified by ``vector`` will be stored in this
 *   object.  When the interrupt was enabled for the processor executing the
 *   directive call at some time point during the call, the object value will
 *   be set to true, otherwise to false.
 *
 * The directive checks if the interrupt associated with the interrupt vector
 * specified by ``vector`` was enabled for the processor executing the
 * directive call at some time point during the call.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ADDRESS The ``enabled`` parameter was NULL.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt vector associated with the
 *   number specified by ``vector``.
 *
 * @par Notes
 * Interrupt vectors may be enabled by rtems_interrupt_vector_enable() and
 * disabled by rtems_interrupt_vector_disable().
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within interrupt context.
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive will not cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_vector_is_enabled(
  rtems_vector_number vector,
  bool               *enabled
);

/* Generated from spec:/rtems/intr/if/vector-enable */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Enables the interrupt vector.
 *
 * @param vector is the number of the interrupt vector to enable.
 *
 * The directive enables the interrupt vector specified by ``vector``. This
 * allows that interrupt service requests are issued to the target processors
 * of the interrupt vector.  Interrupt service requests for an interrupt vector
 * may be raised by rtems_interrupt_raise(), rtems_interrupt_raise_on(),
 * external signals, or messages.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt vector associated with the
 *   number specified by ``vector``.
 *
 * @retval ::RTEMS_UNSATISFIED The request to enable the interrupt vector has
 *   not been satisfied.
 *
 * @par Notes
 * The rtems_interrupt_get_attributes() directive may be used to check if an
 * interrupt vector can be enabled.  Interrupt vectors may be disabled by
 * rtems_interrupt_vector_disable().
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within interrupt context.
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive will not cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_vector_enable( rtems_vector_number vector );

/* Generated from spec:/rtems/intr/if/vector-disable */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Disables the interrupt vector.
 *
 * @param vector is the number of the interrupt vector to disable.
 *
 * The directive disables the interrupt vector specified by ``vector``.  This
 * prevents that an interrupt service request is issued to the target
 * processors of the interrupt vector.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt vector associated with the
 *   number specified by ``vector``.
 *
 * @retval ::RTEMS_UNSATISFIED The request to disable the interrupt vector has
 *   not been satisfied.
 *
 * @par Notes
 * The rtems_interrupt_get_attributes() directive may be used to check if an
 * interrupt vector can be disabled.  Interrupt vectors may be enabled by
 * rtems_interrupt_vector_enable().  There may be targets on which some
 * interrupt vectors cannot be disabled, for example a hardware watchdog
 * interrupt or software generated interrupts.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within interrupt context.
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive will not cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_vector_disable( rtems_vector_number vector );

/* Generated from spec:/rtems/intr/if/get-affinity */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Gets the processor affinity set of the interrupt vector.
 *
 * @param vector is the interrupt vector number.
 *
 * @param affinity_size is the size of the processor set referenced by
 *   ``affinity`` in bytes.
 *
 * @param[out] affinity is the pointer to a cpu_set_t object.  When the
 *   directive call is successful, the processor affinity set of the interrupt
 *   vector will be stored in this object.  A set bit in the processor set
 *   means that the corresponding processor is in the processor affinity set of
 *   the interrupt vector, otherwise the bit is cleared.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ADDRESS The ``affinity`` parameter was NULL.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt vector associated with the
 *   number specified by ``vector``.
 *
 * @retval ::RTEMS_INVALID_SIZE The size specified by ``affinity_size`` of the
 *   processor set was too small for the processor affinity set of the
 *   interrupt vector.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within interrupt context.
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive will not cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_get_affinity(
  rtems_vector_number vector,
  size_t              affinity_size,
  cpu_set_t          *affinity
);

/* Generated from spec:/rtems/intr/if/set-affinity */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Sets the processor affinity set of the interrupt vector.
 *
 * @param vector is the interrupt vector number.
 *
 * @param affinity_size is the size of the processor set referenced by
 *   ``affinity`` in bytes.
 *
 * @param affinity is the pointer to a cpu_set_t object.  The processor set
 *   defines the new processor affinity set of the interrupt vector.  A set bit
 *   in the processor set means that the corresponding processor shall be in
 *   the processor affinity set of the interrupt vector, otherwise the bit
 *   shall be cleared.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ADDRESS The ``affinity`` parameter was NULL.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt vector associated with the
 *   number specified by ``vector``.
 *
 * @retval ::RTEMS_INVALID_NUMBER The referenced processor set was not a valid
 *   new processor affinity set for the interrupt vector.
 *
 * @retval ::RTEMS_UNSATISFIED The request to set the processor affinity of the
 *   interrupt vector has not been satisfied.
 *
 * @par Notes
 * The rtems_interrupt_get_attributes() directive may be used to check if the
 * processor affinity of an interrupt vector can be set.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within interrupt context.
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive will not cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_set_affinity(
  rtems_vector_number vector,
  size_t              affinity_size,
  const cpu_set_t    *affinity
);

/* Generated from spec:/rtems/intr/if/signal-variant */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief This enumeration provides interrupt trigger signal variants.
 */
typedef enum {
  /**
   * @brief This interrupt signal variant indicates that the interrupt trigger
   *   signal is unspecified.
   */
  RTEMS_INTERRUPT_UNSPECIFIED_SIGNAL,

  /**
   * @brief This interrupt signal variant indicates that the interrupt cannot be
   *   triggered by a signal.
   */
  RTEMS_INTERRUPT_NO_SIGNAL,

  /**
   * @brief This interrupt signal variant indicates that the interrupt is
   *   triggered by a low level signal.
   */
  RTEMS_INTERRUPT_SIGNAL_LEVEL_LOW,

  /**
   * @brief This interrupt signal variant indicates that the interrupt is
   *   triggered by a high level signal.
   */
  RTEMS_INTERRUPT_SIGNAL_LEVEL_HIGH,

  /**
   * @brief This interrupt signal variant indicates that the interrupt is
   *   triggered by a falling edge signal.
   */
  RTEMS_INTERRUPT_SIGNAL_EDGE_FALLING,

  /**
   * @brief This interrupt signal variant indicates that the interrupt is
   *   triggered by a raising edge signal.
   */
  RTEMS_INTERRUPT_SIGNAL_EDGE_RAISING
} rtems_interrupt_signal_variant;

/* Generated from spec:/rtems/intr/if/attributes */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief This structure provides the attributes of an interrupt vector.
 *
 * The rtems_interrupt_get_attributes() directive may be used to obtain the
 * attributes of an interrupt vector.
 */
typedef struct {
  /**
   * @brief This member is true, if the interrupt vector is maskable by
   *   rtems_interrupt_local_disable(), otherwise it is false.
   *
   * Interrupt vectors which are not maskable by rtems_interrupt_local_disable()
   * should be used with care since they cannot use most operating system
   * services.
   */
  bool is_maskable;

  /**
   * @brief This member is true, if the interrupt vector can be enabled by
   *   rtems_interrupt_vector_enable(), otherwise it is false.
   *
   * When an interrupt vector can be enabled, this means that the enabled state
   * can always be changed from disabled to enabled.  For an interrupt vector
   * which can be enabled it follows that it may be enabled.
   */
  bool can_enable;

  /**
   * @brief This member is true, if the interrupt vector may be enabled by
   *   rtems_interrupt_vector_enable(), otherwise it is false.
   *
   * When an interrupt vector may be enabled, this means that the enabled state
   * may be changed from disabled to enabled.  The requested enabled state change
   * should be checked by rtems_interrupt_vector_is_enabled().  Some interrupt
   * vectors may be optionally available and cannot be enabled on a particular
   * target.
   */
  bool maybe_enable;

  /**
   * @brief This member is true, if the interrupt vector can be disabled by
   *   rtems_interrupt_vector_disable(), otherwise it is false.
   *
   * When an interrupt vector can be disabled, this means that the enabled state
   * can be changed from enabled to disabled.  For an interrupt vector which can
   * be disabled it follows that it may be disabled.
   */
  bool can_disable;

  /**
   * @brief This member is true, if the interrupt vector may be disabled by
   *   rtems_interrupt_vector_disable(), otherwise it is false.
   *
   * When an interrupt vector may be disabled, this means that the enabled state
   * may be changed from enabled to disabled.  The requested enabled state change
   * should be checked by rtems_interrupt_vector_is_enabled().  Some interrupt
   * vectors may be always enabled and cannot be disabled on a particular target.
   */
  bool maybe_disable;

  /**
   * @brief This member is true, if the interrupt vector can be raised by
   *   rtems_interrupt_raise(), otherwise it is false.
   */
  bool can_raise;

  /**
   * @brief This member is true, if the interrupt vector can be raised on a
   *   processor by rtems_interrupt_raise_on(), otherwise it is false.
   */
  bool can_raise_on;

  /**
   * @brief This member is true, if the interrupt vector can be cleared by
   *   rtems_interrupt_clear(), otherwise it is false.
   */
  bool can_clear;

  /**
   * @brief This member is true, if the pending status of the interrupt
   *   associated with the interrupt vector is cleared by an interrupt
   *   acknowledge from the processor, otherwise it is false.
   */
  bool cleared_by_acknowledge;

  /**
   * @brief This member is true, if the affinity set of the interrupt vector can
   *   be obtained by rtems_interrupt_get_affinity(), otherwise it is false.
   */
  bool can_get_affinity;

  /**
   * @brief This member is true, if the affinity set of the interrupt vector can
   *   be set by rtems_interrupt_set_affinity(), otherwise it is false.
   */
  bool can_set_affinity;

  /**
   * @brief This member is true, if the interrupt associated with the interrupt
   *   vector can be triggered by a message.
   *
   * Interrupts may be also triggered by signals, rtems_interrupt_raise(), or
   * rtems_interrupt_raise_on().  Examples for message triggered interrupts are
   * the PCIe MSI/MSI-X and the ARM GICv3 Locality-specific Peripheral Interrupts
   * (LPI).
   */
  bool can_be_triggered_by_message;

  /**
   * @brief This member describes the trigger signal of the interrupt associated
   *   with the interrupt vector.
   *
   * Interrupts are normally triggered by signals which indicate an interrupt
   * request from a peripheral.  Interrupts may be also triggered by messages,
   * rtems_interrupt_raise(), or rtems_interrupt_raise_on().
   */
  rtems_interrupt_signal_variant trigger_signal;
} rtems_interrupt_attributes;

/* Generated from spec:/rtems/intr/if/get-attributes */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Gets the attributes of the interrupt vector.
 *
 * @param vector is the interrupt vector number.
 *
 * @param[out] attributes is the pointer to an rtems_interrupt_attributes
 *   object.  When the directive call is successful, the attributes of the
 *   interrupt vector will be stored in this object.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ADDRESS The ``attributes`` parameter was NULL.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt vector associated with the
 *   number specified by ``vector``.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within interrupt context.
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive will not cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_get_attributes(
  rtems_vector_number         vector,
  rtems_interrupt_attributes *attributes
);

/* Generated from spec:/rtems/intr/if/handler-iterate */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Iterates over all interrupt handler installed at the interrupt
 *   vector.
 *
 * @param vector is the interrupt vector number.
 *
 * @param routine is the visitor routine.
 *
 * @param arg is the visitor argument.
 *
 * For each installed handler at the interrupt vector the visitor function
 * specified by ``routine`` will be called with the argument specified by
 * ``arg`` and the handler information, options, routine and argument.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INCORRECT_STATE The service was not initialized.
 *
 * @retval ::RTEMS_INVALID_ADDRESS The ``routine`` parameter was NULL.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt vector associated with the
 *   number specified by ``vector``.
 *
 * @retval ::RTEMS_CALLED_FROM_ISR The directive was called from within
 *   interrupt context.
 *
 * @par Notes
 * @parblock
 * The directive is intended for system information and diagnostics.
 *
 * Never install or remove an interrupt handler within the visitor function.
 * This may result in a deadlock.
 * @endparblock
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive may obtain and release the object allocator mutex.  This may
 *   cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_handler_iterate(
  rtems_vector_number                 vector,
  rtems_interrupt_per_handler_routine routine,
  void                               *arg
);

/* Generated from spec:/rtems/intr/if/server-default */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief The constant represents the index of the default interrupt server.
 */
#define RTEMS_INTERRUPT_SERVER_DEFAULT 0

/* Generated from spec:/rtems/intr/if/server-control */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief This structure represents an interrupt server.
 *
 * @par Notes
 * This structure shall be treated as an opaque data type from the API point of
 * view.  Members shall not be accessed directly.  The structure is initialized
 * by rtems_interrupt_server_create() and maintained by the interrupt server
 * support.
 */
typedef struct rtems_interrupt_server_control {
  #if defined(RTEMS_SMP)
    /**
     * @brief This member is the ISR lock protecting the server control state.
     */
    rtems_interrupt_lock lock;
  #endif

  /**
   * @brief This member is the chain of pending interrupt entries.
   */
  Chain_Control entries;

  /**
   * @brief This member is the identifier of the server task.
   */
  rtems_id server;

  /**
   * @brief This member is the error count.
   */
  unsigned long errors;

  /**
   * @brief This member is the server index.
   */
  uint32_t index;

  /**
   * @brief This member is the node for the interrupt server registry.
   */
  Chain_Node node;

  /**
   * @brief This member is the optional handler to destroy the interrupt server
   *   control.
   */
  void ( *destroy )( struct rtems_interrupt_server_control * );
} rtems_interrupt_server_control;

/* Generated from spec:/rtems/intr/if/server-config */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief This structure defines an interrupt server configuration.
 *
 * @par Notes
 * See also rtems_interrupt_server_create().
 */
typedef struct {
  /**
   * @brief This member is the task name of the interrupt server.
   */
  rtems_name name;

  /**
   * @brief This member is the initial task priority of the interrupt server.
   */
  rtems_task_priority priority;

  /**
   * @brief This member is the task storage area of the interrupt server.
   *
   * It shall be NULL for interrupt servers created by
   * rtems_interrupt_server_create().
   */
  void *storage_area;

  /**
   * @brief This member is the task storage size of the interrupt server.
   *
   * For interrupt servers created by rtems_interrupt_server_create() this is the
   * task stack size.
   */
  size_t storage_size;

  /**
   * @brief This member is the initial mode set of the interrupt server.
   */
  rtems_mode modes;

  /**
   * @brief This member is the attribute set of the interrupt server.
   */
  rtems_attribute attributes;

  /**
   * @brief This member is an optional handler to destroy the interrupt server
   *   control handed over to rtems_interrupt_server_create().
   *
   * The destroy handler is optional and may be NULL.  If the destroy handler is
   * present, it is called from within the context of the interrupt server to be
   * deleted, see also rtems_interrupt_server_delete().
   */
  void ( *destroy )( rtems_interrupt_server_control * );
} rtems_interrupt_server_config;

/* Generated from spec:/rtems/intr/if/server-initialize */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Initializes the interrupt server tasks.
 *
 * @param priority is the initial task priority of the created interrupt
 *   servers.
 *
 * @param stack_size is the task stack size of the created interrupt servers.
 *
 * @param modes is the initial mode set of the created interrupt servers.
 *
 * @param attributes is the attribute set of the created interrupt servers.
 *
 * @param[out] server_count is the pointer to an uint32_t object or NULL. When
 *   the pointer is not equal to NULL, the count of successfully created
 *   interrupt servers is stored in this object regardless of the return
 *   status.
 *
 * The directive tries to create an interrupt server task for each online
 * processor in the system.  The tasks will have the initial priority specified
 * by ``priority``, the stack size specified by ``stack_size``, the initial
 * mode set specified by ``modes``, and the attribute set specified by
 * ``attributes``.  The count of successfully created server tasks will be
 * returned in ``server_count`` if the pointer is not equal to NULL.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INCORRECT_STATE The interrupt servers were already
 *   initialized.
 *
 * @return The directive uses rtems_task_create().  If this directive fails,
 *   then its error status will be returned.
 *
 * @par Notes
 * @parblock
 * Interrupt handlers may be installed on an interrupt server with
 * rtems_interrupt_server_handler_install() and removed with
 * rtems_interrupt_server_handler_remove() using a server index.  In case of an
 * interrupt, the request will be forwarded to the interrupt server.  The
 * handlers are executed within the interrupt server context.  If one handler
 * blocks on something this may delay the processing of other handlers.
 *
 * Interrupt servers may be deleted by rtems_interrupt_server_delete().
 * @endparblock
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive may obtain and release the object allocator mutex.  This may
 *   cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_server_initialize(
  rtems_task_priority priority,
  size_t              stack_size,
  rtems_mode          modes,
  rtems_attribute     attributes,
  uint32_t           *server_count
);

/* Generated from spec:/rtems/intr/if/server-create */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Creates an interrupt server.
 *
 * @param[out] control is the pointer to an rtems_interrupt_server_control
 *   object.  When the directive call was successful, the ownership of the
 *   object was transferred from the caller of the directive to the interrupt
 *   server management.
 *
 * @param config is the interrupt server configuration.
 *
 * @param[out] server_index is the pointer to an uint32_t object.  When the
 *   directive call was successful, the index of the created interrupt server
 *   will be stored in this object.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @return The directive uses rtems_task_create().  If this directive fails,
 *   then its error status will be returned.
 *
 * @par Notes
 * See also rtems_interrupt_server_initialize() and
 * rtems_interrupt_server_delete().
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive may obtain and release the object allocator mutex.  This may
 *   cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_server_create(
  rtems_interrupt_server_control      *control,
  const rtems_interrupt_server_config *config,
  uint32_t                            *server_index
);

/* Generated from spec:/rtems/intr/if/server-handler-install */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Installs the interrupt handler routine and argument at the interrupt
 *   vector on the interrupt server.
 *
 * @param server_index is the interrupt server index.  The constant
 *   #RTEMS_INTERRUPT_SERVER_DEFAULT may be used to specify the default
 *   interrupt server.
 *
 * @param vector is the interrupt vector number.
 *
 * @param info is the descriptive information of the interrupt handler to
 *   install.
 *
 * @param options is the interrupt handler install option set.
 *
 * @param routine is the interrupt handler routine to install.
 *
 * @param arg is the interrupt handler argument to install.
 *
 * The handler routine specified by ``routine`` will be executed within the
 * context of the interrupt server task specified by ``server_index``.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt server associated with the
 *   index specified by ``server_index``.
 *
 * @retval ::RTEMS_CALLED_FROM_ISR The directive was called from within
 *   interrupt context.
 *
 * @retval ::RTEMS_INVALID_ADDRESS The ``routine`` parameter was NULL.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt vector associated with the
 *   number specified by ``vector``.
 *
 * @retval ::RTEMS_INVALID_NUMBER An option specified by ``info`` was not
 *   applicable.
 *
 * @retval ::RTEMS_RESOURCE_IN_USE The #RTEMS_INTERRUPT_UNIQUE option was set
 *   in ``info`` and the interrupt vector was already occupied by a handler.
 *
 * @retval ::RTEMS_RESOURCE_IN_USE The #RTEMS_INTERRUPT_SHARED option was set
 *   in ``info`` and the interrupt vector was already occupied by a unique
 *   handler.
 *
 * @retval ::RTEMS_TOO_MANY The handler specified by ``routine`` was already
 *   installed for the interrupt vector specified by ``vector`` with an
 *   argument equal to the argument specified by ``arg``.
 *
 * @retval ::RTEMS_UNSATISFIED The #RTEMS_INTERRUPT_REPLACE option was set in
 *   ``info`` and no handler to replace was installed.
 *
 * @par Notes
 * See also rtems_interrupt_handler_install().
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive may obtain and release the object allocator mutex.  This may
 *   cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_server_handler_install(
  uint32_t                server_index,
  rtems_vector_number     vector,
  const char             *info,
  rtems_option            options,
  rtems_interrupt_handler routine,
  void                   *arg
);

/* Generated from spec:/rtems/intr/if/server-handler-remove */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Removes the interrupt handler routine and argument from the interrupt
 *   vector and the interrupt server.
 *
 * @param server_index is the interrupt server index.  The constant
 *   #RTEMS_INTERRUPT_SERVER_DEFAULT may be used to specify the default
 *   interrupt server.
 *
 * @param vector is the interrupt vector number.
 *
 * @param routine is the interrupt handler routine to remove.
 *
 * @param arg is the interrupt handler argument to remove.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt server associated with the
 *   index specified by ``server_index``.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt vector associated with the
 *   number specified by ``vector``.
 *
 * @retval ::RTEMS_UNSATISFIED There was no handler routine and argument pair
 *   installed specified by ``routine`` and ``arg``.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within task context.
 *
 * * The directive may obtain and release the object allocator mutex.  This may
 *   cause the calling task to be preempted.
 *
 * * The directive sends a request to another task and waits for a response.
 *   This may cause the calling task to be blocked and unblocked.
 *
 * * The directive shall not be called from within the context of an interrupt
 *   server.  Calling the directive from within the context of an interrupt
 *   server is undefined behaviour.
 * @endparblock
 */
rtems_status_code rtems_interrupt_server_handler_remove(
  uint32_t                server_index,
  rtems_vector_number     vector,
  rtems_interrupt_handler routine,
  void                   *arg
);

/* Generated from spec:/rtems/intr/if/server-set-affinity */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Sets the processor affinity of the interrupt server.
 *
 * @param server_index is the interrupt server index.  The constant
 *   #RTEMS_INTERRUPT_SERVER_DEFAULT may be used to specify the default
 *   interrupt server.
 *
 * @param affinity_size is the size of the processor set referenced by
 *   ``affinity`` in bytes.
 *
 * @param affinity is the pointer to a cpu_set_t object.  The processor set
 *   defines the new processor affinity set of the interrupt server.  A set bit
 *   in the processor set means that the corresponding processor shall be in
 *   the processor affinity set of the task, otherwise the bit shall be
 *   cleared.
 *
 * @param priority is the new real priority for the interrupt server.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt server associated with the
 *   index specified by ``server_index``.
 *
 * @return The directive uses rtems_scheduler_ident_by_processor_set(),
 *   rtems_task_set_scheduler(), and rtems_task_set_affinity().  If one of
 *   these directive fails, then its error status will be returned.
 *
 * @par Notes
 * @parblock
 * The scheduler is set determined by the highest numbered processor in the
 * affinity set specified by ``affinity``.
 *
 * This operation is only reliable in case the interrupt server was suspended
 * via rtems_interrupt_server_suspend().
 * @endparblock
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within interrupt context.
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive may change the processor affinity of a task.  This may cause
 *   the calling task to be preempted.
 *
 * * The directive may change the priority of a task.  This may cause the
 *   calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_server_set_affinity(
  uint32_t            server_index,
  size_t              affinity_size,
  const cpu_set_t    *affinity,
  rtems_task_priority priority
);

/* Generated from spec:/rtems/intr/if/server-delete */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Deletes the interrupt server.
 *
 * @param server_index is the index of the interrupt server to delete.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt server associated with the
 *   server index specified by ``server_index``.
 *
 * @par Notes
 * @parblock
 * The interrupt server deletes itself, so after the return of the directive
 * the interrupt server may be still in the termination process depending on
 * the task priorities of the system.
 *
 * See also rtems_interrupt_server_create().
 * @endparblock
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within task context.
 *
 * * The directive shall not be called from within the context of an interrupt
 *   server.  Calling the directive from within the context of an interrupt
 *   server is undefined behaviour.
 *
 * * The directive sends a request to another task and waits for a response.
 *   This may cause the calling task to be blocked and unblocked.
 * @endparblock
 */
rtems_status_code rtems_interrupt_server_delete( uint32_t server_index );

/* Generated from spec:/rtems/intr/if/server-suspend */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Suspends the interrupt server.
 *
 * @param server_index is the index of the interrupt server to suspend.  The
 *   constant #RTEMS_INTERRUPT_SERVER_DEFAULT may be used to specify the
 *   default interrupt server.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt server associated with the
 *   index specified by ``server_index``.
 *
 * @par Notes
 * Interrupt server may be resumed by rtems_interrupt_server_resume().
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within task context.
 *
 * * The directive shall not be called from within the context of an interrupt
 *   server.  Calling the directive from within the context of an interrupt
 *   server is undefined behaviour.
 *
 * * The directive sends a request to another task and waits for a response.
 *   This may cause the calling task to be blocked and unblocked.
 * @endparblock
 */
rtems_status_code rtems_interrupt_server_suspend( uint32_t server_index );

/* Generated from spec:/rtems/intr/if/server-resume */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Resumes the interrupt server.
 *
 * @param server_index is the index of the interrupt server to resume.  The
 *   constant #RTEMS_INTERRUPT_SERVER_DEFAULT may be used to specify the
 *   default interrupt server.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt server associated with the
 *   index specified by ``server_index``.
 *
 * @par Notes
 * Interrupt server may be suspended by rtems_interrupt_server_suspend().
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within task context.
 *
 * * The directive shall not be called from within the context of an interrupt
 *   server.  Calling the directive from within the context of an interrupt
 *   server is undefined behaviour.
 *
 * * The directive sends a request to another task and waits for a response.
 *   This may cause the calling task to be blocked and unblocked.
 * @endparblock
 */
rtems_status_code rtems_interrupt_server_resume( uint32_t server_index );

/* Generated from spec:/rtems/intr/if/server-move */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Moves the interrupt handlers installed at the interrupt vector and
 *   the source interrupt server to the destination interrupt server.
 *
 * @param source_server_index is the index of the source interrupt server.  The
 *   constant #RTEMS_INTERRUPT_SERVER_DEFAULT may be used to specify the
 *   default interrupt server.
 *
 * @param vector is the interrupt vector number.
 *
 * @param destination_server_index is the index of the destination interrupt
 *   server.  The constant #RTEMS_INTERRUPT_SERVER_DEFAULT may be used to
 *   specify the default interrupt server.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt server associated with the
 *   index specified by ``source_server_index``.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt server associated with the
 *   index specified by ``destination_server_index``.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt vector associated with the
 *   number specified by ``vector``.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within task context.
 *
 * * The directive shall not be called from within the context of an interrupt
 *   server.  Calling the directive from within the context of an interrupt
 *   server is undefined behaviour.
 *
 * * The directive sends a request to another task and waits for a response.
 *   This may cause the calling task to be blocked and unblocked.
 * @endparblock
 */
rtems_status_code rtems_interrupt_server_move(
  uint32_t            source_server_index,
  rtems_vector_number vector,
  uint32_t            destination_server_index
);

/* Generated from spec:/rtems/intr/if/server-handler-iterate */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Iterates over all interrupt handler installed at the interrupt vector
 *   and interrupt server.
 *
 * @param server_index is the index of the interrupt server.
 *
 * @param vector is the interrupt vector number.
 *
 * @param routine is the visitor routine.
 *
 * @param arg is the visitor argument.
 *
 * For each installed handler at the interrupt vector and interrupt server the
 * visitor function specified by ``vector`` will be called with the argument
 * specified by ``routine`` and the handler information, options, routine and
 * argument.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt server associated with the
 *   index specified by ``server_index``.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt vector associated with the
 *   number specified by ``vector``.
 *
 * @par Notes
 * @parblock
 * The directive is intended for system information and diagnostics.
 *
 * Never install or remove an interrupt handler within the visitor function.
 * This may result in a deadlock.
 * @endparblock
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive may obtain and release the object allocator mutex.  This may
 *   cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_server_handler_iterate(
  uint32_t                            server_index,
  rtems_vector_number                 vector,
  rtems_interrupt_per_handler_routine routine,
  void                               *arg
);

/* Generated from spec:/rtems/intr/if/server-action */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief This structure represents an interrupt server action.
 *
 * @par Notes
 * This structure shall be treated as an opaque data type from the API point of
 * view.  Members shall not be accessed directly.
 */
typedef struct rtems_interrupt_server_action {
  /**
   * @brief This member is the reference to the next action or NULL.
   */
  struct rtems_interrupt_server_action *next;

  /**
   * @brief This member is the interrupt handler.
   */
  rtems_interrupt_handler handler;

  /**
   * @brief This member is the interrupt handler argument.
   */
  void *arg;
} rtems_interrupt_server_action;

/* Generated from spec:/rtems/intr/if/server-entry */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief This structure represents an interrupt server entry.
 *
 * @par Notes
 * This structure shall be treated as an opaque data type from the API point of
 * view.  Members shall not be accessed directly.  An entry is initialized by
 * rtems_interrupt_server_entry_initialize() and destroyed by
 * rtems_interrupt_server_entry_destroy().  Interrupt server actions can be
 * prepended to the entry by rtems_interrupt_server_action_prepend().  The
 * entry is submitted to be serviced by rtems_interrupt_server_entry_submit().
 */
typedef struct {
  /**
   * @brief This member is the node for the interrupt entry processing.
   */
  Chain_Node node;

  /**
   * @brief This member references the interrupt server used to process the
   *   entry.
   */
  rtems_interrupt_server_control *server;

  /**
   * @brief This member is the interrupt vector number.
   */
  rtems_vector_number vector;

  /**
   * @brief This member is the interrupt server actions list head.
   */
  rtems_interrupt_server_action *actions;
} rtems_interrupt_server_entry;

/* Generated from spec:/rtems/intr/if/server-entry-initialize */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Initializes the interrupt server entry.
 *
 * @param server_index is the interrupt server index.  The constant
 *   #RTEMS_INTERRUPT_SERVER_DEFAULT may be used to specify the default
 *   interrupt server.
 *
 * @param entry is the interrupt server entry to initialize.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt server associated with the
 *   index specified by ``server_index``.
 *
 * @par Notes
 * After initialization, the list of actions of the interrupt server entry is
 * empty.  Actions may be prepended by rtems_interrupt_server_action_prepend().
 * Interrupt server entries may be moved to another interrupt vector with
 * rtems_interrupt_server_entry_move().  Server entries may be submitted to get
 * serviced by the interrupt server with rtems_interrupt_server_entry_submit().
 * Server entries may be destroyed by rtems_interrupt_server_entry_destroy().
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive may obtain and release the object allocator mutex.  This may
 *   cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_server_entry_initialize(
  uint32_t                      server_index,
  rtems_interrupt_server_entry *entry
);

/* Generated from spec:/rtems/intr/if/server-action-prepend */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Prepends the interrupt server action to the list of actions of the
 *   interrupt server entry.
 *
 * @param[in,out] entry is the interrupt server entry to prepend the interrupt
 *   server action.  It shall have been initialized via
 *   rtems_interrupt_server_entry_initialize().
 *
 * @param[out] action is the interrupt server action to initialize and prepend
 *   to the list of actions of the entry.
 *
 * @param routine is the interrupt handler routine to set in the action.
 *
 * @param arg is the interrupt handler argument to set in the action.
 *
 * @par Notes
 * No error checking is performed by the directive.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within interrupt context.
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive will not cause the calling task to be preempted.
 *
 * * The interrupt server entry shall have been initialized by
 *   rtems_interrupt_server_entry_initialize() and further optional calls to
 *   rtems_interrupt_server_action_prepend().
 *
 * * The directive shall not be called concurrently with
 *   rtems_interrupt_server_action_prepend() with the same interrupt server
 *   entry. Calling the directive under this condition is undefined behaviour.
 *
 * * The directive shall not be called concurrently with
 *   rtems_interrupt_server_entry_move() with the same interrupt server entry.
 *   Calling the directive under this condition is undefined behaviour.
 *
 * * The directive shall not be called concurrently with
 *   rtems_interrupt_server_entry_submit() with the same interrupt server
 *   entry. Calling the directive under this condition is undefined behaviour.
 *
 * * The directive shall not be called while the interrupt server entry is
 *   pending on or serviced by its current interrupt server.  Calling the
 *   directive under these conditions is undefined behaviour.
 * @endparblock
 */
void rtems_interrupt_server_action_prepend(
  rtems_interrupt_server_entry  *entry,
  rtems_interrupt_server_action *action,
  rtems_interrupt_handler        routine,
  void                          *arg
);

/* Generated from spec:/rtems/intr/if/server-entry-destroy */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Destroys the interrupt server entry.
 *
 * @param[in,out] entry is the interrupt server entry to destroy.
 *
 * @par Notes
 * No error checking is performed by the directive.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within task context.
 *
 * * The directive shall not be called from within the context of an interrupt
 *   server.  Calling the directive from within the context of an interrupt
 *   server is undefined behaviour.
 *
 * * The directive sends a request to another task and waits for a response.
 *   This may cause the calling task to be blocked and unblocked.
 *
 * * The interrupt server entry shall have been initialized by
 *   rtems_interrupt_server_entry_initialize() and further optional calls to
 *   rtems_interrupt_server_action_prepend().
 * @endparblock
 */
void rtems_interrupt_server_entry_destroy(
  rtems_interrupt_server_entry *entry
);

/* Generated from spec:/rtems/intr/if/server-entry-submit */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Submits the interrupt server entry to be serviced by the interrupt
 *   server.
 *
 * @param entry is the interrupt server entry to submit.
 *
 * The directive appends the entry to the pending entries of the interrupt
 * server.  The interrupt server is notified that a new entry is pending.  Once
 * the interrupt server is scheduled it services the actions of all pending
 * entries.
 *
 * @par Notes
 * @parblock
 * This directive may be used to do a two-step interrupt processing.  The first
 * step is done from within interrupt context by a call to this directive.  The
 * second step is then done from within the context of the interrupt server.
 *
 * No error checking is performed by the directive.
 *
 * A submitted entry may be destroyed by
 * rtems_interrupt_server_entry_destroy().
 * @endparblock
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within interrupt context.
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive may unblock a task.  This may cause the calling task to be
 *   preempted.
 *
 * * The interrupt server entry shall have been initialized by
 *   rtems_interrupt_server_entry_initialize() and further optional calls to
 *   rtems_interrupt_server_action_prepend().
 *
 * * The directive shall not be called concurrently with
 *   rtems_interrupt_server_action_prepend() with the same interrupt server
 *   entry. Calling the directive under this condition is undefined behaviour.
 *
 * * The directive shall not be called concurrently with
 *   rtems_interrupt_server_entry_move() with the same interrupt server entry.
 *   Calling the directive under this condition is undefined behaviour.
 * @endparblock
 */
void rtems_interrupt_server_entry_submit(
  rtems_interrupt_server_entry *entry
);

/* Generated from spec:/rtems/intr/if/server-entry-move */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Moves the interrupt server entry to the interrupt server.
 *
 * @param entry is the interrupt server entry to move.
 *
 * @param server_index is the index of the destination interrupt server.  The
 *   constant #RTEMS_INTERRUPT_SERVER_DEFAULT may be used to specify the
 *   default interrupt server.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt server associated with the
 *   index specified by ``server_index``.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive may obtain and release the object allocator mutex.  This may
 *   cause the calling task to be preempted.
 *
 * * The interrupt server entry shall have been initialized by
 *   rtems_interrupt_server_entry_initialize() and further optional calls to
 *   rtems_interrupt_server_action_prepend().
 *
 * * The directive shall not be called concurrently with
 *   rtems_interrupt_server_action_prepend() with the same interrupt server
 *   entry. Calling the directive under this condition is undefined behaviour.
 *
 * * The directive shall not be called concurrently with
 *   rtems_interrupt_server_entry_move() with the same interrupt server entry.
 *   Calling the directive under this condition is undefined behaviour.
 *
 * * The directive shall not be called concurrently with
 *   rtems_interrupt_server_entry_submit() with the same interrupt server
 *   entry. Calling the directive under this condition is undefined behaviour.
 *
 * * The directive shall not be called while the interrupt server entry is
 *   pending on or serviced by its current interrupt server.  Calling the
 *   directive under these conditions is undefined behaviour.
 * @endparblock
 */
rtems_status_code rtems_interrupt_server_entry_move(
  rtems_interrupt_server_entry *entry,
  uint32_t                      server_index
);

/* Generated from spec:/rtems/intr/if/server-request */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief This structure represents an interrupt server request.
 *
 * @par Notes
 * This structure shall be treated as an opaque data type from the API point of
 * view.  Members shall not be accessed directly.  A request is initialized by
 * rtems_interrupt_server_request_initialize() and destroyed by
 * rtems_interrupt_server_request_destroy().  The interrupt vector of the
 * request can be set by rtems_interrupt_server_request_set_vector().  The
 * request is submitted to be serviced by
 * rtems_interrupt_server_request_submit().
 */
typedef struct {
  /**
   * @brief This member is the interrupt server entry.
   */
  rtems_interrupt_server_entry entry;

  /**
   * @brief This member is the interrupt server action.
   */
  rtems_interrupt_server_action action;
} rtems_interrupt_server_request;

/* Generated from spec:/rtems/intr/if/server-request-initialize */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Initializes the interrupt server request.
 *
 * @param server_index is the interrupt server index.  The constant
 *   #RTEMS_INTERRUPT_SERVER_DEFAULT may be used to specify the default
 *   interrupt server.
 *
 * @param[out] request is the interrupt server request to initialize.
 *
 * @param routine is the interrupt handler routine for the request action.
 *
 * @param arg is the interrupt handler argument for the request action.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ID There was no interrupt server associated with the
 *   index specified by ``server_index``.
 *
 * @par Notes
 * An interrupt server requests consists of an interrupt server entry and
 * exactly one interrupt server action.  The interrupt vector of the request
 * may be changed with rtems_interrupt_server_request_set_vector().  Interrupt
 * server requests may be submitted to get serviced by the interrupt server
 * with rtems_interrupt_server_request_submit().  Requests may be destroyed by
 * rtems_interrupt_server_request_destroy().
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive may obtain and release the object allocator mutex.  This may
 *   cause the calling task to be preempted.
 * @endparblock
 */
rtems_status_code rtems_interrupt_server_request_initialize(
  uint32_t                        server_index,
  rtems_interrupt_server_request *request,
  rtems_interrupt_handler         routine,
  void                           *arg
);

/* Generated from spec:/rtems/intr/if/server-request-set-vector */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Sets the interrupt vector in the interrupt server request.
 *
 * @param[in,out] request is the interrupt server request to change.
 *
 * @param vector is the interrupt vector number to be used by the request.
 *
 * @par Notes
 * @parblock
 * By default, the interrupt vector of an interrupt server request is set to a
 * special value which is outside the range of vectors supported by the
 * interrupt controller hardware.
 *
 * Calls to rtems_interrupt_server_request_submit() will disable the interrupt
 * vector of the request.  After processing of the request by the interrupt
 * server the interrupt vector will be enabled again.
 * @endparblock
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within interrupt context.
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive will not cause the calling task to be preempted.
 *
 * * The interrupt server request shall have been initialized by
 *   rtems_interrupt_server_request_initialize().
 *
 * * The directive shall not be called concurrently with
 *   rtems_interrupt_server_request_set_vector() with the same interrupt server
 *   request.  Calling the directive under this condition is undefined
 *   behaviour.
 *
 * * The directive shall not be called concurrently with
 *   rtems_interrupt_server_request_submit() with the same interrupt server
 *   request. Calling the directive under this condition is undefined
 *   behaviour.
 *
 * * The directive shall not be called while the interrupt server entry is
 *   pending on or serviced by its current interrupt server.  Calling the
 *   directive under these conditions is undefined behaviour.
 * @endparblock
 */
static inline void rtems_interrupt_server_request_set_vector(
  rtems_interrupt_server_request *request,
  rtems_vector_number             vector
)
{
  request->entry.vector = vector;
}

/* Generated from spec:/rtems/intr/if/server-request-destroy */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Destroys the interrupt server request.
 *
 * @param[in,out] request is the interrupt server request to destroy.
 *
 * @par Notes
 * No error checking is performed by the directive.
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within task context.
 *
 * * The directive shall not be called from within the context of an interrupt
 *   server.  Calling the directive from within the context of an interrupt
 *   server is undefined behaviour.
 *
 * * The directive sends a request to another task and waits for a response.
 *   This may cause the calling task to be blocked and unblocked.
 *
 * * The interrupt server request shall have been initialized by
 *   rtems_interrupt_server_request_initialize().
 * @endparblock
 */
static inline void rtems_interrupt_server_request_destroy(
  rtems_interrupt_server_request *request
)
{
  rtems_interrupt_server_entry_destroy( &request->entry );
}

/* Generated from spec:/rtems/intr/if/server-request-submit */

/**
 * @ingroup RTEMSAPIClassicIntr
 *
 * @brief Submits the interrupt server request to be serviced by the interrupt
 *   server.
 *
 * @param[in,out] request is the interrupt server request to submit.
 *
 * The directive appends the interrupt server entry of the request to the
 * pending entries of the interrupt server.  The interrupt server is notified
 * that a new entry is pending.  Once the interrupt server is scheduled it
 * services the actions of all pending entries.
 *
 * @par Notes
 * @parblock
 * This directive may be used to do a two-step interrupt processing.  The first
 * step is done from within interrupt context by a call to this directive.  The
 * second step is then done from within the context of the interrupt server.
 *
 * No error checking is performed by the directive.
 *
 * A submitted request may be destroyed by
 * rtems_interrupt_server_request_destroy().
 * @endparblock
 *
 * @par Constraints
 * @parblock
 * The following constraints apply to this directive:
 *
 * * The directive may be called from within interrupt context.
 *
 * * The directive may be called from within device driver initialization
 *   context.
 *
 * * The directive may be called from within task context.
 *
 * * The directive may unblock a task.  This may cause the calling task to be
 *   preempted.
 *
 * * The interrupt server request shall have been initialized by
 *   rtems_interrupt_server_request_initialize().
 *
 * * The directive shall not be called concurrently with
 *   rtems_interrupt_server_request_set_vector() with the same interrupt server
 *   request.  Calling the directive under this condition is undefined
 *   behaviour.
 * @endparblock
 */
static inline void rtems_interrupt_server_request_submit(
  rtems_interrupt_server_request *request
)
{
  rtems_interrupt_server_entry_submit( &request->entry );
}

#ifdef __cplusplus
}
#endif

#endif /* _RTEMS_IRQ_EXTENSION_H */
