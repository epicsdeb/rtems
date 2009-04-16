/*
 *  13.3.3 Set Scheduling Policy and Scheduling Parameters,
 *         P1003.1b-1993, p. 254
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: sched_setscheduler.c,v 1.1 2007/12/13 16:52:21 joel Exp $
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

int sched_setscheduler(
  pid_t                     pid,
  int                       policy,
  const struct sched_param *param
)
{
  rtems_set_errno_and_return_minus_one( ENOSYS );
}
