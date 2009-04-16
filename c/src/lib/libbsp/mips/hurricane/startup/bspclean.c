/*  bsp_cleanup()
 *
 *  This routine normally is part of start.s and usually returns
 *  control to a monitor.
 *
 *  INPUT:  NONE
 *
 *  OUTPUT: NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspclean.c,v 1.2 2006/03/17 10:34:12 ralf Exp $
 */

#include <rtems.h>
#include <bsp.h>

void idtsim__exit();

void bsp_cleanup( void )
{
  idtsim__exit();
}
