/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: pget_blk.c,v 1.1 2007/04/17 20:32:12 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/itron.h>

#include <rtems/itron/vmempool.h>

/*
 *  pget_blk - Poll and Get Variable-Size Memory Block
 */

ER pget_blk(
  VP  *p_blk,
  ID   mplid,
  INT  blksz
)
{
  return E_OK;
}
