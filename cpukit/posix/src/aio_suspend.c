/*
 *  6.7.7 Wait for Asynchronous I/O Request, P1003.1b-1993, p. 164
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: aio_suspend.c,v 1.1.2.1 2009/01/02 09:53:09 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <aio.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/seterr.h>

int aio_suspend(
  const struct aiocb  * const list[],
  int                     nent,
  const struct timespec  *timeout
)
{
  rtems_set_errno_and_return_minus_one( ENOSYS );
}
