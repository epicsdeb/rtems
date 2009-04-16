/** 
 *  @file  score/src/timespecgreaterthan.c
 */

/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: timespecgreaterthan.c,v 1.2 2008/09/04 17:39:56 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>

#include <rtems/system.h>
#include <rtems/score/timespec.h>
#include <rtems/score/tod.h>

bool _Timespec_Greater_than(
  const struct timespec *lhs,
  const struct timespec *rhs
)
{
  if ( lhs->tv_sec > rhs->tv_sec )
    return TRUE;

  if ( lhs->tv_sec < rhs->tv_sec )
    return FALSE;

  /* ASSERT: lhs->tv_sec == rhs->tv_sec */
  if ( lhs->tv_nsec > rhs->tv_nsec )
    return TRUE; 

  return FALSE;
}
