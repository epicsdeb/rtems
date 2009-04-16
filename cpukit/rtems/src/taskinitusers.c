/*
 *  RTEMS Task Manager
 *
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: taskinitusers.c,v 1.7 2007/05/21 23:19:20 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/config.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/rtems/modes.h>
#include <rtems/score/object.h>
#include <rtems/score/stack.h>
#include <rtems/score/states.h>
#include <rtems/rtems/tasks.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>
#include <rtems/score/tod.h>
#include <rtems/score/userext.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/apiext.h>
#include <rtems/score/sysstate.h>

/*PAGE
 *
 *  _RTEMS_tasks_Initialize_user_tasks_body
 *
 *  This routine creates and starts all configured user
 *  initialzation threads.
 *
 *  Input parameters: NONE
 *
 *  Output parameters:  NONE
 */

void _RTEMS_tasks_Initialize_user_tasks_body( void )
{
  uint32_t                          index;
  uint32_t                          maximum;
  rtems_id                          id;
  rtems_status_code                 return_value;
  rtems_initialization_tasks_table *user_tasks;
  rtems_api_configuration_table    *api_configuration;


  api_configuration = _Configuration_Table->RTEMS_api_configuration;

  /*
   *  NOTE:  This is slightly different from the Ada implementation.
   */

  user_tasks = api_configuration->User_initialization_tasks_table;
  maximum    = api_configuration->number_of_initialization_tasks;

  if ( !user_tasks || maximum == 0 )
    return;

  for ( index=0 ; index < maximum ; index++ ) {
    return_value = rtems_task_create(
      user_tasks[ index ].name,
      user_tasks[ index ].initial_priority,
      user_tasks[ index ].stack_size,
      user_tasks[ index ].mode_set,
      user_tasks[ index ].attribute_set,
      &id
    );

    if ( !rtems_is_status_successful( return_value ) )
      _Internal_error_Occurred( INTERNAL_ERROR_RTEMS_API, TRUE, return_value );

    return_value = rtems_task_start(
      id,
      user_tasks[ index ].entry_point,
      user_tasks[ index ].argument
    );

    if ( !rtems_is_status_successful( return_value ) )
      _Internal_error_Occurred( INTERNAL_ERROR_RTEMS_API, TRUE, return_value );
  }
}
