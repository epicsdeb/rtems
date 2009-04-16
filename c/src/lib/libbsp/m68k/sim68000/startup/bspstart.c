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
 *  $Id: bspstart.c,v 1.14 2008/08/19 09:03:53 ralf Exp $
 */

#include <string.h>

#include <bsp.h>
#include <rtems/libio.h>
#include <rtems/libcsupport.h>

/*
 *  Use the shared implementations of the following routines
 */

void bsp_libc_init( void *, uint32_t, int );
void bsp_pretasking_hook(void);               /* m68k version */

void bsp_predriver_hook(void)
{
  extern void bsp_spurious_initialize(void);
  bsp_spurious_initialize();
}

/*
 *  bsp_start
 *
 *  This routine does the bulk of the system initialization.
 */

void bsp_start( void )
{
  extern void          * _WorkspaceBase;
  extern void          *_RamSize;
  extern unsigned long  _M68k_Ramsize;

#if defined(mcpu32)
#warning "do something about vectors!!!"
#endif

  /*
   *  Clear interrupt sources.
   */

/* XXX address shutdown
  if ( rtems_configuration_get_work_space_size() >(128*1024) )
   _sys_exit( 1 );
*/

  Configuration.work_space_start = (void *) &_WorkspaceBase;
}

#include <rtems/bspIo.h>

void debug_putc( char c ) { write( 2, &c, 1 ); }
BSP_output_char_function_type BSP_output_char =  debug_putc;
BSP_polling_getchar_function_type BSP_poll_char = NULL;
