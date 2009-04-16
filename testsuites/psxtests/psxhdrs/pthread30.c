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
 *  $Id: pthread30.c,v 1.9 2004/04/16 09:23:26 ralf Exp $
 */

#include <pthread.h>

#ifndef _POSIX_THREADS
#error "rtems is supposed to have pthread_setcanceltype"
#endif

void init_routine( void )
{
}

void test( void )
{
  int         type;
  int         oldtype;
  int         result;

  type = PTHREAD_CANCEL_DEFERRED;
  type = PTHREAD_CANCEL_ASYNCHRONOUS;

  result = pthread_setcanceltype( type, &oldtype );
}
