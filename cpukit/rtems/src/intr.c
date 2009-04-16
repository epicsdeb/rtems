/*
 *  Interrupt Manager
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: intr.c,v 1.14 2005/01/18 09:03:43 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/score/isr.h>
#include <rtems/rtems/intr.h>

/*  _Interrupt_Manager_initialization
 *
 *  This routine initializes the interrupt manager.
 *
 *  Input parameters: NONE
 *
 *  Output parameters: NONE
 */

void _Interrupt_Manager_initialization( void )
{
}
