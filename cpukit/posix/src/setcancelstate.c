/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: setcancelstate.c,v 1.8 2008/09/04 15:23:12 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/score/chain.h>
#include <rtems/score/isr.h>
#include <rtems/score/thread.h>
#include <rtems/score/wkspace.h>
#include <rtems/posix/cancel.h>
#include <rtems/posix/pthread.h>
#include <rtems/posix/threadsup.h>

/*PAGE
 *
 *  18.2.2 Setting Cancelability State, P1003.1c/Draft 10, p. 183
 */

int pthread_setcancelstate(
  int  state,
  int *oldstate
)
{
  POSIX_API_Control *thread_support;
  bool               cancel = false;

  /*
   *  Don't even think about deleting a resource from an ISR.
   *  Besides this request is supposed to be for _Thread_Executing
   *  and the ISR context is not a thread.
   */

  if ( _ISR_Is_in_progress() )
    return EPROTO;

  if ( !oldstate )
    return EINVAL;

  if ( state != PTHREAD_CANCEL_ENABLE && state != PTHREAD_CANCEL_DISABLE )
    return EINVAL;

  thread_support = _Thread_Executing->API_Extensions[ THREAD_API_POSIX ];

  _Thread_Disable_dispatch();
    *oldstate = thread_support->cancelability_state;
    thread_support->cancelability_state = state;

    if ( thread_support->cancelability_state == PTHREAD_CANCEL_ENABLE &&
         thread_support->cancelability_type == PTHREAD_CANCEL_ASYNCHRONOUS &&
         thread_support->cancelation_requested )
      cancel = true;

 _Thread_Enable_dispatch();
 if ( cancel )
   _POSIX_Thread_Exit( _Thread_Executing, PTHREAD_CANCELED );
  return 0;
}
