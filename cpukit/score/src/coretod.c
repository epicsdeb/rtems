/*
 *  Time of Day (TOD) Handler
 */

/*  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: coretod.c,v 1.20 2007/04/05 21:17:27 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/score/thread.h>
#include <rtems/score/tod.h>
#include <rtems/score/watchdog.h>

/*PAGE
 *
 *  _TOD_Handler_initialization
 *
 *  This routine initializes the time of day handler.
 *
 *  Input parameters:
 *    microseconds_per_tick - microseconds between clock ticks
 *
 *  Output parameters: NONE
 */

void _TOD_Handler_initialization(
  uint32_t   microseconds_per_tick
)
{
  _TOD_Microseconds_per_tick = microseconds_per_tick;

  /* POSIX format TOD (timespec) */
  _TOD_Now.tv_sec  = TOD_SECONDS_1970_THROUGH_1988;
  _TOD_Now.tv_nsec = 0;

  /* Uptime (timespec) */
  _TOD_Uptime.tv_sec  = 0;
  _TOD_Uptime.tv_nsec = 0;

  /* TOD has not been set */
  _TOD_Is_set = FALSE;
  _TOD_Activate();
}
