/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: userextthreadcreate.c,v 1.2 2008/09/04 17:39:56 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/userext.h>

/*PAGE
 *
 *  _User_extensions_Thread_create
 */

bool _User_extensions_Thread_create (
  Thread_Control *the_thread
)
{
  Chain_Node              *the_node;
  User_extensions_Control *the_extension;
  bool                     status;

  for ( the_node = _User_extensions_List.first ;
        !_Chain_Is_tail( &_User_extensions_List, the_node ) ;
        the_node = the_node->next ) {

    the_extension = (User_extensions_Control *) the_node;

    if ( the_extension->Callouts.thread_create != NULL ) {
      status = (*the_extension->Callouts.thread_create)(
        _Thread_Executing,
        the_thread
      );
      if ( !status )
        return FALSE;
    }
  }

  return TRUE;
}
