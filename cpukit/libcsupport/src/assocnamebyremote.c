/*
 * assoc.c
 *      rtems assoc routines
 *
 *  $Id: assocnamebyremote.c,v 1.1 2007/03/26 22:31:31 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#define INSIDE_ASSOC

#include <rtems.h>
#include <rtems/assoc.h>

#include <string.h>             /* strcat, strcmp */


const char *rtems_assoc_name_by_remote(
  const rtems_assoc_t *ap,
  uint32_t             remote_value
)
{
  const rtems_assoc_t *nap;
  nap = rtems_assoc_ptr_by_remote(ap, remote_value);

  if (nap)
    return nap->name;

  return rtems_assoc_name_bad(remote_value);
}

