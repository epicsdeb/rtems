/*  Resume_task
 *
 *  This subprogram is scheduled as a timer service routine.  When
 *  it fires it resumes the task which is mapped to this timer.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: resume.c,v 1.4.2.1 2008/12/03 21:00:35 joel Exp $
 */

#include "system.h"

rtems_timer_service_routine Resume_task(
  rtems_id  timer_id,
  void     *ignored_address
)
{
  rtems_id          task_to_resume;
  rtems_status_code status;

  task_to_resume = Task_id[ rtems_object_id_get_index( timer_id ) ];
  status = rtems_task_resume( task_to_resume );
  directive_failed( status, "rtems_task_resume" );
}
