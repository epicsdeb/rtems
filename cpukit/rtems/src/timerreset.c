/*
 *  Timer Manager - rtems_timer_reset directive
 *
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: timerreset.c,v 1.8.2.1 2008/12/03 21:01:08 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/score/object.h>
#include <rtems/score/thread.h>
#include <rtems/rtems/timer.h>
#include <rtems/score/tod.h>
#include <rtems/score/watchdog.h>

/*PAGE
 *
 *  rtems_timer_reset
 *
 *  This directive allows a thread to reset a timer.
 *
 *  Input parameters:
 *    id - timer id
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code       - if unsuccessful
 */

rtems_status_code rtems_timer_reset(
  Objects_Id id
)
{
  Timer_Control     *the_timer;
  Objects_Locations  location;

  the_timer = _Timer_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      switch ( the_timer->the_class ) {
        case TIMER_INTERVAL:
          _Watchdog_Remove( &the_timer->Ticker );
          _Watchdog_Insert( &_Watchdog_Ticks_chain, &the_timer->Ticker );
          break;
        case TIMER_INTERVAL_ON_TASK:
          if ( !_Timer_Server_schedule_operation ) {
            _Thread_Enable_dispatch();
            return RTEMS_INCORRECT_STATE;
          }
          _Watchdog_Remove( &the_timer->Ticker );
          (*_Timer_Server_schedule_operation)( the_timer );
          break;
        case TIMER_TIME_OF_DAY:
        case TIMER_TIME_OF_DAY_ON_TASK:
        case TIMER_DORMANT:
          _Thread_Enable_dispatch();
          return RTEMS_NOT_DEFINED;
      }
      _Thread_Enable_dispatch();
      return RTEMS_SUCCESSFUL;

#if defined(RTEMS_MULTIPROCESSING)
    case OBJECTS_REMOTE:            /* should never return this */
#endif
    case OBJECTS_ERROR:
      break;
  }

  return RTEMS_INVALID_ID;
}
