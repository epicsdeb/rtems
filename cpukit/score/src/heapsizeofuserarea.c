/*
 *  Heap Handler
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: heapsizeofuserarea.c,v 1.11 2008/09/04 17:39:55 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/sysstate.h>
#include <rtems/score/heap.h>

/*PAGE
 *
 *  _Heap_Size_of_user_area
 *
 *  This kernel routine sets '*size' to the size of the block of memory
 *  which begins at 'starting_address'.
 *  It returns TRUE if the 'starting_address' is in the heap, and FALSE
 *  otherwise.
 *
 *  Input parameters:
 *    the_heap         - pointer to heap header
 *    starting_address - starting address of the memory block
 *    size             - pointer to size of area
 *
 *  Output parameters:
 *    size  - size of area filled in
 *    TRUE  - if starting_address is valid heap address
 *    FALSE - if starting_address is invalid heap address
 */

bool _Heap_Size_of_user_area(
  Heap_Control        *the_heap,
  void                *starting_address,
  size_t              *size
)
{
  Heap_Block        *the_block;
  Heap_Block        *next_block;
  uint32_t           the_size;

  if ( !_Addresses_Is_in_range(
         starting_address, (void *)the_heap->start, (void *)the_heap->final ) )
    return( FALSE );

  _Heap_Start_of_block( the_heap, starting_address, &the_block );

  _HAssert(_Heap_Is_block_in( the_heap, the_block ));
  if ( !_Heap_Is_block_in( the_heap, the_block ) )
    return( FALSE );

  the_size   = _Heap_Block_size( the_block );
  next_block = _Heap_Block_at( the_block, the_size );

  _HAssert(_Heap_Is_block_in( the_heap, next_block ));
  _HAssert(_Heap_Is_prev_used( next_block ));
  if (
    !_Heap_Is_block_in( the_heap, next_block ) ||
    !_Heap_Is_prev_used( next_block )
  )
    return( FALSE );

  /* 'starting_address' could be greater than 'the_block' address plus
     HEAP_BLOCK_USER_OFFSET as _Heap_Allocate_aligned() may produce such user
     pointers. To get rid of this offset we calculate user size as difference
     between the end of 'the_block' (='next_block') and 'starting_address'
     and then add correction equal to the offset of the 'size' field of the
     'Heap_Block' structure. The correction is due to the fact that
     'prev_size' field of the next block is actually used as user accessible
     area of 'the_block'. */

  *size = _Addresses_Subtract ( next_block, starting_address )
    + HEAP_BLOCK_HEADER_OFFSET;

  return( TRUE );
}

