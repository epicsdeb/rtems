/*
 *  This routine is an implementation of the bsp_get_work_area()
 *  that can be used by all m68k BSPs following linkcmds conventions
 *  regarding heap, stack, and workspace allocation.
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: m68kbspgetworkarea.c,v 1.4 2008/08/19 14:39:21 joel Exp $
 */

#include <bsp.h>
#include <bsp/bootcard.h>

/*
 *  These are provided by the linkcmds of ALL M68K BSPs which use this file.
 */
extern char         _RamBase[];
extern char         _WorkspaceBase[];
extern char         _HeapSize[];
extern char         _RamSize[];

/*
 *  This method returns the base address and size of the area which
 *  is to be allocated between the RTEMS Workspace and the C Program
 *  Heap.
 */
void bsp_get_work_area(
  void   **work_area_start,
  size_t  *work_area_size,
  void   **heap_start,
  size_t  *heap_size
)
{
  *work_area_start       = _WorkspaceBase;
  *work_area_size       = (unsigned long)_RamBase + (unsigned long) _RamSize -
       (unsigned long)_WorkspaceBase;
  *heap_start = BSP_BOOTCARD_HEAP_USES_WORK_AREA;
  *heap_size = (size_t) _HeapSize;
}
