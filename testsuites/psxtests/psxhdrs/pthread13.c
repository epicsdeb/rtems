/*
 *  This test file is used to verify that the header files associated with
 *  invoking this function are correct.
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: pthread13.c,v 1.12 2009/05/10 20:15:55 joel Exp $
 */

#include <pthread.h>

#ifndef _POSIX_THREAD_PRIORITY_SCHEDULING
#error "RTEMS is supposed to have pthread_attr_setscope"
#endif

void test( void );

void test( void )
{
  pthread_attr_t  attr;
  int             contentionscope;
  int             result;

  contentionscope = PTHREAD_SCOPE_SYSTEM;
  contentionscope = PTHREAD_SCOPE_PROCESS;

  result = pthread_attr_setscope( &attr, contentionscope );
}
