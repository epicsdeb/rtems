/*  bsp.h
 *
 *  This include file contains all board IO definitions.
 *
 *  XXX : put yours in here
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bsp.h,v 1.6 2007/12/11 15:48:51 joel Exp $
 */

#ifndef _BSP_H
#define _BSP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bspopts.h>

#include <rtems.h>
#include <rtems/iosupp.h>
#include <rtems/console.h>
#include <rtems/clockdrv.h>
#include <libcpu/rm5231.h>

extern void WriteDisplay( char * string );

/*
 *  Stuff for Time Test 27
 */

#define MUST_WAIT_FOR_INTERRUPT 0

#define Install_tm27_vector( handler ) set_vector( (handler), 0, 1 )

#define Cause_tm27_intr()

#define Clear_tm27_intr()

#define Lower_tm27_intr()

extern uint32_t mips_get_timer( void );

#define CPU_CLOCK_RATE_MHZ     (200)
#define CLOCKS_PER_MICROSECOND ( CPU_CLOCK_RATE_MHZ ) /* equivalent to CPU clock speed in MHz */

/*
 *  Simple spin delay in microsecond units for device drivers.
 *  This is very dependent on the clock speed of the target.
 *
 *  NOTE: This macro generates a warning like "integer constant out 
 *        of range" which is safe to ignore.  In 64 bit mode, unsigned32
 *        types are actually 64 bits long so that comparisons between
 *        unsigned32 types and pointers are valid.  The warning is caused
 *        by code in the delay macro that is necessary for 64 bit mode.
 */

#define rtems_bsp_delay( microseconds ) \
  { \
     uint32_t _end_clock = \
          mips_get_timer() + microseconds * CLOCKS_PER_MICROSECOND; \
     _end_clock %= 0x100000000;  /* make sure result is 32 bits */ \
     \
     /* handle timer overflow, if necessary */ \
     while ( _end_clock < mips_get_timer() );  \
     \
     while ( _end_clock > mips_get_timer() ); \
  }

/* Constants */

#define RAM_START 0
#define RAM_END   0x100000

/* miscellaneous stuff assumed to exist */

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

rtems_isr_entry set_vector(
  rtems_isr_entry, rtems_vector_number, int );

#ifdef __cplusplus
}
#endif

#endif	/* __HURRICANE_BSP_h */
