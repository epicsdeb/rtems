/*  Clock
 *
 *  This routine generates clock ticks using standard POSIX services.
 *  The tick frequency is specified by the bsp.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: clock.c,v 1.15 2008/09/05 22:06:16 joel Exp $
 */

#include <bsp.h>
#include <rtems/libio.h>
#include <stdlib.h>

void Clock_exit(void);

volatile uint32_t         Clock_driver_ticks;

uint32_t         Clock_driver_vector;

/*
 * These are set by clock driver during its init
 */

rtems_device_major_number rtems_clock_major = ~0;
rtems_device_minor_number rtems_clock_minor;

void Install_clock(rtems_isr_entry clock_isr)
{
    Clock_driver_ticks = 0;

    (void) set_vector( clock_isr, Clock_driver_vector, 1 );

    _CPU_Start_clock( rtems_configuration_get_microseconds_per_tick() );

    atexit(Clock_exit);
}

void Clock_isr(int vector)
{
    Clock_driver_ticks++;
    rtems_clock_tick();
}

/*
 * Called via atexit()
 * Remove the clock signal
 */

void Clock_exit(void)
{
  _CPU_Stop_clock();

  (void) set_vector( 0, Clock_driver_vector, 1 );
}

rtems_device_driver Clock_initialize(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void *pargp
)
{
    Clock_driver_vector = _CPU_Get_clock_vector();

    Install_clock((rtems_isr_entry) Clock_isr);

    /*
     * make major/minor avail to others such as shared memory driver
     */
    rtems_clock_major = major;
    rtems_clock_minor = minor;

    return RTEMS_SUCCESSFUL;
}
