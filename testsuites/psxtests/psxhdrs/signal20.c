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
 *  $Id: signal20.c,v 1.8 2004/04/16 09:23:26 ralf Exp $
 */

#include <signal.h>

void test( void )
{
  unsigned int seconds;
  unsigned int result;

  seconds = 10;

  result = alarm( seconds );
}
