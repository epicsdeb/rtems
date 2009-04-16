/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: condget.c,v 1.1 2008/01/09 22:08:31 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/score/states.h>
#include <rtems/score/watchdog.h>
#include <rtems/posix/cond.h>
#include <rtems/posix/time.h>
#include <rtems/posix/mutex.h>

POSIX_Condition_variables_Control *_POSIX_Condition_variables_Get (
  pthread_cond_t    *cond,
  Objects_Locations *location
)
{
  Objects_Id        *id = (Objects_Id *)cond;
  int status;

  if ( !id ) {
    *location = OBJECTS_ERROR;
    return (POSIX_Condition_variables_Control *) 0;
  }

  if ( *id == PTHREAD_COND_INITIALIZER ) {
    /*
     *  Do an "auto-create" here.
     */

    status = pthread_cond_init( (pthread_cond_t *)id, 0 );
    if ( status ) {
      *location = OBJECTS_ERROR;
      return (POSIX_Condition_variables_Control *) 0;
    }
  }

  /*
   *  Now call Objects_Get()
   */

  return (POSIX_Condition_variables_Control *)
    _Objects_Get( &_POSIX_Condition_variables_Information, *id, location );
}
 
