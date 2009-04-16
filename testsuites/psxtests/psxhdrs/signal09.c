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
 *  $Id: signal09.c,v 1.8 2004/04/16 09:23:26 ralf Exp $
 */

#include <sys/types.h>
#include <signal.h>

void test( void )
{
  pid_t pid;
  int   signal_number;
  int   result;

  pid = 0;
  signal_number = SIGALRM;

  result = kill( pid, signal_number );
}
