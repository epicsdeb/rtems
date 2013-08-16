/*  bsp.h
 *
 *  This include file contains all SPARC simulator definitions.
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  Ported to ERC32 implementation of the SPARC by On-Line Applications
 *  Research Corporation (OAR) under contract to the European Space
 *  Agency (ESA).
 *
 *  ERC32 modifications of respective RTEMS file: COPYRIGHT (c) 1995.
 *  European Space Agency.
 *
 *  $Id: bsp.h,v 1.29.2.1 2011/04/25 19:39:48 joel Exp $
 */

#ifndef _BSP_H
#define _BSP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bspopts.h>

#include <rtems.h>
#include <rtems/iosupp.h>
#include <erc32.h>
#include <rtems/clockdrv.h>

#include <rtems/console.h>

/*
 *  confdefs.h overrides for this BSP:
 *   - two termios serial ports
 *   - Interrupt stack space is not minimum if defined.
 */

#define CONFIGURE_NUMBER_OF_TERMIOS_PORTS 2

/*
 * Network driver configuration
 */

struct rtems_bsdnet_ifconfig;
extern int rtems_erc32_sonic_driver_attach (struct rtems_bsdnet_ifconfig *config);
#define RTEMS_BSP_NETWORK_DRIVER_NAME	"sonic1"
#define RTEMS_BSP_NETWORK_DRIVER_ATTACH	rtems_erc32_sonic_driver_attach

/* Constants */

/*
 *  Information placed in the linkcmds file.
 */

extern int   RAM_START;
extern int   RAM_END;
extern int   RAM_SIZE;

extern int   PROM_START;
extern int   PROM_END;
extern int   PROM_SIZE;

extern int   CLOCK_SPEED;

extern int   end;        /* last address in the program */

/*
 *  Device Driver Table Entries
 */

/*
 * NOTE: Use the standard Console driver entry
 */

/*
 * NOTE: Use the standard Clock driver entry
 */

/* miscellaneous stuff assumed to exist */

void bsp_cleanup( void );

void bsp_start( void );

rtems_isr_entry set_vector(                     /* returns old vector */
    rtems_isr_entry     handler,                /* isr routine        */
    rtems_vector_number vector,                 /* vector number      */
    int                 type                    /* RTEMS or RAW intr  */
);

void BSP_fatal_return( void );

void bsp_spurious_initialize( void );

#ifdef __cplusplus
}
#endif

#endif
