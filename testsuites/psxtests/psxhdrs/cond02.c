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
 *  $Id: cond02.c,v 1.10 2004/04/16 09:23:25 ralf Exp $
 */

#include <pthread.h>

#ifndef _POSIX_THREADS
#error "rtems is supposed to have pthread_condattr_destroy"
#endif

void test( void )
{
  pthread_condattr_t attribute;
  int result;

  result = pthread_condattr_destroy( &attribute );
}
