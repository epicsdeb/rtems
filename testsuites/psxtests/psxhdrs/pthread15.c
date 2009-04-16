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
 *  $Id: pthread15.c,v 1.10 2004/04/16 09:23:26 ralf Exp $
 */

#include <pthread.h>

#ifndef _POSIX_THREAD_PRIORITY_SCHEDULING
#error "RTEMS is supposed to have pthread_attr_setinheritsched"
#endif

void test( void )
{
  pthread_attr_t  attr;
  int             inheritsched;
  int             result;

  inheritsched = PTHREAD_INHERIT_SCHED;
  inheritsched = PTHREAD_EXPLICIT_SCHED;

  result = pthread_attr_setinheritsched( &attr, inheritsched );
}
