/*  Task_1_through_3
 *
 *  This routine serves as a test task.  It verifies the basic task
 *  switching capabilities of the executive.
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
 *  $Id: task1.c,v 1.8 2003/09/04 18:53:47 joel Exp $
 */

#include "system.h"

rtems_task Task_1_through_3(
  rtems_task_argument argument
)
{
  rtems_id          tid;
  rtems_time_of_day time;
  rtems_status_code status;

  status = rtems_task_ident( RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &tid );
  directive_failed( status, "rtems_task_ident" );

  while( FOREVER ) {
    status = rtems_clock_get( RTEMS_CLOCK_GET_TOD, &time );
    directive_failed( status, "rtems_clock_get" );

    if ( time.second >= 35 ) {
      puts( "*** END OF TEST 1 ***" );
      rtems_test_exit( 0 );
    }

    put_name( Task_name[ task_number( tid ) ], FALSE );
    print_time( " - rtems_clock_get - ", &time, "\n" );

    status = rtems_task_wake_after( task_number( tid ) * 5 * TICKS_PER_SECOND );
    directive_failed( status, "rtems_task_wake_after" );
  }
}
