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
 *  $Id: bspclean.c,v 1.3 2003/09/04 18:51:51 joel Exp $
 */

#include <rtems.h>
#include <bsp.h>

void bsp_cleanup( void )
{
	/* Cause double bus fault to force reset? */
}
