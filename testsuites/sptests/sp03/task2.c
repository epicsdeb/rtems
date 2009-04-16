/*  Task_2
 *
 *  This routine serves as a test task.  It repeatedly gets the system time
 *  and date, converts it to printable form, and print it.
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
 *  $Id: task2.c,v 1.9 2003/09/04 18:53:47 joel Exp $
 */

#include "system.h"

rtems_task Task_2(
  rtems_task_argument argument
)
{
  rtems_status_code status;
  rtems_time_of_day time;

  while( FOREVER ) {
    status = rtems_task_wake_after( 1*TICKS_PER_SECOND );
    directive_failed( status, "rtems_task_wake_after" );

    status = rtems_clock_get( RTEMS_CLOCK_GET_TOD, &time );
    directive_failed( status, "rtems_clock_get" );

    if ( time.second >= 17 ) {
      puts( "*** END OF TEST 3 *** " );
      rtems_test_exit( 0 );
    }

    put_name( Task_name[ 2 ], FALSE );
    print_time( "  ", &time, "\n" );
  }
}
