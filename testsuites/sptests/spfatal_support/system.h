/*  system.h
 *
 *  This include file contains information that is included in every
 *  function in the test set.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: system.h,v 1.2 2008/09/06 03:28:08 ralf Exp $
 */

/*
 *  Some of the fatal error cases require the ability to peek inside RTEMS
 */
#define __RTEMS_VIOLATE_KERNEL_VISIBILITY__
#include <rtems.h>
#include <tmacros.h>

/* functions */

rtems_task Init(
  rtems_task_argument argument
);

rtems_extension Fatal_extension(
  uint32_t   source,
  bool       is_internal,
  uint32_t   error
);

/* need prototypes */

#include "testcase.h"

/* configuration information */

extern rtems_extensions_table initial_extensions;

#define CONFIGURE_INITIAL_EXTENSIONS \
  { \
    NULL,                    /* create  */ \
    NULL,                    /* start   */ \
    NULL,                    /* restart */ \
    NULL,                    /* delete  */ \
    NULL,                    /* switch  */ \
    NULL,                    /* begin   */ \
    NULL,                    /* exitted */ \
    Fatal_extension          /* fatal   */ \
  }

/* extra parameters may be in testcase.h */
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

/* always need an Init task, some cases need more tasks */
#ifndef SPFATAL_TEST_CASE_EXTRA_TASKS
#define SPFATAL_TEST_CASE_EXTRA_TASKS 0
#endif
#define CONFIGURE_MAXIMUM_TASKS \
  (SPFATAL_TEST_CASE_EXTRA_TASKS + 1)

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#include <rtems/confdefs.h>

/* global variables */

/* end of include file */
