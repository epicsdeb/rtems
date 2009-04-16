/**
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: pheapgetinfo.c,v 1.1 2007/05/03 21:33:39 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/protectedheap.h>

void _Protected_heap_Get_information(
  Heap_Control            *the_heap,
  Heap_Information_block  *the_info
)
{
  Heap_Get_information_status status;

  _RTEMS_Lock_allocator();
    status = _Heap_Get_information( the_heap, the_info );
  _RTEMS_Unlock_allocator();
}
