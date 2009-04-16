/*
 *  This file contains the TTY driver for the serial ports on the LEON.
 *
 *  This driver uses the termios pseudo driver.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  Modified for LEON3 BSP.
 *  COPYRIGHT (c) 2004.
 *  Gaisler Research.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: debugputs.c,v 1.7 2007/09/06 15:34:58 joel Exp $
 */

#include <bsp.h>
#include <rtems/libio.h>
#include <stdlib.h>
#include <assert.h>

/*
 *  console_outbyte_polled
 *
 *  This routine transmits a character using polling.
 */

void console_outbyte_polled(
  int  port,
  unsigned char ch
)
{
  if ((port >= 0) && (port <= CONFIGURE_NUMBER_OF_TERMIOS_PORTS))
  {
    while ( (LEON3_Console_Uart[LEON3_Cpu_Index+port]->status &
                      LEON_REG_UART_STATUS_THE) == 0 );
    LEON3_Console_Uart[LEON3_Cpu_Index+port]->data = (unsigned int) ch;
  }
}

/*
 *  console_inbyte_nonblocking 
 *
 *  This routine polls for a character.
 */

int console_inbyte_nonblocking( int port )
{

  if ((port >=0) && (port < CONFIGURE_NUMBER_OF_TERMIOS_PORTS)) 
  {

      if (LEON3_Console_Uart[LEON3_Cpu_Index+port]->status & LEON_REG_UART_STATUS_ERR) {
        LEON3_Console_Uart[LEON3_Cpu_Index+port]->status = ~LEON_REG_UART_STATUS_ERR;
      }

      if ((LEON3_Console_Uart[LEON3_Cpu_Index+port]->status & LEON_REG_UART_STATUS_DR) == 0)
         return -1;
      return (int) LEON3_Console_Uart[LEON3_Cpu_Index+port]->data;
  }

  else
  {
      assert( 0 );
  }

  return -1;
}

/* putchar/getchar for printk */

static void bsp_out_char(char c)
{
  console_outbyte_polled(0, c);
}

/*
 *  To support printk
 */

#include <rtems/bspIo.h>

BSP_output_char_function_type BSP_output_char = bsp_out_char;

static char bsp_in_char(void)
{
  int tmp;

  while ((tmp = console_inbyte_nonblocking(0)) < 0);
  return (char) tmp;
}

BSP_polling_getchar_function_type BSP_poll_char = bsp_in_char;
