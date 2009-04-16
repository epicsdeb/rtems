/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: cal_por.c,v 1.1 2007/04/17 20:32:12 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/itron.h>

#include <rtems/itron/port.h>

/*
 *  cal_por - Call Port for Rendezvous Poll
 */

ER cal_por(
  VP    msg,
  INT  *p_rmsgsz,
  ID    porid,
  UINT  calptn,
  INT   cmsgsz
)
{
  return E_OK;
}
