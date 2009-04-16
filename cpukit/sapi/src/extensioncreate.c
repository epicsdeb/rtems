/*
 *  Extension Manager -- rtems_extension_create
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: extensioncreate.c,v 1.5 2007/12/19 20:55:55 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/support.h>
#include <rtems/score/object.h>
#include <rtems/score/thread.h>
#include <rtems/extension.h>

/*PAGE
 *
 *  rtems_extension_create
 *
 *  This directive creates a extension and performs some initialization.
 *
 *  Input parameters:
 *    name            - extension name
 *    extension_table - pointer to extension set information
 *    id              - pointer to extension id
 *
 *  Output parameters:
 *    id                - extension id
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

rtems_status_code rtems_extension_create(
  rtems_name              name,
  rtems_extensions_table *extension_table,
  Objects_Id             *id
)
{
  Extension_Control *the_extension;

  if ( !id )
    return RTEMS_INVALID_ADDRESS;

  if ( !rtems_is_name_valid( name ) )
    return RTEMS_INVALID_NAME;

  _Thread_Disable_dispatch();         /* to prevent deletion */

  the_extension = _Extension_Allocate();

  if ( !the_extension ) {
    _Thread_Enable_dispatch();
    return RTEMS_TOO_MANY;
  }

  _User_extensions_Add_set( &the_extension->Extension, extension_table );

  _Objects_Open(
    &_Extension_Information,
    &the_extension->Object,
    (Objects_Name) name
  );

  *id = the_extension->Object.id;
  _Thread_Enable_dispatch();
  return RTEMS_SUCCESSFUL;
}
