/*
 *  $Id: sersupp.h,v 1.4 2009/09/30 02:44:53 ralf Exp $
 */

#ifndef __LIBCHIP_SERIAL_SUPPORT_h
#define __LIBCHIP_SERIAL_SUPPORT_h

#include <rtems/termiostypes.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libchip_serial_default_probe(
  int minor
);

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
