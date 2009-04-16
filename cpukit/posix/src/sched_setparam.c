/*
 *  13.3.1 Set Scheduling Parameters, P1003.1b-1993, p. 252
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: sched_setparam.c,v 1.1 2007/12/13 16:52:21 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <sched.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/score/tod.h>
#include <rtems/score/thread.h>
#include <rtems/seterr.h>
#include <rtems/posix/priority.h>
#include <rtems/posix/time.h>

int sched_setparam(
  pid_t                     pid,
  const struct sched_param *param
)
{
  rtems_set_errno_and_return_minus_one( ENOSYS );
}
