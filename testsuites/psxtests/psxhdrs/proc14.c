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
 *  $Id: proc14.c,v 1.9 2004/04/16 09:23:26 ralf Exp $
 */

#include <sys/types.h>

void test( void )
{
  pid_t pid = 0;
  pid_t pgid = 0;
  int   result;

  result = setpgid( pid, pgid );
}
