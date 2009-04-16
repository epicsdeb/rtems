/*  Task_2
 *
 *  This routine serves as a test task.  It simply runs forever so
 *  another task can restart it.
 *
 *  Input parameters:
 *    argument - task argument
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: task2.c,v 1.8 2003/09/04 18:53:47 joel Exp $
 */

#include "system.h"

rtems_task Task_2( argument )
rtems_task_argument argument;
{
  rtems_status_code status;

  puts( "TA2 - is beginning to run" );
  if ( argument == Argument ) {
    status = rtems_task_wake_after( 2 * TICKS_PER_SECOND );
    directive_failed( status, "rtems_task_wake_after" );
  }

  while( FOREVER ) {
    puts( "TA2 - rtems_task_wake_after - sleep 1/2 second" );
    status = rtems_task_wake_after( TICKS_PER_SECOND / 2 );
    directive_failed( status, "rtems_task_wake_after" );
  }
}
