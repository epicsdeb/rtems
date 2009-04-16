/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: clockgettime.c,v 1.12 2008/02/01 00:44:15 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <time.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/score/isr.h>
#include <rtems/score/thread.h>
#include <rtems/score/tod.h>

#include <rtems/seterr.h>

/*PAGE
 *
 *  14.2.1 Clocks, P1003.1b-1993, p. 263
 */

int clock_gettime(
  clockid_t        clock_id,
  struct timespec *tp
)
{
  if ( !tp )
    rtems_set_errno_and_return_minus_one( EINVAL );

  if ( clock_id == CLOCK_REALTIME ) 
    _TOD_Get(tp);
#ifdef CLOCK_MONOTONIC
  else if ( clock_id == CLOCK_MONOTONIC )
    _TOD_Get_uptime(tp);
#endif
#ifdef _POSIX_CPUTIME
  else if ( clock_id == CLOCK_PROCESS_CPUTIME )
    _TOD_Get_uptime(tp);
#endif
#ifdef _POSIX_THREAD_CPUTIME
  else if ( clock_id == CLOCK_THREAD_CPUTIME )
    rtems_set_errno_and_return_minus_one( ENOSYS );
#endif
  else
    rtems_set_errno_and_return_minus_one( EINVAL );

  return 0;
}
