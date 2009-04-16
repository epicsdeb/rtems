/*
 * Philips LPC22XX/LPC21xx BSP Shutdown  code
 * Copyright (c) 2007 by Ray Xu <rayx.cn@gmail.com>
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *
 *  http://www.rtems.com/license/LICENSE.
 *
 *
 *  $Id: bspclean.c,v 1.4 2007/11/06 22:50:42 joel Exp $
*/

#include <bsp.h>
#include <rtems/bspIo.h>
#include <rtems/libio.h>

int uart_poll_read(int);

void rtemsReboot (void)
{
#ifdef __thumb__
  int tmp;
  asm volatile (" .code 16            \n" \
                "ldr %[tmp], =_start  \n" \
                "bx  %[tmp]           \n" \
                "nop                  \n" \
                : [tmp]"=&r" (tmp) );
#else                   
  asm volatile ("b _start");
#endif   
}

void bsp_cleanup(void)
{
  /*
   * AT this point, the console driver is disconnected => we must
   * use polled output/input. This is exactly what printk
   * does.
   */
  printk("\nEXECUTIVE SHUTDOWN! Any key to reboot...");
  while (uart_poll_read(0) < 0) continue;

  /* rtemsReboot(); */
}
