/*
 * Copyright (c) 2008 by Matthieu Bucchianeri <mbucchia@gmail.com>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 *
 * http://www.rtems.com/license/LICENSE
 *
 * $Id: bsp.h,v 1.1 2008/04/16 18:37:31 joel Exp $
 */

#ifndef __BSP_H_
#define __BSP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <bspopts.h>
#include <rtems.h>
#include <rtems/iosupp.h>
#include <rtems/bspIo.h>
#include <rtems/console.h>
#include <rtems/clockdrv.h>

  struct rtems_bsdnet_ifconfig;

  int
    rtems_wifi_driver_attach (struct rtems_bsdnet_ifconfig *config,
                              int attach);

#define RTEMS_BSP_NETWORK_DRIVER_NAME	"dswifi0"
#define RTEMS_BSP_NETWORK_DRIVER_ATTACH	rtems_wifi_driver_attach

  void bsp_start (void);
  void bsp_cleanup (void);


#define RTC_DRIVER_TABLE_ENTRY \
    { rtc_initialize, NULL, NULL, NULL, NULL, NULL }
  extern rtems_device_driver rtc_initialize (rtems_device_major_number major,
                                             rtems_device_minor_number minor,
                                             void *arg);

#ifdef __cplusplus
}
#endif

#endif
