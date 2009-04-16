/*
 *  Extension Manager -- rtems_extension_ident
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: extensionident.c,v 1.7 2008/01/23 22:57:43 joel Exp $
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
 *  rtems_extension_ident
 *
 *  This directive returns the system ID associated with
 *  the extension name.
 *
 *  Input parameters:
 *    name - user defined message queue name
 *    id   - pointer to extension id
 *
 *  Output parameters:
 *    *id               - message queue id
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

rtems_status_code rtems_extension_ident(
  rtems_name    name,
  Objects_Id   *id
)
{
  Objects_Name_or_id_lookup_errors  status;

  status = _Objects_Name_to_id_u32(
    &_Extension_Information,
    name,
    OBJECTS_SEARCH_LOCAL_NODE,
    id
  );

  return _Status_Object_name_errors_to_status[ status ];
}
