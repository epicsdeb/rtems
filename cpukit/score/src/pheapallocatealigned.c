/**
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: pheapallocatealigned.c,v 1.2 2007/12/11 23:43:26 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/protectedheap.h>

void *_Protected_heap_Allocate_aligned(
  Heap_Control *the_heap,
  size_t        size,
  uint32_t      alignment
)
{
  void *p;

  _RTEMS_Lock_allocator();
    p = _Heap_Allocate_aligned( the_heap, size, alignment );
  _RTEMS_Unlock_allocator();
  return p;
}

