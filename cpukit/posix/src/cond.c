/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: cond.c,v 1.31 2008/02/15 07:39:27 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>
#include <errno.h>
#include <limits.h>

#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/score/states.h>
#include <rtems/score/watchdog.h>
#include <rtems/posix/cond.h>
#include <rtems/posix/time.h>
#include <rtems/posix/mutex.h>

/*PAGE
 *
 *  _POSIX_Condition_variables_Manager_initialization
 *
 *  This routine initializes all condition variable manager related data
 *  structures.
 *
 *  Input parameters:
 *    maximum_condition_variables - maximum configured condition_variables
 *
 *  Output parameters:  NONE
 */

void _POSIX_Condition_variables_Manager_initialization(
  uint32_t   maximum_condition_variables
)
{
  _Objects_Initialize_information(
    &_POSIX_Condition_variables_Information, /* object information table */
    OBJECTS_POSIX_API,                       /* object API */
    OBJECTS_POSIX_CONDITION_VARIABLES,       /* object class */
    maximum_condition_variables,             /* maximum objects of this class */
    sizeof( POSIX_Condition_variables_Control ),
                                /* size of this object's control block */
    TRUE,                       /* TRUE if names for this object are strings */
    _POSIX_PATH_MAX             /* maximum length of each object's name */
#if defined(RTEMS_MULTIPROCESSING)
    ,
    FALSE,                      /* TRUE if this is a global object class */
    NULL                        /* Proxy extraction support callout */
#endif
  );
}
