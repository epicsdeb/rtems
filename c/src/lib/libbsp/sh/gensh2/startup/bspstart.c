/*
 *  This routine starts the application.  It includes application,
 *  board, and monitor specific initialization and configuration.
 *  The generic CPU dependent initialization has been performed
 *  before this routine is invoked.
 *
 *  Authors: Ralf Corsepius (corsepiu@faw.uni-ulm.de) and
 *           Bernd Becker (becker@faw.uni-ulm.de)
 *
 *  COPYRIGHT (c) 1997-1998, FAW Ulm, Germany
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 *  COPYRIGHT (c) 1998.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspstart.c,v 1.17 2008/05/15 15:53:30 joel Exp $
 */

#include <string.h>

#include <bsp.h>
#include <rtems/libio.h>
#include <rtems/libcsupport.h>

uint32_t bsp_clicks_per_second;

extern void bsp_hw_init(void);

/*
 *  Use the shared implementations of the following routines
 */

void bsp_libc_init( void *, uint32_t, int );

/*
 *  Function:   bsp_pretasking_hook
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
    bsp_libc_init(&HeapStart, (char *)&HeapEnd - (char *)&HeapStart, 0);
}

/*
 *  bsp_start
 *
 *  This routine does the bulk of the system initialization.
 */

void bsp_start(void)
{
  /*
     For real boards you need to setup the hardware
     and need to copy the vector table from rom to ram.

     Depending on the board this can ether be done from inside the rom
     startup code, rtems startup code or here.
   */

#ifndef START_HW_INIT
  /* board hardware setup here, or from 'start.S' */
  bsp_hw_init();
#endif

  /*
   *  Allocate the memory for the RTEMS Work Space.  This can come from
   *  a variety of places: hard coded address, malloc'ed from outside
   *  RTEMS world (e.g. simulator or primitive memory manager), or (as
   *  typically done by stock BSPs) by subtracting the required amount
   *  of work space from the last physical address on the CPU board.
   */

  /*
   *  Need to "allocate" the memory for the RTEMS Workspace and
   *  tell the RTEMS configuration where it is.  This memory is
   *  not malloc'ed.  It is just "pulled from the air".
   */

  Configuration.work_space_start = (void *) &WorkSpaceStart ;
  rtems_configuration_get_work_space_size()  =
    &WorkSpaceEnd - &WorkSpaceStart ;

  /*
   *  initialize the CPU table for this BSP
   */

#if ( CPU_ALLOCATE_INTERRUPT_STACK == FALSE )
  _CPU_Interrupt_stack_low = &CPU_Interrupt_stack_low ;
  _CPU_Interrupt_stack_high = &CPU_Interrupt_stack_high ;
#endif

  /*
   *  initialize the device driver parameters
   */
  bsp_clicks_per_second = CPU_CLOCK_RATE_HZ;
}
