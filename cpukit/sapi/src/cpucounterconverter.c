/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSImplCounter
 *
 * @brief This source file contains a implementation of the counter value
 *   conversion functions.
 */

/*
 * Copyright (C) 2014, 2018 embedded brains GmbH & Co. KG
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

/**
 * @defgroup RTEMSImplCounter Counter
 *
 * @ingroup RTEMSImpl
 *
 * @brief This group contains the counter implementation.
 */

#include <rtems/counter.h>
#include <rtems/sysinit.h>

RTEMS_STATIC_ASSERT(
  sizeof( rtems_counter_ticks ) <= sizeof( uint32_t ),
  type
);

/**
 * @brief This constant defines the counter frequency in Hz which the converter
 *   uses before the first call of rtems_counter_initialize_converter().
 */
#define COUNTER_DEFAULT_FREQUENCY UINT32_C( 100000000 )

/**
 * @brief Gets the scaler of the conversion of counter ticks into nanoseconds.
 *
 * @param frequency is the counter frequency in Hz.  The frequency shall be
 *   greater than zero.
 *
 * @return Returns 10**9 multiplied by 2**32 and divided by the frequency,
 *   rounded up.
 */
#define COUNTER_TO_NS_SCALER( frequency ) \
  ( ( ( UINT64_C( 1000000000 ) << 32 ) + ( frequency ) - 1 ) / ( frequency ) )

/**
 * @brief Gets the scaler of the conversion of nanoseconds into counter ticks.
 *
 * @param frequency is the counter frequency in Hz.
 *
 * @return Returns the frequency multiplied by 2**32 and divided by 10**9,
 *   rounded up.
 */
#define COUNTER_FROM_NS_SCALER( frequency )                          \
  ( ( ( (uint64_t) ( frequency ) << 32 ) + UINT64_C( 999999999 ) ) / \
    UINT64_C( 1000000000 ) )

/**
 * @brief Gets the scaler of the conversion of counter ticks into signed binary
 *   time.
 *
 * @param frequency is the counter frequency in Hz.  The frequency shall be
 *   greater than zero.
 *
 * @return Returns 2**63 divided by the frequency, rounded up.
 */
#define COUNTER_TO_SBT_SCALER( frequency ) \
  ( ( ( UINT64_C( 1 ) << 63 ) + ( frequency ) - 1 ) / ( frequency ) )

/**
 * @brief This structure contains the state of the counter value converter.
 */
typedef struct {
  /**
   * @brief This member contains the scaler of the conversion of counter ticks
   *   into nanoseconds.
   */
  uint64_t to_ns_scaler;

  /**
   * @brief This member contains the scaler of the conversion of nanoseconds
   *   into counter ticks.
   */
  uint64_t from_ns_scaler;

  /**
   * @brief This member contains the scaler of the conversion of counter ticks
   *   into signed binary time.
   */
  uint64_t to_sbt_scaler;

  /**
   * @brief This member contains the scaler of the conversion of signed binary
   *   time into counter ticks.  The scaler is the counter frequency in Hz.
   */
  uint32_t from_sbt_scaler;
} Counter_Converter;

/**
 * @brief This object contains the state of the counter value converter.
 */
static Counter_Converter _Counter_Converter = {
  .to_ns_scaler = COUNTER_TO_NS_SCALER( COUNTER_DEFAULT_FREQUENCY ),
  .from_ns_scaler = COUNTER_FROM_NS_SCALER( COUNTER_DEFAULT_FREQUENCY ),
  .to_sbt_scaler = COUNTER_TO_SBT_SCALER( COUNTER_DEFAULT_FREQUENCY ),
  .from_sbt_scaler = COUNTER_DEFAULT_FREQUENCY
};

/**
 * @brief Scales the value.
 *
 * The product of the 32-bit value and the 64-bit scaler has 96 bits.  The two
 * partial products fit into 64 bits each.  The product shifted right by the
 * shift value shall be less than 2**64.
 *
 * @param value is the value to scale.
 *
 * @param scaler is the scaler.
 *
 * @param shift is the shift value.  The shift value shall be at most 32.
 *
 * @return Returns the product of the value and the scaler shifted right by
 *   the shift value.
 */
static uint64_t _Counter_Scale(
  uint32_t     value,
  uint64_t     scaler,
  unsigned int shift
)
{
  uint64_t high;
  uint64_t low;

  high = (uint64_t) value * (uint32_t) ( scaler >> 32 );
  low = (uint64_t) value * (uint32_t) scaler;

  return ( high << ( 32 - shift ) ) + ( low >> shift );
}

uint64_t rtems_counter_ticks_to_nanoseconds( rtems_counter_ticks ticks )
{
  return _Counter_Scale( ticks, _Counter_Converter.to_ns_scaler, 32 );
}

rtems_counter_ticks rtems_counter_nanoseconds_to_ticks( uint32_t nanoseconds )
{
  uint64_t ticks;

  ticks = _Counter_Scale( nanoseconds, _Counter_Converter.from_ns_scaler, 32 );

  if ( ticks > UINT32_MAX ) {
    return UINT32_MAX;
  }

  return (rtems_counter_ticks) ticks;
}

int64_t rtems_counter_ticks_to_sbintime( rtems_counter_ticks ticks )
{
  uint64_t sbt;

  sbt = _Counter_Scale( ticks, _Counter_Converter.to_sbt_scaler, 31 );

  if ( sbt > INT64_MAX ) {
    return INT64_MAX;
  }

  return (int64_t) sbt;
}

rtems_counter_ticks rtems_counter_sbintime_to_ticks( int64_t sbt )
{
  uint64_t value;
  uint64_t frequency;
  uint64_t ticks;

  if ( sbt < 0 ) {
    return 0;
  }

  value = (uint64_t) sbt;
  frequency = _Counter_Converter.from_sbt_scaler;

  /* The high partial product is less than 2**63 */
  ticks = ( value >> 32 ) * frequency +
          ( ( ( value & UINT32_MAX ) * frequency ) >> 32 );

  if ( ticks > UINT32_MAX ) {
    return UINT32_MAX;
  }

  return (rtems_counter_ticks) ticks;
}

void rtems_counter_initialize_converter( uint32_t frequency )
{
  if ( frequency == 0 ) {
    return;
  }

  _Counter_Converter.to_ns_scaler = COUNTER_TO_NS_SCALER( frequency );
  _Counter_Converter.from_ns_scaler = COUNTER_FROM_NS_SCALER( frequency );
  _Counter_Converter.to_sbt_scaler = COUNTER_TO_SBT_SCALER( frequency );
  _Counter_Converter.from_sbt_scaler = frequency;
}

/**
 * @brief Initializes the counter value converter with the counter frequency.
 */
static void _Counter_Initialize_converter( void )
{
  rtems_counter_initialize_converter( rtems_counter_frequency() );
}

RTEMS_SYSINIT_ITEM(
  _Counter_Initialize_converter,
  RTEMS_SYSINIT_CPU_COUNTER,
  RTEMS_SYSINIT_ORDER_LAST_BUT_5
);
