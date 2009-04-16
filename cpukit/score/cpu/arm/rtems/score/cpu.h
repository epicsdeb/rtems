/**
 * @file rtems/score/cpu.h
 */

/*
 * $Id: cpu.h,v 1.36 2008/09/05 01:54:26 ralf Exp $
 *
 *  This include file contains information pertaining to the ARM
 *  processor.
 *
 *  Copyright (c) 2007 Ray Xu <Rayx.cn@gmail.com>
 *
 *  Copyright (c) 2006 OAR Corporation
 *
 *  Copyright (c) 2002 Advent Networks, Inc.
 *        Jay Monkman <jmonkman@adventnetworks.com>
 *
 *  COPYRIGHT (c) 2000 Canon Research Centre France SA.
 *  Emmanuel Raguet, mailto:raguet@crf.canon.fr
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 */

/* FIXME: finish commenting/cleaning up this file */
#ifndef _RTEMS_SCORE_CPU_H
#define _RTEMS_SCORE_CPU_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/score/arm.h>            /* pick up machine definitions */
#ifndef ASM
#include <rtems/score/types.h>
#endif

/* conditional compilation parameters */

/*
 *  Should the calls to _Thread_Enable_dispatch be inlined?
 *
 *  If TRUE, then they are inlined.
 *  If FALSE, then a subroutine call is made.
 *
 *  Basically this is an example of the classic trade-off of size
 *  versus speed.  Inlining the call (TRUE) typically increases the
 *  size of RTEMS while speeding up the enabling of dispatching.
 *  [NOTE: In general, the _Thread_Dispatch_disable_level will
 *  only be 0 or 1 unless you are in an interrupt handler and that
 *  interrupt handler invokes the executive.]  When not inlined
 *  something calls _Thread_Enable_dispatch which in turns calls
 *  _Thread_Dispatch.  If the enable dispatch is inlined, then
 *  one subroutine call is avoided entirely.]
 */

#if defined(__thumb__)
#define CPU_INLINE_ENABLE_DISPATCH       FALSE
#else
#define CPU_INLINE_ENABLE_DISPATCH       TRUE
#endif

/*
 *  Should the body of the search loops in _Thread_queue_Enqueue_priority
 *  be unrolled one time?  In unrolled each iteration of the loop examines
 *  two "nodes" on the chain being searched.  Otherwise, only one node
 *  is examined per iteration.
 *
 *  If TRUE, then the loops are unrolled.
 *  If FALSE, then the loops are not unrolled.
 *
 *  The primary factor in making this decision is the cost of disabling
 *  and enabling interrupts (_ISR_Flash) versus the cost of rest of the
 *  body of the loop.  On some CPUs, the flash is more expensive than
 *  one iteration of the loop body.  In this case, it might be desirable
 *  to unroll the loop.  It is important to note that on some CPUs, this
 *  code is the longest interrupt disable period in RTEMS.  So it is
 *  necessary to strike a balance when setting this parameter.
 */

#define CPU_UNROLL_ENQUEUE_PRIORITY      TRUE

/*
 *  Does RTEMS manage a dedicated interrupt stack in software?
 *
 *  If TRUE, then a stack is allocated in _Interrupt_Manager_initialization.
 *  If FALSE, nothing is done.
 *
 *  If the CPU supports a dedicated interrupt stack in hardware,
 *  then it is generally the responsibility of the BSP to allocate it
 *  and set it up.
 *
 *  If the CPU does not support a dedicated interrupt stack, then
 *  the porter has two options: (1) execute interrupts on the
 *  stack of the interrupted task, and (2) have RTEMS manage a dedicated
 *  interrupt stack.
 *
 *  If this is TRUE, CPU_ALLOCATE_INTERRUPT_STACK should also be TRUE.
 *
 *  Only one of CPU_HAS_SOFTWARE_INTERRUPT_STACK and
 *  CPU_HAS_HARDWARE_INTERRUPT_STACK should be set to TRUE.  It is
 *  possible that both are FALSE for a particular CPU.  Although it
 *  is unclear what that would imply about the interrupt processing
 *  procedure on that CPU.
 */

#define CPU_HAS_SOFTWARE_INTERRUPT_STACK FALSE

/*
 *  Does this CPU have hardware support for a dedicated interrupt stack?
 *
 *  If TRUE, then it must be installed during initialization.
 *  If FALSE, then no installation is performed.
 *
 *  If this is TRUE, CPU_ALLOCATE_INTERRUPT_STACK should also be TRUE.
 *
 *  Only one of CPU_HAS_SOFTWARE_INTERRUPT_STACK and
 *  CPU_HAS_HARDWARE_INTERRUPT_STACK should be set to TRUE.  It is
 *  possible that both are FALSE for a particular CPU.  Although it
 *  is unclear what that would imply about the interrupt processing
 *  procedure on that CPU.
 */

#define CPU_HAS_HARDWARE_INTERRUPT_STACK TRUE

/*
 *  Does RTEMS allocate a dedicated interrupt stack in the Interrupt Manager?
 *
 *  If TRUE, then the memory is allocated during initialization.
 *  If FALSE, then the memory is allocated during initialization.
 *
 *  This should be TRUE is CPU_HAS_SOFTWARE_INTERRUPT_STACK is TRUE.
 */

#define CPU_ALLOCATE_INTERRUPT_STACK FALSE

/*
 *  Does the RTEMS invoke the user's ISR with the vector number and
 *  a pointer to the saved interrupt frame (1) or just the vector 
 *  number (0)?
 */

#define CPU_ISR_PASSES_FRAME_POINTER 0

/*
 *  Does the CPU have hardware floating point?
 *
 *  If TRUE, then the RTEMS_FLOATING_POINT task attribute is supported.
 *  If FALSE, then the RTEMS_FLOATING_POINT task attribute is ignored.
 *
 *  If there is a FP coprocessor such as the i387 or mc68881, then
 *  the answer is TRUE.
 *
 *  The macro name "ARM_HAS_FPU" should be made CPU specific.
 *  It indicates whether or not this CPU model has FP support.  For
 *  example, it would be possible to have an i386_nofp CPU model
 *  which set this to false to indicate that you have an i386 without
 *  an i387 and wish to leave floating point support out of RTEMS.
 */

#if ( ARM_HAS_FPU == 1 )
#define CPU_HARDWARE_FP     TRUE
#else
#define CPU_HARDWARE_FP     FALSE
#endif

#define CPU_SOFTWARE_FP     FALSE

/*
 *  Are all tasks RTEMS_FLOATING_POINT tasks implicitly?
 *
 *  If TRUE, then the RTEMS_FLOATING_POINT task attribute is assumed.
 *  If FALSE, then the RTEMS_FLOATING_POINT task attribute is followed.
 *
 *  If CPU_HARDWARE_FP is FALSE, then this should be FALSE as well.
 */

#define CPU_ALL_TASKS_ARE_FP     FALSE

/*
 *  Should the IDLE task have a floating point context?
 *
 *  If TRUE, then the IDLE task is created as a RTEMS_FLOATING_POINT task
 *  and it has a floating point context which is switched in and out.
 *  If FALSE, then the IDLE task does not have a floating point context.
 *
 *  Setting this to TRUE negatively impacts the time required to preempt
 *  the IDLE task from an interrupt because the floating point context
 *  must be saved as part of the preemption.
 */

#define CPU_IDLE_TASK_IS_FP      FALSE

/*
 *  Should the saving of the floating point registers be deferred
 *  until a context switch is made to another different floating point
 *  task?
 *
 *  If TRUE, then the floating point context will not be stored until
 *  necessary.  It will remain in the floating point registers and not
 *  disturned until another floating point task is switched to.
 *
 *  If FALSE, then the floating point context is saved when a floating
 *  point task is switched out and restored when the next floating point
 *  task is restored.  The state of the floating point registers between
 *  those two operations is not specified.
 *
 *  If the floating point context does NOT have to be saved as part of
 *  interrupt dispatching, then it should be safe to set this to TRUE.
 *
 *  Setting this flag to TRUE results in using a different algorithm
 *  for deciding when to save and restore the floating point context.
 *  The deferred FP switch algorithm minimizes the number of times
 *  the FP context is saved and restored.  The FP context is not saved
 *  until a context switch is made to another, different FP task.
 *  Thus in a system with only one FP task, the FP context will never
 *  be saved or restored.
 */

#define CPU_USE_DEFERRED_FP_SWITCH   FALSE

/*
 *  Does this port provide a CPU dependent IDLE task implementation?
 *
 *  If TRUE, then the routine _CPU_Thread_Idle_body
 *  must be provided and is the default IDLE thread body instead of
 *  _CPU_Thread_Idle_body.
 *
 *  If FALSE, then use the generic IDLE thread body if the BSP does
 *  not provide one.
 *
 *  This is intended to allow for supporting processors which have
 *  a low power or idle mode.  When the IDLE thread is executed, then
 *  the CPU can be powered down.
 *
 *  The order of precedence for selecting the IDLE thread body is:
 *
 *    1.  BSP provided
 *    2.  CPU dependent (if provided)
 *    3.  generic (if no BSP and no CPU dependent)
 */

#define CPU_PROVIDES_IDLE_THREAD_BODY    FALSE

/*
 *  Does the stack grow up (toward higher addresses) or down
 *  (toward lower addresses)?
 *
 *  If TRUE, then the grows upward.
 *  If FALSE, then the grows toward smaller addresses.
 */

#define CPU_STACK_GROWS_UP               FALSE

/*
 *  The following is the variable attribute used to force alignment
 *  of critical RTEMS structures.  On some processors it may make
 *  sense to have these aligned on tighter boundaries than
 *  the minimum requirements of the compiler in order to have as
 *  much of the critical data area as possible in a cache line.
 *
 *  The placement of this macro in the declaration of the variables
 *  is based on the syntactically requirements of the GNU C
 *  "__attribute__" extension.  For example with GNU C, use
 *  the following to force a structures to a 32 byte boundary.
 *
 *      __attribute__ ((aligned (32)))
 *
 *  NOTE:  Currently only the Priority Bit Map table uses this feature.
 *         To benefit from using this, the data must be heavily
 *         used so it will stay in the cache and used frequently enough
 *         in the executive to justify turning this on.
 */

#define CPU_STRUCTURE_ALIGNMENT  __attribute__ ((aligned (32)))

/*
 *  Define what is required to specify how the network to host conversion
 *  routines are handled.
 */

#if defined(__ARMEL__)
#define CPU_BIG_ENDIAN          FALSE
#define CPU_LITTLE_ENDIAN       TRUE
#elif defined(__ARMEB__)
#define CPU_BIG_ENDIAN          TRUE
#define CPU_LITTLE_ENDIAN       FALSE
#else
#error "Unknown endianness"
#endif
			
/*
 *  The following defines the number of bits actually used in the
 *  interrupt field of the task mode.  How those bits map to the
 *  CPU interrupt levels is defined by the routine _CPU_ISR_Set_level().
 */

#define CPU_MODES_INTERRUPT_MASK   0x000000c0

/*
 *  Processor defined structures required by cpukit/score.
 */

/* may need to put some structures here.  */

/*
 * Contexts
 *
 *  Generally there are 2 types of context to save.
 *     1. Interrupt registers to save
 *     2. Task level registers to save
 *
 *  This means we have the following 3 context items:
 *     1. task level context stuff::  Context_Control
 *     2. floating point task stuff:: Context_Control_fp
 *     3. special interrupt level context :: Context_Control_interrupt
 *
 *  On some processors, it is cost-effective to save only the callee
 *  preserved registers during a task context switch.  This means
 *  that the ISR code needs to save those registers which do not
 *  persist across function calls.  It is not mandatory to make this
 *  distinctions between the caller/callee saves registers for the
 *  purpose of minimizing context saved during task switch and on interrupts.
 *  If the cost of saving extra registers is minimal, simplicity is the
 *  choice.  Save the same context on interrupt entry as for tasks in
 *  this case.
 *
 *  Additionally, if gdb is to be made aware of RTEMS tasks for this CPU, then
 *  care should be used in designing the context area.
 *
 *  On some CPUs with hardware floating point support, the Context_Control_fp
 *  structure will not be used or it simply consist of an array of a
 *  fixed number of bytes.   This is done when the floating point context
 *  is dumped by a "FP save context" type instruction and the format
 *  is not really defined by the CPU.  In this case, there is no need
 *  to figure out the exact format -- only the size.  Of course, although
 *  this is enough information for RTEMS, it is probably not enough for
 *  a debugger such as gdb.  But that is another problem.
 */
typedef struct {
    uint32_t   register_cpsr;
    uint32_t   register_r4;
    uint32_t   register_r5;
    uint32_t   register_r6;
    uint32_t   register_r7;
    uint32_t   register_r8;
    uint32_t   register_r9;
    uint32_t   register_r10;
    uint32_t   register_fp;
    uint32_t   register_sp;
    uint32_t   register_lr;
    uint32_t   register_pc;
} Context_Control;

#define _CPU_Context_Get_SP( _context ) \
  (_context)->register_sp

typedef struct {
    double      some_float_register;
} Context_Control_fp;

typedef struct {
    uint32_t   register_r0;
    uint32_t   register_r1;
    uint32_t   register_r2;
    uint32_t   register_r3;
    uint32_t   register_ip;
    uint32_t   register_lr;
} CPU_Exception_frame;

typedef void (*cpuExcHandlerType) (CPU_Exception_frame*);
extern cpuExcHandlerType _currentExcHandler;
extern void rtems_exception_init_mngt(void);
  
/*
 *  The following structure defines the set of information saved
 *  on the current stack by RTEMS upon receipt of each interrupt
 *  that will lead to re-enter the kernel to signal the thread.
 */

typedef CPU_Exception_frame CPU_Interrupt_frame;

/*
 *  This variable is optional.  It is used on CPUs on which it is difficult
 *  to generate an "uninitialized" FP context.  It is filled in by
 *  _CPU_Initialize and copied into the task's FP context area during
 *  _CPU_Context_Initialize.
 */

SCORE_EXTERN Context_Control_fp  _CPU_Null_fp_context;

/*
 *  The size of the floating point context area.  On some CPUs this
 *  will not be a "sizeof" because the format of the floating point
 *  area is not defined -- only the size is.  This is usually on
 *  CPUs with a "floating point save context" instruction.
 */

#define CPU_CONTEXT_FP_SIZE sizeof( Context_Control_fp )

/*
 *  Amount of extra stack (above minimum stack size) required by
 *  MPCI receive server thread.  Remember that in a multiprocessor
 *  system this thread must exist and be able to process all directives.
 */

#define CPU_MPCI_RECEIVE_SERVER_EXTRA_STACK 0

/*
 *  This defines the number of entries in the ISR_Vector_table managed
 *  by RTEMS.
 */

#define CPU_INTERRUPT_NUMBER_OF_VECTORS      8
#define CPU_INTERRUPT_MAXIMUM_VECTOR_NUMBER  (CPU_INTERRUPT_NUMBER_OF_VECTORS - 1)

/*
 *  This is defined if the port has a special way to report the ISR nesting
 *  level.  Most ports maintain the variable _ISR_Nest_level.
 */

#define CPU_PROVIDES_ISR_IS_IN_PROGRESS FALSE

/*
 *  Should be large enough to run all RTEMS tests.  This ensures
 *  that a "reasonable" small application should not have any problems.
 */

#define CPU_STACK_MINIMUM_SIZE          (1024*4)

/*
 *  CPU's worst alignment requirement for data types on a byte boundary.  This
 *  alignment does not take into account the requirements for the stack.
 */

#define CPU_ALIGNMENT              4

/*
 *  This number corresponds to the byte alignment requirement for the
 *  heap handler.  This alignment requirement may be stricter than that
 *  for the data types alignment specified by CPU_ALIGNMENT.  It is
 *  common for the heap to follow the same alignment requirement as
 *  CPU_ALIGNMENT.  If the CPU_ALIGNMENT is strict enough for the heap,
 *  then this should be set to CPU_ALIGNMENT.
 *
 *  NOTE:  This does not have to be a power of 2.  It does have to
 *         be greater or equal to than CPU_ALIGNMENT.
 */

#define CPU_HEAP_ALIGNMENT         CPU_ALIGNMENT

/*
 *  This number corresponds to the byte alignment requirement for memory
 *  buffers allocated by the partition manager.  This alignment requirement
 *  may be stricter than that for the data types alignment specified by
 *  CPU_ALIGNMENT.  It is common for the partition to follow the same
 *  alignment requirement as CPU_ALIGNMENT.  If the CPU_ALIGNMENT is strict
 *  enough for the partition, then this should be set to CPU_ALIGNMENT.
 *
 *  NOTE:  This does not have to be a power of 2.  It does have to
 *         be greater or equal to than CPU_ALIGNMENT.
 */

#define CPU_PARTITION_ALIGNMENT    CPU_ALIGNMENT

/*
 *  This number corresponds to the byte alignment requirement for the
 *  stack.  This alignment requirement may be stricter than that for the
 *  data types alignment specified by CPU_ALIGNMENT.  If the CPU_ALIGNMENT
 *  is strict enough for the stack, then this should be set to 0.
 *
 *  NOTE:  This must be a power of 2 either 0 or greater than CPU_ALIGNMENT.
 */

#define CPU_STACK_ALIGNMENT        4

/* ISR handler macros */

/*
 *  Support routine to initialize the RTEMS vector table after it is allocated.
 */

#define _CPU_Initialize_vectors() 

/*
 *  Disable all interrupts for an RTEMS critical section.  The previous
 *  level is returned in _level.
 */
#if (defined(__THUMB_INTERWORK__) || defined(__thumb__))

extern uint32_t _CPU_ISR_Disable_Thumb(void) __attribute__ ((naked));
extern void _CPU_ISR_Enable_Thumb( int ) __attribute__ ((naked));
extern void _CPU_ISR_Flash_Thumb(int) __attribute__ ((naked));
extern void _CPU_ISR_Set_level_Thumb(int ) __attribute__ ((naked));
extern uint32_t _CPU_ISR_Get_level_Thumb(void ) __attribute__ ((naked));

#define _CPU_ISR_Disable(_level)	\
 (_level) = _CPU_ISR_Disable_Thumb()

#define _CPU_ISR_Enable(a) 	_CPU_ISR_Enable_Thumb(a)

#define _CPU_ISR_Flash(a)	_CPU_ISR_Flash_Thumb(a)

#define _CPU_ISR_Set_level(a) 	_CPU_ISR_Set_level_Thumb(a)

#define _CPU_ISR_Get_level(a) 	_CPU_ISR_Get_level_Thumb(a)

#else /*For ARM mode*/
#define _CPU_ISR_Disable( _level )                \
  {                                               \
    int reg;                                       \
    asm volatile ("MRS	%0, cpsr \n"               \
                  "ORR  %1, %0, #0xc0 \n"          \
                  "MSR  cpsr, %1 \n"               \
                   : "=&r" (_level), "=&r" (reg)); \
  }

/*
 *  Enable interrupts to the previous level (returned by _CPU_ISR_Disable).
 *  This indicates the end of an RTEMS critical section.  The parameter
 *  _level is not modified.
 */

#define _CPU_ISR_Enable( _level )               \
  {                                             \
    asm volatile ("MSR  cpsr, %0 \n"            \
                  : : "r" (_level));            \
  }

/*
 *  This temporarily restores the interrupt to _level before immediately
 *  disabling them again.  This is used to divide long RTEMS critical
 *  sections into two or more parts.  The parameter _level is not
 * modified.
 */

#define _CPU_ISR_Flash( _level ) \
  { \
    int reg;                                    \
    asm volatile ("MRS	%0, cpsr \n"            \
                  "MSR  cpsr, %1 \n"            \
                  "MSR  cpsr, %0 \n"            \
                  : "=&r" (reg)                 \
                  : "r" (_level));              \
  }

/*
 *  Map interrupt level in task mode onto the hardware that the CPU
 *  actually provides.  Currently, interrupt levels which do not
 *  map onto the CPU in a generic fashion are undefined.  Someday,
 *  it would be nice if these were "mapped" by the application
 *  via a callout.  For example, m68k has 8 levels 0 - 7, levels
 *  8 - 255 would be available for bsp/application specific meaning.
 *  This could be used to manage a programmable interrupt controller
 *  via the rtems_task_mode directive.
 *
 *  The get routine usually must be implemented as a subroutine.
 */

#define _CPU_ISR_Set_level( new_level )         \
  {                                             \
    int reg = 0; /* to avoid warning */         \
    asm volatile ("MRS	%0, cpsr \n"            \
                  "BIC  %0, %0, #0xc0 \n"       \
                  "ORR  %0, %0, %2 \n"          \
                  "MSR  cpsr_c, %0 \n"          \
                  : "=r" (reg)                  \
                  : "0" (reg), "r" (new_level)); \
  }

#endif /*(defined(__THUMB_INTERWORK__) || defined(__thumb__))*/

uint32_t   _CPU_ISR_Get_level( void );

/* end of ISR handler macros */

/* Context handler macros */

/*
 *  Initialize the context to a state suitable for starting a
 *  task after a context restore operation.  Generally, this
 *  involves:
 *
 *     - setting a starting address
 *     - preparing the stack
 *     - preparing the stack and frame pointers
 *     - setting the proper interrupt level in the context
 *     - initializing the floating point context
 *
 *  This routine generally does not set any unnecessary register
 *  in the context.  The state of the "general data" registers is
 *  undefined at task start time.
 *
 *  NOTE: This is_fp parameter is TRUE if the thread is to be a floating
 *        point thread.  This is typically only used on CPUs where the
 *        FPU may be easily disabled by software such as on the SPARC
 *        where the PSR contains an enable FPU bit.
 */

void _CPU_Context_Initialize(
  Context_Control  *the_context,
  uint32_t         *stack_base,
  uint32_t          size,
  uint32_t          new_level,
  void             *entry_point,
  bool              is_fp
);

/*
 *  This routine is responsible for somehow restarting the currently
 *  executing task.  If you are lucky, then all that is necessary
 *  is restoring the context.  Otherwise, there will need to be
 *  a special assembly routine which does something special in this
 *  case.  Context_Restore should work most of the time.  It will
 *  not work if restarting self conflicts with the stack frame
 *  assumptions of restoring a context.
 */

#define _CPU_Context_Restart_self( _the_context ) \
   _CPU_Context_restore( (_the_context) );

/*
 *  The purpose of this macro is to allow the initial pointer into
 *  a floating point context area (used to save the floating point
 *  context) to be at an arbitrary place in the floating point
 *  context area.
 *
 *  This is necessary because some FP units are designed to have
 *  their context saved as a stack which grows into lower addresses.
 *  Other FP units can be saved by simply moving registers into offsets
 *  from the base of the context area.  Finally some FP units provide
 *  a "dump context" instruction which could fill in from high to low
 *  or low to high based on the whim of the CPU designers.
 */

#define _CPU_Context_Fp_start( _base, _offset ) \
   ( (void *) _Addresses_Add_offset( (_base), (_offset) ) )

/*
 *  This routine initializes the FP context area passed to it to.
 *  There are a few standard ways in which to initialize the
 *  floating point context.  The code included for this macro assumes
 *  that this is a CPU in which a "initial" FP context was saved into
 *  _CPU_Null_fp_context and it simply copies it to the destination
 *  context passed to it.
 *
 *  Other models include (1) not doing anything, and (2) putting
 *  a "null FP status word" in the correct place in the FP context.
 */

#define _CPU_Context_Initialize_fp( _destination ) \
  { \
   *(*(_destination)) = _CPU_Null_fp_context; \
  }

/* end of Context handler macros */

/* Fatal Error manager macros */

/*
 *  This routine copies _error into a known place -- typically a stack
 *  location or a register, optionally disables interrupts, and
 *  halts/stops the CPU.
 */

#define _CPU_Fatal_halt( _error )           \
   do {                                     \
     int _level;                            \
     _CPU_ISR_Disable( _level );            \
     asm volatile ("mov r0, %0\n"           \
                   : "=r" (_error)          \
                   : "0" (_error)           \
                   : "r0" );                \
     while(1) ;                             \
   } while(0);
 

/* end of Fatal Error manager macros */

/* Bitfield handler macros */
/* 
 * If we had a particularly fast function for finding the first
 * bit set in a word, it would go here. Since we don't (*), we'll
 * just use the universal macros.
 *
 * (*) On ARM V5 and later, there's a CLZ function which could be
 *     used to implement much quicker than the default macro.
 */
#define CPU_USE_GENERIC_BITFIELD_CODE TRUE
#define CPU_USE_GENERIC_BITFIELD_DATA TRUE

/* functions */

/*
 *  _CPU_Initialize
 *
 *  This routine performs CPU dependent initialization.
 */

void _CPU_Initialize(
  void      (*thread_dispatch)
);

typedef enum {
  ARM_EXCEPTION_RESET      = 0,
  ARM_EXCEPTION_UNDEF      = 1,
  ARM_EXCEPTION_SWI        = 2,
  ARM_EXCEPTION_PREF_ABORT = 3,
  ARM_EXCEPTION_DATA_ABORT = 4,
  ARM_EXCEPTION_RESERVED   = 5,
  ARM_EXCEPTION_IRQ        = 6,
  ARM_EXCEPTION_FIQ        = 7,
  MAX_EXCEPTIONS           = 8
} Arm_symbolic_exception_name;

/*
 *  _CPU_ISR_install_vector
 *
 *  This routine installs an interrupt vector.
 */

void _CPU_ISR_install_vector(
  uint32_t    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
);

/*
 *  _CPU_Install_interrupt_stack
 *
 *  This routine installs the hardware interrupt stack pointer.
 *
 *  NOTE:  It need only be provided if CPU_HAS_HARDWARE_INTERRUPT_STACK
 *         is TRUE.
 */

void _CPU_Install_interrupt_stack( void );

/*
 *  _CPU_Context_switch
 *
 *  This routine switches from the run context to the heir context.
 */

void _CPU_Context_switch(
  Context_Control  *run,
  Context_Control  *heir
);

/*
 *  _CPU_Context_restore
 *
 *  This routine is generally used only to restart self in an
 *  efficient manner.  It may simply be a label in _CPU_Context_switch.
 *
 *  NOTE: May be unnecessary to reload some registers.
 */

void _CPU_Context_restore(
  Context_Control *new_context
);

#if (ARM_HAS_FPU == 1)
/*
 *  _CPU_Context_save_fp
 *
 *  This routine saves the floating point context passed to it.
 */

void _CPU_Context_save_fp(
  Context_Control_fp **fp_context_ptr
);

/*
 *  _CPU_Context_restore_fp
 *
 *  This routine restores the floating point context passed to it.
 */

void _CPU_Context_restore_fp(
  Context_Control_fp **fp_context_ptr
);
#endif /* (ARM_HAS_FPU == 1) */

/*  The following routine swaps the endian format of an unsigned int.
 *  It must be static because it is referenced indirectly.
 *
 *  This version will work on any processor, but if there is a better
 *  way for your CPU PLEASE use it.  The most common way to do this is to:
 *
 *     swap least significant two bytes with 16-bit rotate
 *     swap upper and lower 16-bits
 *     swap most significant two bytes with 16-bit rotate
 *
 *  Some CPUs have special instructions which swap a 32-bit quantity in
 *  a single instruction (e.g. i486).  It is probably best to avoid
 *  an "endian swapping control bit" in the CPU.  One good reason is
 *  that interrupts would probably have to be disabled to ensure that
 *  an interrupt does not try to access the same "chunk" with the wrong
 *  endian.  Another good reason is that on some CPUs, the endian bit
 *  endianness for ALL fetches -- both code and data -- so the code
 *  will be fetched incorrectly.
 */
 
static inline uint32_t CPU_swap_u32(
  uint32_t value
)
{
#if defined(__thumb__)
  uint32_t byte1, byte2, byte3, byte4, swapped;
 
  byte4 = (value >> 24) & 0xff;
  byte3 = (value >> 16) & 0xff;
  byte2 = (value >> 8)  & 0xff;
  byte1 =  value        & 0xff;
 
  swapped = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
  return swapped;
#else
  uint32_t   tmp = value; /* make compiler warnings go away */
  asm volatile ("EOR   %1, %0, %0, ROR #16\n" 
                "BIC   %1, %1, #0xff0000\n"   
                "MOV   %0, %0, ROR #8\n"      
                "EOR   %0, %0, %1, LSR #8\n"  
                : "=r" (value), "=r" (tmp)  
                : "0" (value), "1" (tmp));
  return value;
#endif
}

static inline uint16_t   CPU_swap_u16(uint16_t   value)
{
    uint16_t   lower;
    uint16_t   upper;

    value = value & (uint16_t  ) 0xffff;
    lower = (value >> 8) ;
    upper = (value << 8) ;

    return (lower | upper);
}

#ifdef __cplusplus
}
#endif

#endif
