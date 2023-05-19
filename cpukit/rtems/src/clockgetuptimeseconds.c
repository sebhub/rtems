/**
 * @file
 *
 * @ingroup RTEMSImplClassicClock
 *
 * @brief This source file contains the implementation of
 *   rtems_clock_get_uptime_seconds().
 */

/*
 * Copyright (c) 2018 embedded brains GmbH & Co. KG
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/rtems/clock.h>
#include <rtems/score/timecounter.h>

time_t rtems_clock_get_uptime_seconds( void )
{
  return _Timecounter_Time_uptime - 1;
}
