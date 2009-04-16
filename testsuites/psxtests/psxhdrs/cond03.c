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
 *  $Id: cond03.c,v 1.9 2004/04/16 09:23:25 ralf Exp $
 */

#include <pthread.h>

#ifndef _POSIX_THREADS
#error "rtems is supposed to have pthread_cond_init"
#endif

void test( void )
{
  pthread_cond_t     cond = PTHREAD_COND_INITIALIZER;
  pthread_condattr_t attribute;
  int result;

  result = pthread_cond_init( &cond, &attribute );
}
