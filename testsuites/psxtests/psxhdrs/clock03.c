/*
 *  This test file is used to verify that the header files associated with
 *  invoking this function are correct.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: clock03.c,v 1.8 2004/04/16 09:23:25 ralf Exp $
 */

#include <time.h>

#ifndef _POSIX_TIMERS
#error "rtems is supposed to have clock_getres"
#endif

void test( void )
{
  clockid_t        clock_id = 0;
  struct timespec  res;
  int              result;

  result = clock_getres( clock_id, &res );
}
