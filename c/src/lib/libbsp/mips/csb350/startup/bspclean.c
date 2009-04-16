/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspclean.c,v 1.3 2007/03/12 11:17:51 joel Exp $
 */
#include <rtems.h>
#include <libcpu/au1x00.h>
#include <rtems/bspIo.h>

void bsp_cleanup( void )
{
  int console_inbyte_nonblocking(int);
  void (*reset_func)(void);
  
  reset_func = (void *)0xbfc00000;

  mips_set_sr( 0x00200000 ); /* all interrupts off, boot exception vectors */

  printk("\nEXECUTIVE SHUTDOWN! Any key to reboot...");
  while (console_inbyte_nonblocking(0) < 0) {
    continue;
  }

  /* Try to restart bootloader */
  reset_func();
}
