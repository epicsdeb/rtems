/*
 *  MCF5206eLITE bsp_cleanup
 *
 *  This routine returns control from RTEMS to the dBUG monitor.
 *
 *  Copyright (C) 2000 OKTET Ltd., St.-Petersburg, Russia.
 *  Author: Victor V. Vengerov <vvv@oktet.ru>
 *
 *  COPYRIGHT (c) 1989-1998.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspclean.c,v 1.4 2004/04/21 10:42:52 ralf Exp $
 */

#include <rtems.h>
#include <bsp.h>

/* bsp_cleanup --
 *     This function called when RTEMS execution is finished. bsp_cleanup
 *     for MCF5206eLITE evaluation board throw execution to the dBUG
 *     monitor.
 *
 * PARAMETERS:
 *     none
 *
 * RETURNS:
 *     none
 */
void
bsp_cleanup(void)
{
}
