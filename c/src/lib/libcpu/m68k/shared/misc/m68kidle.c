/*
 *  Motorola MC68xxx Dependent Idle Body Source
 *
 *  COPYRIGHT (c) 1989-2002.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: m68kidle.c,v 1.3 2008/07/31 15:00:11 joel Exp $
 */

#include <rtems/system.h>
#include <rtems/score/thread.h>


/*PAGE
 *
 *  _CPU_Thread_Idle_body
 *
 *  This kernel routine is the idle thread.  The idle thread runs any time
 *  no other thread is ready to run.  This thread loops forever with
 *  interrupts enabled.
 *
 *  Input parameters:
 *    ignored - this parameter is ignored
 *
 *  Output parameters:  NONE
 */
 
void *_CPU_Thread_Idle_body( uint32_t ignored )
{
#if defined(mcf5272)
  for( ; ; ) {
    asm volatile( "nop" );
    asm volatile( "nop" );
  }
#else
  for( ; ; )
    asm volatile( "stop #0x3000" );  /* supervisor mode, all interrupts on */
#endif
}
