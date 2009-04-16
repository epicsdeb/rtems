/*
 *  Extension Manager
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: extension.c,v 1.20 2008/09/04 17:46:39 ralf Exp $
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
 *  _Extension_Manager_initialization
 *
 *  This routine initializes all extension manager related data structures.
 *
 *  Input parameters:
 *    maximum_extensions - number of extensions to initialize
 *
 *  Output parameters:  NONE
 */

void _Extension_Manager_initialization(
  uint32_t   maximum_extensions
)
{
  _Objects_Initialize_information(
    &_Extension_Information,
    OBJECTS_CLASSIC_API,                 /* object API */
    OBJECTS_RTEMS_EXTENSIONS,
    maximum_extensions,
    sizeof( Extension_Control ),
    false,                     /* true if the name is a string */
    RTEMS_MAXIMUM_NAME_LENGTH  /* maximum length of an object name */
#if defined(RTEMS_MULTIPROCESSING)
    ,
    false,                     /* true if this is a global object class */
    NULL                       /* Proxy extraction support callout */
#endif
  );
}
