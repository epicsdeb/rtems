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
 *  $Id: pthread07.c,v 1.10 2004/04/16 09:23:26 ralf Exp $
 */

#include <pthread.h>

#ifndef _POSIX_THREADS
#error "rtems is supposed to have pthread_create"
#endif

void *test_task(
  void * arg
)
{
  for ( ; ; )
    ;
  return NULL;
}

void test( void )
{
  pthread_t       thread;
  pthread_attr_t  attribute;
  void           *arg = NULL;
  int             result;

  result = pthread_create( &thread, &attribute, test_task, arg );
}
