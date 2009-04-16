/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: twai_flg.c,v 1.1 2007/04/17 20:32:13 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/itron.h>

#include <rtems/itron/eventflags.h>

/*
 *  twai_flg - Wait on Eventflag with Timeout
 */

ER twai_flg(
  UINT *p_flgptn,
  ID    flgid,
  UINT  waiptn,
  UINT  wfmode,
  TMO   tmout
)
{
  return E_OK;
}
