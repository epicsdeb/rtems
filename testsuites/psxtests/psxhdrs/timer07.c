/*
 *  This test file is used to verify that the header files associated with
 *  invoking this function are correct.
 *
 *  COPYRIGHT (c) 1989-2003.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: timer07.c,v 1.3 2004/04/16 09:23:26 ralf Exp $
 */

#include <unistd.h>

void test( void )
{
  unsigned long usec;
  unsigned      result;

  usec = 0;
  result = usleep( usec );
}
