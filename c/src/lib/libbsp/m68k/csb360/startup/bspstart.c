/*
 *  BSP startup
 *
 *  This routine starts the application.  It includes application,
 *  board, and monitor specific initialization and configuration.
 *  The generic CPU dependent initialization has been performed
 *  before this routine is invoked.
 *
 *  Copyright (C) 2000 OKTET Ltd., St.-Petersburg, Russia
 *  Author: Victor V. Vengerov <vvv@oktet.ru>
 *
 *  Based on work:
 *  Author:
 *    David Fiddes, D.J@fiddes.surfaid.org
 *    http://www.calm.hw.ac.uk/davidf/coldfire/
 *
 *  COPYRIGHT (c) 1989-1998.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *
 *  http://www.rtems.com/license/LICENSE.
 * 
 *  $Id: bspstart.c,v 1.10 2008/05/12 18:43:21 joel Exp $
 */

#include <bsp.h>
#include <rtems/libio.h>
 
#include <rtems/libcsupport.h>
 
#include <string.h>

extern unsigned int _RamSize;

/*
 *  Use the shared implementations of the following routines
 */
void bsp_libc_init( void *, uint32_t, int );
void bsp_pretasking_hook(void);               /* m68k version */

/* bsp_start --
 *     This routine does the bulk of the system initialisation.
 */
void bsp_start( void )
{
    extern void *_WorkspaceBase;
    extern unsigned long _M68k_Ramsize;
    _M68k_Ramsize = (unsigned long)&_RamSize;   /* size set in linker script */

    /*
     *  Need to "allocate" the memory for the RTEMS Workspace and
     *  tell the RTEMS configuration where it is.  This memory is
     *  not malloc'ed.  It is just "pulled from the air".
     */

    Configuration.work_space_start = (void *)&_WorkspaceBase;
}
