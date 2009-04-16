/*
 * Real Time Clock (MK48T08) for RTEMS on Score603e
 *
 *  Based on MVME162 TOD by:
 *    COPYRIGHT (C) 1997
 *    by Katsutoshi Shibuya - BU Denken Co.,Ltd. - Sapporo - JAPAN
 *    ALL RIGHTS RESERVED
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: tod.h,v 1.4 2004/04/21 16:01:47 ralf Exp $
 */

#ifndef TOD_H
#define TOD_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Set the RTC.
 */

int setRealTime(
  rtems_time_of_day *tod
);

/*
 *  Get the time from the RTC.
 */

void getRealTime(
  rtems_time_of_day *tod
);

/*
 *  Read real time from RTC and set it to RTEMS' clock manager
 */

void setRealTimeToRTEMS();

/*
 *  Read time from RTEMS' clock manager and set it to RTC
 */

void setRealTimeFromRTEMS();

/*
 *  Return the difference between RTC and RTEMS' clock manager time in minutes.
 *  If the difference is greater than 1 day, this returns 9999.
 */

int checkRealTime();

#ifdef __cplusplus
}
#endif

#endif
