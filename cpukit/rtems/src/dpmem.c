/*
 *  Dual Port Memory Manager
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: dpmem.c,v 1.19 2007/05/21 23:19:20 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/score/address.h>
#include <rtems/rtems/dpmem.h>
#include <rtems/score/object.h>
#include <rtems/score/thread.h>

/*PAGE
 *
 *  _Dual_ported_memory_Manager_initialization
 *
 *  This routine initializes all dual-ported memory manager related
 *  data structures.
 *
 *  Input parameters:
 *    maximum_ports - number of ports to initialize
 *
 *  Output parameters:  NONE
 */

void _Dual_ported_memory_Manager_initialization(
  uint32_t   maximum_ports
)
{
  _Objects_Initialize_information(
    &_Dual_ported_memory_Information, /* object information table */
    OBJECTS_CLASSIC_API,              /* object API */
    OBJECTS_RTEMS_PORTS,              /* object class */
    maximum_ports,                    /* maximum objects of this class */
    sizeof( Dual_ported_memory_Control ),
                                  /* size of this object's control block */
    FALSE,                        /* TRUE if names of this object are strings */
    RTEMS_MAXIMUM_NAME_LENGTH     /* maximum length of each object's name */
#if defined(RTEMS_MULTIPROCESSING)
    ,
    FALSE,                        /* TRUE if this is a global object class */
    NULL                          /* Proxy extraction support callout */
#endif
  );
}
