/*
 *  UNIX Port C Library Support
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: unixlibc.c,v 1.15 2008/08/05 16:51:45 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(RTEMS_UNIXLIB)

void libc_init(void)
{
}

#else

/* remove ANSI errors.
 *  A program must contain at least one external-declaration
 *  (X3.159-1989 p.82,L3).
 */
void unixlibc_dummy_function( void )
{
}

#endif
