/*
 *  rtems, floss, libc are always compiled with GNU compilers
 *  application code may be compiled with some other C++ compiler
 *  that has a different global constructor technique.
 *
 *  For the simple case, where the app has no global constructors or
 *  is compiled by g++, we provide this empty routine
 *  In order to get both g++ constructors (RTEMS::RTEMS, for example)
 *  and application constructors run, we provide this routine.
 *
 *  $Id: no-ctor.c,v 1.1 1996/01/15 21:49:55 joel Exp $
 */

void invoke_non_gnu_constructors(void)
{
}
