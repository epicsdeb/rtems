/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  $Id: bsplibc.c,v 1.12 2008/08/19 12:41:03 joel Exp $
 */

#include <rtems.h>
#include <rtems/libio.h>
#include <rtems/libcsupport.h>

#include <bsp/bootcard.h>

void bsp_libc_init(
  void   *heap_start,
  size_t  heap_size,
  size_t  sbrk_amount
)
{
    RTEMS_Malloc_Initialize( heap_start, heap_size, sbrk_amount );

    /*
     *  Init the RTEMS libio facility to provide UNIX-like system
     *  calls for use by newlib (ie: provide open, close, etc)
     *  Uses malloc() to get area for the iops, so must be after malloc init
     */

    rtems_libio_init();

    /*
     * Set up for the libc handling.
     */

    libc_init();
}
