/**
 * @file rtems/score/unix.h
 */

/*
 *  This include file contains the definitions required by RTEMS
 *  which are typical for a modern UNIX computer using GCC.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: unix.h,v 1.8 2005/01/28 15:56:09 ralf Exp $
 */

#ifndef _RTEMS_SCORE_UNIX_H
#define _RTEMS_SCORE_UNIX_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  This file contains the information required to build
 *  RTEMS for a particular member of the "unix"
 *  family when executing in protected mode.  It does
 *  this by setting variables to indicate which implementation
 *  dependent features are present in a particular member
 *  of the family.
 */
 
#if defined(rtems_multilib)
/*
 *  Figure out all CPU Model Feature Flags based upon compiler 
 *  predefines. 
 */

#define CPU_MODEL_NAME  "rtems_multilib"
 
#elif defined(hpux)
 
#define CPU_MODEL_NAME  "HP-UX"
 
#elif defined(solaris2)
 
#define CPU_MODEL_NAME  "Solaris"
 
#elif defined(__linux__) || defined(linux)
 
#define CPU_MODEL_NAME  "Linux"
 
#elif defined(__CYGWIN__)
 
#define CPU_MODEL_NAME  "Cygwin"
 
#elif defined(__FreeBSD__)
 
#define CPU_MODEL_NAME  "FreeBSD"
 
#else
 
#error "Unsupported CPU Model"
 
#endif
 
#ifdef __cplusplus
}
#endif

#endif
