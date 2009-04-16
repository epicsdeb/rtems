/*
 *  WHOAMI Shell Command Implmentation
 *
 *  Author: Fernando RUIZ CASAS
 *  Work: fernando.ruiz@ctv.es
 *  Home: correo@fernando-ruiz.com
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: main_whoami.c,v 1.5 2008/07/17 15:43:56 joel Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>

#include <rtems.h>
#include <rtems/shell.h>
#include "internal.h"

int rtems_shell_main_whoami(
  int   argc,
  char *argv[]
)
{
  struct passwd *pwd;

  pwd = getpwuid(geteuid());
  printf( "%s\n", (pwd) ? pwd->pw_name : "nobody");
  return 0;
}

rtems_shell_cmd_t rtems_shell_WHOAMI_Command = {
  "whoami",                                   /* name */
  "show current user",                        /* usage */
  "misc",                                     /* topic */
  rtems_shell_main_whoami,                    /* command */
  NULL,                                       /* alias */
  NULL                                        /* next */
};
