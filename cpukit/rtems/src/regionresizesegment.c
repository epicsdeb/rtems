/*
 *  Region Manager
 *
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: regionresizesegment.c,v 1.3 2007/11/27 17:38:11 humph Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/score/object.h>
#include <rtems/rtems/options.h>
#include <rtems/rtems/region.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/score/apimutex.h>

/*PAGE
 *
 *  rtems_region_resize_segment
 *
 *  This directive will try to resize segment to the new size 'size'
 *  "in place".
 *
 *  Input parameters:
 *    id      - region id
 *    segment - pointer to segment address
 *    size    - new required size
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code       - if unsuccessful
 */

rtems_status_code rtems_region_resize_segment(
  Objects_Id  id,
  void       *segment,
  size_t      size,
  size_t     *old_size
)
{
  uint32_t                 avail_size;
  Objects_Locations        location;
  uint32_t                 osize;
  rtems_status_code        return_status = RTEMS_INTERNAL_ERROR;
  Heap_Resize_status       status;
  register Region_Control *the_region;

  if ( !old_size )
    return RTEMS_INVALID_ADDRESS;

  _RTEMS_Lock_allocator();

    the_region = _Region_Get( id, &location );
    switch ( location ) {

      case OBJECTS_LOCAL:

        _Region_Debug_Walk( the_region, 7 );

        status = _Heap_Resize_block(
          &the_region->Memory,
          segment,
          (uint32_t) size,
          &osize,
          &avail_size
        );
        *old_size = (uint32_t) osize;

        _Region_Debug_Walk( the_region, 8 );

        if ( status == HEAP_RESIZE_SUCCESSFUL && avail_size > 0 )
          _Region_Process_queue( the_region );    /* unlocks allocator */
        else
          _RTEMS_Unlock_allocator();

        return
          (status == HEAP_RESIZE_SUCCESSFUL) ?  RTEMS_SUCCESSFUL :
          (status == HEAP_RESIZE_UNSATISFIED) ? RTEMS_UNSATISFIED :
          RTEMS_INVALID_ADDRESS;
        break;

#if defined(RTEMS_MULTIPROCESSING)
      case OBJECTS_REMOTE:        /* this error cannot be returned */
        break;
#endif

      case OBJECTS_ERROR:
        return_status = RTEMS_INVALID_ID;
        break;
    }

  _RTEMS_Unlock_allocator();
  return return_status;
}
