/*
 *  Classic API Signal to Task from ISR
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: init.c,v 1.6 2008/09/06 03:28:07 ralf Exp $
 */

#define CONFIGURE_INIT
#include "system.h"

volatile bool case_hit;

rtems_id main_task;
rtems_id other_task;

rtems_timer_service_routine test_event_from_isr(
  rtems_id  timer,
  void     *arg
)
{
  rtems_status_code     status;

  if ( _Event_Sync_state == THREAD_BLOCKING_OPERATION_NOTHING_HAPPENED ) {
    /*
     *  This event send hits the critical section but sends to
     *  another task so doesn't impact this critical section.
     */
    rtems_event_send( other_task, 0x02 );

    /*
     *  This event send hits the main task but doesn't satisfy
     *  it's blocking condition so it will still block
     */
    rtems_event_send( main_task, 0x02 );

    case_hit = TRUE;
  }
  status = rtems_event_send( main_task, 0x01 );
}

rtems_timer_service_routine test_event_with_timeout_from_isr(
  rtems_id  timer,
  void     *arg
)
{
  rtems_status_code     status;

  if ( _Event_Sync_state == THREAD_BLOCKING_OPERATION_NOTHING_HAPPENED ) {
    /*
     *  We want to catch the task while it is blocking.  Otherwise
     *  just send and make it happy.
     */
    case_hit = TRUE;
  }
  status = rtems_event_send( main_task, 0x01 );
}

rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_status_code     status;
  rtems_id              timer;
  rtems_event_set       out;
  int                   i;
  int                   max;
  int                   iterations = 0;

  puts( "\n\n*** TEST 39 ***" );

  main_task = rtems_task_self();

  /*
   *  Timer used in multiple ways
   */
  status = rtems_timer_create( 1, &timer );
  directive_failed( status, "rtems_timer_create" );

  status = rtems_task_create(
     0xa5a5a5a5,
     1,
     RTEMS_MINIMUM_STACK_SIZE,
     RTEMS_DEFAULT_MODES,
     RTEMS_DEFAULT_ATTRIBUTES,
     &other_task
  );
  directive_failed( status, "rtems_task_create" );

  /*
   * Test Event send successful from ISR -- receive is forever
   */
  case_hit = FALSE;
  iterations = 0;
  max = 1;

  while (1) {
    if ( case_hit )
      break;
    status = rtems_timer_fire_after( timer, 1, test_event_from_isr, NULL );
    directive_failed( status, "timer_fire_after failed" );

    for (i=0 ; i<max ; i++ )
      if ( _Event_Sync_state == THREAD_BLOCKING_OPERATION_SATISFIED )
        break;

    status = rtems_event_receive( 0x01, RTEMS_DEFAULT_OPTIONS, 0, &out );
    directive_failed( status, "rtems_event_receive" );
    if ( case_hit == TRUE )
      break;
    max += 2;

    /* with our clock tick, this is about 30 seconds */
    if ( ++iterations >= 4 * 1000 * 30)
      break;
  }

   printf(
     "Event sent from ISR hitting synchronization point has %soccurred\n",
     (( case_hit == TRUE ) ? "" : "NOT ")
  ); 

  /*
   * Test Event send successful from ISR -- receive has timeout
   */
  case_hit = FALSE;
  iterations = 0;
  max = 1;

  while (1) {
    if ( case_hit )
      break;
    status = rtems_timer_fire_after(
      timer, 1, test_event_with_timeout_from_isr, NULL );
    directive_failed( status, "timer_fire_after failed" );

    for (i=0 ; i<max ; i++ )
      if ( _Event_Sync_state == THREAD_BLOCKING_OPERATION_SATISFIED )
        break;

    status = rtems_event_receive( 0x01, RTEMS_DEFAULT_OPTIONS, 10, &out );
    directive_failed( status, "rtems_event_receive" );
    if ( case_hit == TRUE )
      break;
    max += 2;

    /* with our clock tick, this is about 30 seconds */
    if ( ++iterations >= 4 * 1000 * 30)
      break;
  }

   printf(
     "Event sent from ISR (with timeout) hitting synchronization "
       "point has %soccurred\n",
     (( case_hit == TRUE ) ? "" : "NOT ")
  ); 

  /*
   *  Now try for a timeout case
   */
  iterations = 0;
  case_hit = FALSE;
  max = 1;

  puts(
    "Run multiple times in attempt to hit event timeout synchronization point"
  ); 
  while (1) {

    for (i=0 ; i<max ; i++ )
      if ( _Event_Sync_state == THREAD_BLOCKING_OPERATION_SATISFIED )
        break;

    status = rtems_event_receive( 0x01, RTEMS_DEFAULT_OPTIONS, 1, &out );
    fatal_directive_status( status, RTEMS_TIMEOUT, "event_receive timeout" );

    if ( ++max > 10240 )
      max = 0;

    /* with our clock tick, this is about 30 seconds */
    if ( ++iterations >= 4 * 1000 * 30)
      break;

  }

  puts( "*** END OF TEST 39 ***" );
  rtems_test_exit( 0 );
}
