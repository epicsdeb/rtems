/** 
 *  @file  score/src/timespectoticks.c
 */

/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: timespectoticks.c,v 1.1 2007/04/05 21:17:27 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/timespec.h>
#include <sys/types.h>
#include <rtems/score/tod.h>
#include <rtems/score/watchdog.h>

/**
 *
 *  This routines converts a timespec to the corresponding number of ticks.
 */

uint32_t _Timespec_To_ticks(
  const struct timespec *time
)
{
  uint32_t  ticks;

  if ( (time->tv_sec == 0) && (time->tv_nsec == 0) ) 
    return 0;

  ticks  = time->tv_sec * TOD_TICKS_PER_SECOND;

  ticks += (time->tv_nsec / TOD_NANOSECONDS_PER_MICROSECOND) /
             _TOD_Microseconds_per_tick;

  if (ticks)
    return ticks;

  return 1;
}
