/*
 *  MMOVE Shell Command Implmentation
 *
 *  Author: Fernando RUIZ CASAS
 *  Work: fernando.ruiz@ctv.es
 *  Home: correo@fernando-ruiz.com
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: main_mmove.c,v 1.4 2008/02/27 21:52:16 joel Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <rtems.h>
#include <rtems/shell.h>
#include "internal.h"

extern int rtems_shell_main_mdump(int, char *);

int rtems_shell_main_mmove(
  int   argc,
  char *argv[]
)
{
 uintptr_t  src;
 uintptr_t  dst;
 size_t     length;

 if ( argc<4 ) {
  fprintf(stderr,"%s: too few arguments\n", argv[0]);
  return -1;
 }

 dst    = rtems_shell_str2int(argv[1]);
 src    = rtems_shell_str2int(argv[2]);
 length = rtems_shell_str2int(argv[3]);
 memcpy((unsigned char*)dst, (unsigned char*)src, length);

 return 0;
}

rtems_shell_cmd_t rtems_shell_MMOVE_Command = {
  "mmove",                                      /* name */
  "mmove dst src length",                       /* usage */
  "mem",                                        /* topic */
  rtems_shell_main_mmove,                       /* command */
  NULL,                                         /* alias */
  NULL                                          /* next */
};
