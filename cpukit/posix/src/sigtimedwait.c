/*
 *  3.3.8 Synchronously Accept a Signal, P1003.1b-1993, p. 76
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: sigtimedwait.c,v 1.15 2008/09/04 15:23:12 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>
#include <signal.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/posix/pthread.h>
#include <rtems/posix/psignal.h>
#include <rtems/seterr.h>
#include <rtems/posix/time.h>
#include <rtems/score/isr.h>

int _POSIX_signals_Get_highest(
  sigset_t   set
)
{
  int signo;

  for ( signo = SIGRTMIN ; signo <= SIGRTMAX ; signo++ ) {
    if ( set & signo_to_mask( signo ) )
      return signo;
  }

/* XXX - add __SIGFIRSTNOTRT or something like that to newlib signal .h */

  for ( signo = SIGHUP ; signo <= __SIGLASTNOTRT ; signo++ ) {
    if ( set & signo_to_mask( signo ) )
      return signo;
  }

  return 0;
}

int sigtimedwait(
  const sigset_t         *set,
  siginfo_t              *info,
  const struct timespec  *timeout
)
{
  Thread_Control    *the_thread;
  POSIX_API_Control *api;
  Watchdog_Interval  interval;
  siginfo_t          signal_information;
  siginfo_t         *the_info;
  int                signo;
  ISR_Level          level;

  /*
   *  Error check parameters before disabling interrupts.
   *
   *  NOTE: This is very specifically a RELATIVE not ABSOLUTE time
   *        in the Open Group specification.
   */

  interval = 0;
  if ( timeout ) {

    if ( !_Timespec_Is_valid( timeout ) )
      rtems_set_errno_and_return_minus_one( EINVAL );

    interval = _Timespec_To_ticks( timeout );

    if ( !interval )
      rtems_set_errno_and_return_minus_one( EINVAL );
  }

  /*
   *  Initialize local variables.
   */

  the_info = ( info ) ? info : &signal_information;

  the_thread = _Thread_Executing;

  api = the_thread->API_Extensions[ THREAD_API_POSIX ];

  /*
   *  What if they are already pending?
   */

  /* API signals pending? */

  _ISR_Disable( level );
  if ( *set & api->signals_pending ) {
    /* XXX real info later */
    the_info->si_signo = _POSIX_signals_Get_highest( api->signals_pending );
    _POSIX_signals_Clear_signals(
      api,
      the_info->si_signo,
      the_info,
      false,
      false
    );
    _ISR_Enable( level );

    the_info->si_code = SI_USER;
    the_info->si_value.sival_int = 0;
    return the_info->si_signo;
  }

  /* Process pending signals? */

  if ( *set & _POSIX_signals_Pending ) {
    signo = _POSIX_signals_Get_highest( _POSIX_signals_Pending );
    _POSIX_signals_Clear_signals( api, signo, the_info, true, false );
    _ISR_Enable( level );

    the_info->si_signo = signo;
    the_info->si_code = SI_USER;
    the_info->si_value.sival_int = 0;
    return signo;
  }

  the_info->si_signo = -1;

  _Thread_Disable_dispatch();
    the_thread->Wait.queue           = &_POSIX_signals_Wait_queue;
    the_thread->Wait.return_code     = EINTR;
    the_thread->Wait.option          = *set;
    the_thread->Wait.return_argument = the_info;
    _Thread_queue_Enter_critical_section( &_POSIX_signals_Wait_queue );
    _ISR_Enable( level );
    _Thread_queue_Enqueue( &_POSIX_signals_Wait_queue, interval );
  _Thread_Enable_dispatch();

  /*
   * When the thread is set free by a signal, it is need to eliminate
   * the signal.
   */

  _POSIX_signals_Clear_signals( api, the_info->si_signo, the_info, false, false );
  errno = _Thread_Executing->Wait.return_code;
  return the_info->si_signo;
}
