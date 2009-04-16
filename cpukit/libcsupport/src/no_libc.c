/*
 *  This file contains stubs for the reentrancy hooks when
 *  an unknown C library is used.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: no_libc.c,v 1.16 2008/08/05 16:51:45 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#if !defined(RTEMS_NEWLIB) && !defined(RTEMS_UNIX)

#include <rtems/libcsupport.h>

#include <stdlib.h>             /* for free() */

void libc_init(void)
)
{
}

void libc_suspend_main(void)
{
}


void libc_global_exit(
  uint32_t   code
)
{
}

void _exit(
  int status
)
{
}

#else

/* remove ANSI errors.
 *  A program must contain at least one external-declaration
 *  (X3.159-1989 p.82,L3).
 */
void no_libc_dummy_function( void )
{
}

#endif
