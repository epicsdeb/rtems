/*
 *  tm27.h
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: tm27.h,v 1.1 2008/07/14 08:45:02 thomas Exp $
 */

#ifndef _RTEMS_TMTEST27
#error "This is an RTEMS internal file you must not include directly."
#endif

#ifndef __tm27_h
#define __tm27_h

/*
 *  Stuff for Time Test 27
 */

#define MUST_WAIT_FOR_INTERRUPT 0

#define Install_tm27_vector( handler ) set_vector( (handler), PPC_IRQ_SCALL, 1 )

#define Cause_tm27_intr()  asm volatile ("sc")

#define Clear_tm27_intr() /* empty */

#define Lower_tm27_intr() /* empty */

#endif