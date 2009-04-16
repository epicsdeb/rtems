/*
 *  readdir_r - reentrant version of readdir()
 *
 *  $Id: readdir_r.c,v 1.2 2005/01/27 13:32:25 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>

/*
 * The RTEMS version of readdir is already thread-safe.
 */

int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result)
{
     *result = readdir(dirp);
     if (*result)
         *entry = **result;
     return *result ? 0 : errno;
}
