/*
 *  $Id: nvdisk-sram.h,v 1.1 2008/05/01 04:15:36 ccj Exp $
 *
 * RTEMS Project (http://www.rtems.org/)
 *
 * Copyright 2007 Chris Johns (chrisj@rtems.org)
 */

/**
 * NV Disk Static RAM Device Driver.
 *
 * This driver maps an NV disk to static RAM. You can use this 
 */

#if !defined (_RTEMS_NVDISK_SRAM_H_)
#define _RTEMS_NVDISK_SRAM_H_

#include <rtems/nvdisk.h>

/**
 * The handlers for the NV Disk SRAM driver.
 */
extern const rtems_nvdisk_driver_handlers rtems_nvdisk_sram_handlers;

#endif
