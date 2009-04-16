/*
 *  _malloc_r Implementation
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: _malloc_r.c,v 1.1 2007/12/18 20:36:40 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef RTEMS_NEWLIB
#include <sys/reent.h>
#include <stdlib.h>

void *_malloc_r(
  struct _reent *ignored,
  size_t         size
)
{
  return malloc( size );
}
#endif
