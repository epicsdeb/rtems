/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: clr_flg.c,v 1.1 2007/04/17 20:32:12 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/itron.h>

#include <rtems/itron/eventflags.h>

/*
 *  clr_flg - Clear Eventflag
 */

ER clr_flg(
  ID   flgid,
  UINT clrptn
)
{
  return E_OK;
}
