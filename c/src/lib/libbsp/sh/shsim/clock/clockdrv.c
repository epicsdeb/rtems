/*
 *  Instantiate the clock driver shell.
 *
 *  Since there is no clock source on the simulator, all we do is
 *  make sure it will build.
 *
 *  $Id: clockdrv.c,v 1.2 2004/04/21 16:01:47 ralf Exp $
 */

#define CLOCK_VECTOR 0

#define Clock_driver_support_at_tick()

#define Clock_driver_support_install_isr( _new, _old ) \
  do { _old = 0; } while(0)

#define Clock_driver_support_initialize_hardware()

#define Clock_driver_support_shutdown_hardware()

#include "../../../shared/clockdrv_shell.c"
