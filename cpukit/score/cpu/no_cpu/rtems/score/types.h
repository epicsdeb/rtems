/**
 * @file rtems/score/types.h
 */

/*
 *  This include file contains type definitions pertaining to the Intel
 *  no_cpu processor family.
 *
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: types.h,v 1.17 2008/12/11 00:45:34 ralf Exp $
 */

#ifndef _RTEMS_SCORE_TYPES_H
#define _RTEMS_SCORE_TYPES_H

#ifndef ASM

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  This section defines the basic types for this processor.
 */

/** This defines the type for a priority bit map entry. */
typedef uint16_t Priority_Bit_map_control;

/** This defines the return type for an ISR entry point. */
typedef void no_cpu_isr;

/** This defines the prototype for an ISR entry point. */
typedef no_cpu_isr ( *no_cpu_isr_entry )( void );

#ifdef __cplusplus
}
#endif

#endif  /* !ASM */

#endif
