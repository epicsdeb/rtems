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
 *  $Id: pthread32.c,v 1.8 2004/04/16 09:23:26 ralf Exp $
 */

#include <pthread.h>

#ifndef _POSIX_THREADS
#error "rtems is supposed to have pthread_cleanup_push"
#endif

void cleanup_routine(
  void *argument
)
{
}

void test( void )
{
  int argument;

  argument = 0;

  pthread_cleanup_push( cleanup_routine, &argument );
}
