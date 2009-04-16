/*  Minimum Size Application Initialization 
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: init.c,v 1.15 2008/06/17 21:10:25 joel Exp $
 */

#include <bsp.h>

rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_mode mode;

  /* initialize application */

  /* Real application would call idle loop functionality */

  /* but in this case, just return and fall into a fatal error */ 
}

/* configuration information */

/*
 * This application has no device drivers.
 */
/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER

/*
 *  This is the smallest IO subsystem RTEMS currently supports.
 */
#define CONFIGURE_USE_MINIIMFS_AS_BASE_FILESYSTEM

/*
 *  This disables reentrancy support in the C Library.  It is usually
 *  not something an application wants to do unless the development
 *  team is committed to using C Library routines that are KNOWN to
 *  be reentrant.  Caveat Emptor!!
 */
#define CONFIGURE_DISABLE_NEWLIB_REENTRANCY

/*
 *  This test does not need the console driver so there is no reason
 *  to configure termios.
 */
#define CONFIGURE_TERMIOS_DISABLED

/*
 *  This test does not use any stdio.
 */
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 0

/*
 *  This may prevent us from running on every architecture but it
 *  demonstrates that the user can specify how small of a minimum
 *  stack they want.
 */
#define CONFIGURE_MINIMUM_TASK_STACK_SIZE 512

/*
 *  This lowers the number of priorities that this test is able to
 *  use.  The Idle task will be running at the lowest priority.
 */
#define CONFIGURE_MAXIMUM_PRIORITY 15

/*
 *  This disables Classic API Notepads and saves 16 uint32_t's of RAM
 *  per Task Control Block.  If you aren't using these and are tight
 *  on RAM, this is an option.
 */
#define CONFIGURE_DISABLE_CLASSIC_API_NOTEPADS

/*
 *  In this application, the initialization task performs the system
 *  initialization and then transforms itself into the idle task.
 */
#define CONFIGURE_IDLE_TASK_BODY Init
#define CONFIGURE_IDLE_TASK_INITIALIZES_APPLICATION

/*
 *  If you are debugging confdefs.h, define this
 */
/* #define CONFIGURE_CONFDEFS_DEBUG */

/*
 *  Instantiate the configuration tables.
 */
#define CONFIGURE_INIT

#include <rtems/confdefs.h>

/* global variables */
