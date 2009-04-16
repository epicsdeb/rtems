/*
 *  Spinlock Manager
 *
 *  DESCRIPTION:
 *
 *  This package is the implementation of the Spinlock Manager.
 *
 *  Directives provided are:
 *
 *     + create a spinlock
 *     + get an ID of a spinlock
 *     + delete a spinlock
 *     + acquire a spinlock
 *     + release a spinlock
 *
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: pspin.c,v 1.3 2008/02/15 07:39:27 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <limits.h>

#include <rtems/system.h>
#include <rtems/posix/spinlock.h>

/**
 *  @brief _POSIX_Spinlock_Manager_initialization
 *
 *  Input parameters:
 *    maximum_spinlocks - maximum configured spinlocks
 *
 *  Output parameters:  NONE
 */

void _POSIX_Spinlock_Manager_initialization(
  uint32_t   maximum_spinlocks
)
{
  _Objects_Initialize_information(
    &_POSIX_Spinlock_Information,    /* object information table */
    OBJECTS_POSIX_API,              /* object API */
    OBJECTS_POSIX_SPINLOCKS,         /* object class */
    maximum_spinlocks,               /* maximum objects of this class */
    sizeof( POSIX_Spinlock_Control ),/* size of this object's control block */
    TRUE,                           /* TRUE if the name is a string */
    _POSIX_PATH_MAX             /* maximum length of each object's name */
#if defined(RTEMS_MULTIPROCESSING)
    ,
    FALSE,                         /* TRUE if this is a global object class */
    NULL                           /* Proxy extraction support callout */
#endif
  );
}
