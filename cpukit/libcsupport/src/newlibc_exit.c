/*
 *  Implementation of hooks for the CYGNUS newlib libc
 *  These hooks set things up so that:
 *       + '_REENT' is switched at task switch time.
 *
 *  COPYRIGHT (c) 1994 by Division Incorporated
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: newlibc_exit.c,v 1.4 2008/08/18 19:18:52 joel Exp $
 *
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#define __RTEMS_VIOLATE_KERNEL_VISIBILITY__
#include <rtems.h>

#if defined(RTEMS_NEWLIB)
#include <rtems/libcsupport.h>

/* Since we compile with strict ANSI we need to undef it to get
 * prototypes for extensions
 */
#undef __STRICT_ANSI__

#include <stdlib.h>             /* for free() */
#include <string.h>             /* for memset() */

#include <sys/reent.h>          /* for extern of _REENT (aka _impure_ptr) */
#include <errno.h>

#include <stdio.h>

int _fwalk(struct _reent *ptr, int (*function) (FILE *) );

/* do we think we are reentrant? */
extern int             libc_reentrant;
extern struct _reent   libc_global_reent __ATTRIBUTE_IMPURE_PTR__;

/*
 * CYGNUS newlib routine that does atexit() processing and flushes
 *      stdio streams
 *      undocumented
 */

extern void _wrapup_reent(struct _reent *);
extern void _reclaim_reent(struct _reent *);

void libc_wrapup(void)
{
  /*
   *  In case RTEMS is already down, don't do this.  It could be
   *  dangerous.
   */

  if (!_System_state_Is_up(_System_state_Get()))
     return;

  /*
   *  This was already done if the user called exit() directly .
  _wrapup_reent(0);
   */

  if (_REENT != &libc_global_reent) {
      _wrapup_reent(&libc_global_reent);
#if 0
      /*  Don't reclaim this one, just in case we do printfs
       *  on the way out to ROM.
       */
      _reclaim_reent(&libc_global_reent);
#endif
      _REENT = &libc_global_reent;
  }

  /*
   * Try to drain output buffers.
   *
   * Should this be changed to do *all* file streams?
   *    _fwalk (_REENT, fclose);
   */

  fclose (stdin);
  fclose (stdout);
  fclose (stderr);
}

/*
 *  Function:   _exit
 *  Created:    94/12/10
 *
 *  Description:
 *      Called from exit() after it does atexit() processing and stdio fflush's
 *
 *      called from bottom of exit() to really delete the task.
 *      If we are using reentrant libc, then let the delete extension
 *      do all the work, otherwise if a shutdown is in progress,
 *      then just do it.
 *
 *  Parameters:
 *      exit status
 *
 *  Returns:
 *      does not return
 *
 *  Side Effects:
 *
 *  Notes:
 *
 *
 *  Deficiencies/ToDo:
 *
 *
 */

#include <unistd.h>

#if !defined(RTEMS_UNIX)
  #define EXIT_SYMBOL _exit

  #if defined(__USE_INIT_FINI__)
    extern void _fini( void );
  #endif
#else
  #define EXIT_SYMBOL exit
#endif

void EXIT_SYMBOL(int status)
{
  /*
   *  If the toolset uses init/fini sections, then we need to 
   *  run the global destructors now.
   */
  #if defined(__USE_INIT_FINI__)
    _fini();
  #endif

  /*
   *  We need to do the exit processing on the global reentrancy structure.
   *  This has already been done on the per task reentrancy structure
   *  associated with this task.
   */

  libc_wrapup();
  rtems_shutdown_executive(status);
  for (;;) ; /* to avoid warnings */
}


#endif
