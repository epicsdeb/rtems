/*
 *  This routine starts the application.  It includes application,
 *  board, and monitor specific initialization and configuration.
 *  The generic CPU dependent initialization has been performed
 *  before this routine is invoked.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspstart.c,v 1.14 2008/05/15 15:55:16 joel Exp $
 */

#include <string.h>

#include <bsp.h>
#include <rtems/libio.h>
#include <rtems/libcsupport.h>

/*
 *  Use the shared implementations of the following routines
 */

void bsp_libc_init( void *, uint32_t, int );

/*
 *  Function:   bsp_pretasking_hook
 *  Created:    95/03/10
 *
 *  Description:
 *      BSP pretasking hook.  Called just before drivers are initialized.
 *      Used to setup libc and install any BSP extensions.
 *
 *  NOTES:
 *      Must not use libc (to do io) from here, since drivers are
 *      not yet initialized.
 *
 */

void bsp_pretasking_hook(void)
{
    void         *heapStart;
    unsigned long heapSize;
    extern int WorkspaceBase;

    heapStart =  (void *)
       ((unsigned long)&WorkspaceBase + rtems_configuration_get_work_space_size());
    if ( (unsigned long) heapStart > (256 * 1024) )
       rtems_fatal_error_occurred (('H'<<24) | ('E'<<16) | ('A'<<8) | 'P');
    heapSize = (256 * 1024) - (unsigned long)(heapStart);
    bsp_libc_init(heapStart, heapSize, 0);
}

/*
 *  bsp_start
 *
 *  This routine does the bulk of the system initialization.
 */

void bsp_start( void )
{
  extern int WorkspaceBase;

/*
  if ( rtems_configuration_get_work_space_size() >(512*1024) )
   _sys_exit( 1 );
*/

  Configuration.work_space_start = (void *) &WorkspaceBase;
}

void H8BD_Install_IRQ(
  uint32_t  	vector,
  proc_ptr	new_handler,
  proc_ptr	*old_handler )
{ /* empty */
}
