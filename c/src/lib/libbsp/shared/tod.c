/*
 *  Real Time Clock Driver Wrapper for Libchip
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: tod.c,v 1.12 2008/09/05 08:08:39 ralf Exp $
 */

#include <rtems.h>
#include <libchip/rtc.h>

/*
 *  Configuration Information
 */

extern size_t                     RTC_Count;
extern rtems_device_minor_number  RTC_Minor;

int RTC_Present;

extern void setRealTimeToRTEMS(void);

/*
 *  rtc_initialize
 *
 *  Initialize the RTC driver
 */

rtems_device_driver rtc_initialize(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor_arg,
  void                      *arg
)
{
  rtems_device_minor_number  minor;
  rtems_status_code          status;

  for (minor=0; minor < RTC_Count ; minor++) {
    /*
     * First perform the configuration dependent probe, then the
     * device dependent probe
     */

    if (RTC_Table[minor].deviceProbe && RTC_Table[minor].deviceProbe(minor)) {
      /*
       * Use this device as the primary RTC
       */
      RTC_Minor = minor;
      RTC_Present = 1;
      break;
    }
  }

  if ( !RTC_Present ) {
    /*
     * Failed to find an RTC -- this is not a fatal error.
     */

    return RTEMS_INVALID_NUMBER;
  }

  /*
   *  Register and initialize the primary RTC's
   */

  status = rtems_io_register_name( "/dev/rtc", major, RTC_Minor );
  if (status != RTEMS_SUCCESSFUL) {
    rtems_fatal_error_occurred(status);
  }

  RTC_Table[minor].pDeviceFns->deviceInitialize( RTC_Minor );

  /*
   *  Now initialize any secondary RTC's
   */

  for ( minor++ ; minor<RTC_Count ; minor++) {
    /*
     * First perform the configuration dependent probe, then the
     * device dependent probe
     */

    if (RTC_Table[minor].deviceProbe && RTC_Table[minor].deviceProbe(minor)) {
      status = rtems_io_register_name(
        RTC_Table[minor].sDeviceName,
        major,
        minor );
      if (status != RTEMS_SUCCESSFUL) {
        rtems_fatal_error_occurred(status);
      }

      /*
       * Initialize the hardware device.
       */

      RTC_Table[minor].pDeviceFns->deviceInitialize(minor);

    }
  }

  setRealTimeToRTEMS();
  return RTEMS_SUCCESSFUL;
}

/*PAGE
 *
 *  This routine copies the time from the real time clock to RTEMS
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  Return values: NONE
 */

void setRealTimeToRTEMS()
{
  rtems_time_of_day rtc_tod;

  if (!RTC_Present)
    return;

  RTC_Table[RTC_Minor].pDeviceFns->deviceGetTime(RTC_Minor, &rtc_tod);
  rtems_clock_set( &rtc_tod );
}

/*PAGE
 *
 *  setRealTimeFromRTEMS
 *
 *  This routine copies the time from RTEMS to the real time clock
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  Return values: NONE
 */

void setRealTimeFromRTEMS(void)
{
  rtems_time_of_day rtems_tod;

  if (!RTC_Present)
    return;

  rtems_clock_get( RTEMS_CLOCK_GET_TOD, &rtems_tod );
  RTC_Table[RTC_Minor].pDeviceFns->deviceSetTime(RTC_Minor, &rtems_tod);
}

/*PAGE
 *
 *  getRealTime
 *
 *  This routine reads the current time from the RTC.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  Return values: NONE
 */

void getRealTime(
  rtems_time_of_day *tod
)
{

  if (!RTC_Present)
    return;

  RTC_Table[RTC_Minor].pDeviceFns->deviceGetTime(RTC_Minor, tod);
}

/*PAGE
 *
 *  setRealTime
 *
 *  This routine sets the RTC.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  Return values: NONE
 */

/* XXX this routine should be part of the public RTEMS interface */
extern bool _TOD_Validate( rtems_time_of_day *tod );

int setRealTime(
  rtems_time_of_day *tod
)
{

  if (!RTC_Present)
    return -1;

  if ( !_TOD_Validate(tod) )
    return -1;

  RTC_Table[RTC_Minor].pDeviceFns->deviceSetTime(RTC_Minor, tod);
  return 0;
}

/*PAGE
 *
 *  checkRealTime
 *
 *  This routine reads the returns the variance betweent the real time and
 *  rtems time.
 *
 *  Input parameters: NONE
 *
 *  Output parameters:  NONE
 *
 *  Return values:
 *    int   The differance between the real time clock and rtems time.
 */

/* XXX this routine should be part of the public RTEMS interface */
uint32_t   _TOD_To_seconds( rtems_time_of_day *tod );

int checkRealTime(void)
{
  rtems_time_of_day rtems_tod;
  rtems_time_of_day rtc_tod;
  uint32_t   rtems_time;
  uint32_t   rtc_time;

  if (!RTC_Present)
    return -1;

  rtems_clock_get( RTEMS_CLOCK_GET_TOD, &rtems_tod );
  RTC_Table[RTC_Minor].pDeviceFns->deviceGetTime(RTC_Minor, &rtc_tod);

  rtems_time = _TOD_To_seconds( &rtems_tod );
  rtc_time = _TOD_To_seconds( &rtc_tod );

  return rtems_time - rtc_time;
}
