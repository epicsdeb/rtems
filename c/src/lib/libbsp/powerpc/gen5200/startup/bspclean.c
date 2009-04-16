/*
 *  This is a dummy bsp_cleanup routine.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspclean.c,v 1.8 2008/09/03 15:39:03 thomas Exp $
 */

#include <rtems.h>
#include <rtems/bspIo.h>
#include <bsp.h>
#include <bsp/mpc5200.h>
#include <bsp/bootcard.h>

extern int mpc5200_uart_pollRead(int minor);

void bsp_cleanup( void )
{
  
  #if (BENCHMARK_IRQ_PROCESSING == 1)
  {
    BSP_IRQ_Benchmarking_Report();
  }
  #endif
 
  #if defined(BSP_PRESS_KEY_FOR_RESET)
    printk( "\nEXECUTIVE SHUTDOWN! Any key to reboot..." );

    /*
     * Wait for a key to be pressed
     */
    while ( mpc5200_uart_pollRead(0) == -1 ) ;
  #endif

  /*
   *  Check both conditions -- if you want to ask for reboot, then
   *  you must have meant to reset the board.
   */
  #if defined(BSP_PRESS_KEY_FOR_RESET) || defined(BSP_RESET_BOARD_AT_EXIT)
    /*
     * Now reset the CPU
     */
    _ISR_Set_level( 0 );

    mpc5200.gpt[0].count_in = 0xf;
    mpc5200.gpt[0].emsel = 0x9004;
  #endif 
}
