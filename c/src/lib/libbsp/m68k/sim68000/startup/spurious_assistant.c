/*
 *  Spurious Trap Handler Assistant
 *
 *  COPYRIGHT (c) 1989-2000.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: spurious_assistant.c,v 1.3 2003/09/04 18:52:10 joel Exp $
 */

#include <bsp.h>
#include <rtems/bspIo.h>

/*
 *  bsp_spurious_handler_assistant
 *
 *  We can't recover so just return to gdb.
 */

void bsp_spurious_handler_assistant(
  rtems_vector_number  vector/* ,
  CPU_Interrupt_frame *isf */
)
{
  /* XXX do something here */
}
