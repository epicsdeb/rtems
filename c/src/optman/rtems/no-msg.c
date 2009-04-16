/*
 *  Message Queue Manager
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: no-msg.c,v 1.17 2007/05/09 18:44:20 joel Exp $
 */

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/attr.h>
#include <rtems/score/chain.h>
#include <rtems/score/isr.h>
#include <rtems/rtems/message.h>
#include <rtems/score/object.h>
#include <rtems/rtems/options.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/interr.h>

void _Message_queue_Manager_initialization(
  uint32_t   maximum_message_queues
)
{
}
