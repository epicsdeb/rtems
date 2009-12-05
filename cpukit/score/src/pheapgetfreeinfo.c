/**
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: pheapgetfreeinfo.c,v 1.1.4.1 2009/05/13 16:48:22 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/protectedheap.h>

bool _Protected_heap_Get_free_information(
  Heap_Control        *the_heap,
  Heap_Information    *info
)
{
  /*
   * TBD: _Heap_Get_free_information does not error check or return status.
   */

  _RTEMS_Lock_allocator();
    _Heap_Get_free_information( the_heap, info );
  _RTEMS_Unlock_allocator();

  return true;
}

