/*
 *  times() - POSIX 1003.1b 4.5.2 - Get Process Times
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: __times.c,v 1.17 2008/06/06 15:44:11 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>

#include <sys/times.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <assert.h>
#ifdef RTEMS_ENABLE_NANOSECOND_CPU_USAGE_STATISTICS
  #include <rtems/score/timespec.h>
#endif

clock_t _times(
   struct tms  *ptms
)
{
  rtems_interval ticks;

  if ( !ptms ) {
    errno = EFAULT;
    return -1;
  }

  /*
   *  This call does not depend on TOD being initialized and can't fail.
   */

  (void) rtems_clock_get( RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &ticks );

  /*
   *  RTEMS technically has no notion of system versus user time
   *  since there is no separation of OS from application tasks.
   *  But we can at least make a distinction between the number
   *  of ticks since boot and the number of ticks executed by this
   *  this thread.
   */

  #ifdef RTEMS_ENABLE_NANOSECOND_CPU_USAGE_STATISTICS
    {
      struct timespec per_tick;
      uint32_t ticks;
      uint32_t fractional_ticks;
      
      per_tick.tv_sec =
        _TOD_Microseconds_per_tick / TOD_MILLISECONDS_PER_SECOND;
      per_tick.tv_nsec =
        (_TOD_Microseconds_per_tick % TOD_MILLISECONDS_PER_SECOND) / 1000;

      _Timespec_Divide(
        &_Thread_Executing->cpu_time_used,
        &per_tick,
        &ticks,
        &fractional_ticks
      );
      ptms->tms_utime = ticks;
    }
  #else
    ptms->tms_utime  = _Thread_Executing->cpu_time_used;
  #endif
  ptms->tms_stime  = ticks;
  ptms->tms_cutime = 0;
  ptms->tms_cstime = 0;

  return ticks;
}

/*
 *  times()
 *
 *  times() system call wrapper for _times() above.
 */

clock_t times(
   struct tms  *ptms
)
{
  return _times( ptms );
}

/*
 *  _times_r
 *
 *  This is the Newlib dependent reentrant version of times().
 */

#if defined(RTEMS_NEWLIB)

#include <reent.h>

clock_t _times_r(
   struct _reent *ptr,
   struct tms  *ptms
)
{
  return _times( ptms );
}
#endif
