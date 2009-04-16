/*
 *  Dual Port Memory Manager
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: no-dpmem.c,v 1.13 2007/05/09 18:44:20 joel Exp $
 */

#include <rtems/system.h>
#include <rtems/score/address.h>
#include <rtems/score/object.h>
#include <rtems/score/thread.h>
#include <rtems/score/interr.h>

#include <rtems/rtems/status.h>
#include <rtems/rtems/types.h>
#include <rtems/rtems/dpmem.h>

void _Dual_ported_memory_Manager_initialization(
  uint32_t   maximum_ports
)
{
}
