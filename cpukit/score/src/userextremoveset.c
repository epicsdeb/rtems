/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: userextremoveset.c,v 1.1 2007/05/09 18:27:26 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/userext.h>

/**
 *  This routine is used to remove a user extension set from the active list.
 */

void _User_extensions_Remove_set (
  User_extensions_Control  *the_extension
)
{
  _Chain_Extract( &the_extension->Node );
  
  /*
   * If a switch handler is present, remove it.
   */

  if ( the_extension->Callouts.thread_switch != NULL )
    _Chain_Extract( &the_extension->Switch.Node );
}
