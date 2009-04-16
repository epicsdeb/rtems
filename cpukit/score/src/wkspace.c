/*
 *  Workspace Handler
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: wkspace.c,v 1.24 2008/09/04 17:39:56 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/interr.h>
#include <rtems/config.h>

#include <string.h>  /* for memset */

/*
 *  _Workspace_Handler_initialization
 */
void _Workspace_Handler_initialization(
  void       *starting_address,
  size_t      size
)
{
  uint32_t    memory_available;

  if ( !starting_address || !_Addresses_Is_aligned( starting_address ) )
    _Internal_error_Occurred(
      INTERNAL_ERROR_CORE,
      TRUE,
      INTERNAL_ERROR_INVALID_WORKSPACE_ADDRESS
    );

  if ( _Configuration_Table->do_zero_of_workspace )
   memset( starting_address, 0, size );

  memory_available = _Heap_Initialize(
    &_Workspace_Area,
    starting_address,
    size,
    CPU_HEAP_ALIGNMENT
  );

  if ( memory_available == 0 )
    _Internal_error_Occurred(
      INTERNAL_ERROR_CORE,
      TRUE,
      INTERNAL_ERROR_TOO_LITTLE_WORKSPACE
    );
}

/*
 *  _Workspace_Allocate
 */
void *_Workspace_Allocate(
  size_t   size
)
{
   return _Heap_Allocate( &_Workspace_Area, size );
}

/*
 *  _Workspace_Free
 */
bool _Workspace_Free(
  void *block
)
{
   return _Heap_Free( &_Workspace_Area, block );
}

/*
 *  _Workspace_Allocate_or_fatal_error
 */
void *_Workspace_Allocate_or_fatal_error(
  size_t      size
)
{
  void        *memory;

  memory = _Workspace_Allocate( size );

  if ( memory == NULL )
    _Internal_error_Occurred(
      INTERNAL_ERROR_CORE,
      TRUE,
      INTERNAL_ERROR_WORKSPACE_ALLOCATION
    );

  return memory;
}
