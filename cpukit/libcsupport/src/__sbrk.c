/*
 *  RTEMS "Broken" __sbrk Implementation
 *
 *  NOTE: sbrk is provided by the BSP.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: __sbrk.c,v 1.4 2003/09/04 18:54:13 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#if !defined(RTEMS_UNIX)

#include <errno.h>

void * __sbrk(
  int incr
)
{
  errno = EINVAL;
  return (void *)0;
}
#endif
