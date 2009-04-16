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
 *  $Id: pthread35.c,v 1.8 2004/04/16 09:23:26 ralf Exp $
 */

#include <pthread.h>

#ifndef _POSIX_THREAD_CPUTIME
#error "rtems is supposed to have pthread_attr_setcputime"
#endif

void test( void )
{
  pthread_attr_t  attr;
  int             clock_allowed;
  int             result;

  clock_allowed = CLOCK_ALLOWED;
  clock_allowed = CLOCK_DISALLOWED;

  result = pthread_attr_setcputime( &attr, clock_allowed );
}
