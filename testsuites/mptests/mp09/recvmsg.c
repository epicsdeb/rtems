/*  Receive_messages
 *
 *  This routine receives and prints three messages.
 *  an error condition.
 *
 *  Input parameters:  NONE
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
 *  $Id: recvmsg.c,v 1.12 2008/09/08 15:25:08 joel Exp $
 */

#include "system.h"

void Receive_messages()
{
 rtems_status_code status;
 uint32_t          index;
 size_t            size;
 char              receive_buffer[16];

 for ( index=1 ; index <=3 ; index++ ) {
   puts( "Receiving message ..." );
   status = rtems_message_queue_receive(
     Queue_id[ 1 ],
     receive_buffer,
     &size,
     RTEMS_DEFAULT_OPTIONS,
     RTEMS_NO_TIMEOUT
   );
   directive_failed( status, "rtems_message_queue_receive" );
   puts_nocr( "Received : ");
   puts( receive_buffer );
 }

  puts( "Receiver delaying for a second" );
  status = rtems_task_wake_after( TICKS_PER_SECOND );
  directive_failed( status, "rtems_task_wake_after" );
}
