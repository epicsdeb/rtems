/*
 *  Mutex Timed Lock
 */

/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: mutextimedlock.c,v 1.7 2008/09/04 15:23:12 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <pthread.h>

#include <rtems/system.h>
#include <rtems/score/coremutex.h>
#include <rtems/score/watchdog.h>
#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/score/mpci.h>
#endif
#include <rtems/posix/mutex.h>
#include <rtems/posix/priority.h>
#include <rtems/posix/time.h>

/*PAGE
 *
 *  11.3.3 Locking and Unlocking a Mutex, P1003.1c/Draft 10, p. 93
 *
 *  NOTE: P1003.4b/D8 adds pthread_mutex_timedlock(), p. 29
 */

int pthread_mutex_timedlock(
  pthread_mutex_t       *mutex,
  const struct timespec *abstime
)
{
  Watchdog_Interval                            ticks;
  bool                                         do_wait;
  POSIX_Absolute_timeout_conversion_results_t  status;
  int                                          lock_status;

  /*
   *  POSIX requires that blocking calls with timeouts that take
   *  an absolute timeout must ignore issues with the absolute
   *  time provided if the operation would otherwise succeed.
   *  So we check the abstime provided, and hold on to whether it
   *  is valid or not.  If it isn't correct and in the future,
   *  then we do a polling operation and convert the UNSATISFIED
   *  status into the appropriate error.
   */
  status = _POSIX_Absolute_timeout_to_ticks( abstime, &ticks );
  switch ( status ) {
    case POSIX_ABSOLUTE_TIMEOUT_INVALID:
    case POSIX_ABSOLUTE_TIMEOUT_IS_IN_PAST:
    case POSIX_ABSOLUTE_TIMEOUT_IS_NOW:
      do_wait = FALSE;
      break;
    case POSIX_ABSOLUTE_TIMEOUT_IS_IN_FUTURE:
      do_wait = TRUE;
      break;
  }

  lock_status = _POSIX_Mutex_Lock_support(
    mutex,
    do_wait,
    ticks
  );

  /*
   *  This service only gives us the option to block.  We used a polling
   *  attempt to lock if the abstime was not in the future.  If we did
   *  not obtain the mutex, then not look at the status immediately,
   *  make sure the right reason is returned.
   */
  if ( !do_wait && (lock_status == EBUSY) ) {
    switch (lock_status) {
      case POSIX_ABSOLUTE_TIMEOUT_INVALID:
        return EINVAL;
      case POSIX_ABSOLUTE_TIMEOUT_IS_IN_PAST:
      case POSIX_ABSOLUTE_TIMEOUT_IS_NOW:
        return ETIMEDOUT;
      case POSIX_ABSOLUTE_TIMEOUT_IS_IN_FUTURE:
        break;
    }
  }

  return lock_status;
}
