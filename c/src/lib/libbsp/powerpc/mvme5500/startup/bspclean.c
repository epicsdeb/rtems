#include <bsp.h>
#include <rtems/bspIo.h>
#include <libcpu/stackTrace.h>

#define AUTO_BOOT 0

void bsp_cleanup(void)
{
#if AUTO_BOOT
  /* Till Straumann <strauman@slac.stanford.edu> for SVGM */
  void bsp_reset();

  bsp_reset();
#else
  /* Kate Feng <feng1@bnl.gov> for the MVME5500 */
  printk("\nPrinting a stack trace for your convenience :-)\n");
  CPU_print_stack();
  printk("RTEMS terminated; Boot manually or turn on AUTO_BOOT.\n");
#endif
}
