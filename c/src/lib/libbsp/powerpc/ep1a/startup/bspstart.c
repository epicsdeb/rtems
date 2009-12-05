/*
 *  This routine starts the application.  It includes application,
 *  board, and monitor specific initialization and configuration.
 *  The generic CPU dependent initialization has been performed
 *  before this routine is invoked.
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspstart.c,v 1.15.2.1 2009/10/16 16:42:02 joel Exp $
 */

#define SHOW_MORE_INIT_SETTINGS

#include <string.h>

#include <rtems/libio.h>
#include <rtems/libcsupport.h>
#include <bsp/consoleIo.h>
#include <libcpu/spr.h>
#include <bsp/residual.h>
#include <bsp/pci.h>
#include <bsp/openpic.h>
#include <bsp/irq.h>
#include <bsp/VME.h>
#include <bsp.h>
#include <libcpu/bat.h>
#include <libcpu/pte121.h>
#include <libcpu/cpuIdent.h>
#include <bsp/vectors.h>
#include <rtems/powerpc/powerpc.h>
#include <bsp/ppc_exc_bspsupp.h>

extern unsigned long __rtems_end[];
extern void L1_caches_enables(void);
extern unsigned get_L2CR(void);
extern void set_L2CR(unsigned);
extern void bsp_cleanup(void);
extern Triv121PgTbl BSP_pgtbl_setup(void);
extern void BSP_pgtbl_activate(Triv121PgTbl);
extern void BSP_vme_config(void);
extern void ShowBATS(void);
unsigned int rsPMCQ1Init(void);

uint32_t bsp_clicks_per_usec;

SPR_RW(SPRG1)

uint8_t LightIdx = 0;

void BSP_Increment_Light(void){
  uint8_t data;
  data = *GENERAL_REGISTER1;
  data &= 0xf0;
  data |= LightIdx++;
  *GENERAL_REGISTER1 = data;
}

void BSP_Fatal_Fault_Light(void) {
  uint8_t data;
  data = *GENERAL_REGISTER1;
  data &= 0xf0;
  data |= 0x7;
  while(1)
    *GENERAL_REGISTER1 = data;
}

void write_to_Q2ram(int offset, unsigned int data )
{
printk("0x%x ==> %d\n", offset, data );
#if 0
  unsigned int *ptr = 0x82000000;
  ptr += offset;
  *ptr = data;
#endif
}

/*
 * Vital Board data Start using DATA RESIDUAL
 */

uint32_t VME_Slot1 = FALSE;

/*
 * Total memory.
 * Note: RAM_END is defined in linkcmds.  We want to verify that the application
 *       is only using 10M of memory, and we do this by only accounting for this
 *       much memory.
 */
extern int   RAM_END;  
unsigned int BSP_mem_size = (unsigned int)&RAM_END;

/*
 * Where the heap starts; is used by bsp_pretasking_hook;
 */
unsigned int BSP_heap_start;
uint32_t BSP_intrStackStart;
uint32_t BSP_intrStackSize;

/*
 * PCI Bus Frequency
 */
unsigned int BSP_bus_frequency; 

/*
 * processor clock frequency
 */
unsigned int BSP_processor_frequency;

/*
 * Time base divisior (how many tick for 1 second).
 * Calibrated by outputing a 20 ms pulse.
 */
unsigned int BSP_time_base_divisor = 1320;  

/*
 * system init stack 
 */
#define INIT_STACK_SIZE 0x2000
#define INTR_STACK_SIZE rtems_configuration_get_interrupt_stack_size()

void BSP_panic(char *s)
{
  printk("%s PANIC %s\n",_RTEMS_version, s);
  __asm__ __volatile ("sc"); 
}

void _BSP_Fatal_error(unsigned int v)
{
  printk("%s PANIC ERROR %x\n",_RTEMS_version, v);
  __asm__ __volatile ("sc"); 
}
 
int BSP_FLASH_Disable_writes(
  uint32_t    area
)
{
  unsigned char    data;
  
  data = *GENERAL_REGISTER1;
  data |= DISABLE_USER_FLASH;
  *GENERAL_REGISTER1 = data;
                                                                            
  return RTEMS_SUCCESSFUL;
}

int BSP_FLASH_Enable_writes(
 uint32_t               area                           /* IN  */
)
{
  unsigned char    data;
                                                                                                                        
  data = *GENERAL_REGISTER1;
  data &= (~DISABLE_USER_FLASH);
  *GENERAL_REGISTER1 = data;
                                                                                                                        
  return RTEMS_SUCCESSFUL;
}

void BSP_FLASH_set_page(
  uint8_t  page
)
{
  unsigned char  data;
                                                                                                                        
  /* Set the flash page register. */
  data = *GENERAL_REGISTER2;
  data &= ~(BSP_FLASH_PAGE_MASK);
  data |= 0x80 | (page << BSP_FLASH_PAGE_SHIFT);
  *GENERAL_REGISTER2 = data;
}

/*
 *  Use the shared implementations of the following routines
 */
 
void bsp_libc_init( void *, uint32_t, int );


void ShowMemoryLayout(){
  extern unsigned long __bss_start[], __SBSS_START__[], __SBSS_END__[];
  extern unsigned long __SBSS2_START__[], __SBSS2_END__[];
  extern unsigned long __bss_start[];
  extern unsigned long __bss_end[];
  extern unsigned long __stack[];
  extern unsigned long __stackLow[];
  extern uint32_t end;
  /* extern uint32_t BSP_intrStackStart; */
  /* extern uint32_t BSP_intrStackSize; */
  /* Configuration.work_space_start     */
  /* rtems_configuration_get_work_space_size() */
  printk(" bss start: 0x%x\n", __bss_start);
  printk(" bss end: 0x%x\n",  __bss_end);
  printk(" rtems end: 0x%x\n",  __rtems_end);
  printk(" stack    : 0x%x\n",  __stack);
  printk(" stack Low: 0x%x\n",  __stackLow);
  printk(" end      : 0x%x\n",  &end);
  printk(" Intr Stack: 0x%x\n", BSP_intrStackStart);
  printk(" Intr Stack Size: 0x%x\n", BSP_intrStackSize);
  printk(" Heap start: %x\n", BSP_heap_start);

  printk(" workspace start: 0x%x\n", Configuration.work_space_start);
  printk(" workspace size: 0x%x\n", rtems_configuration_get_work_space_size() );

}





/*
 *  Function:   bsp_pretasking_hook
 *  Created:    95/03/10
 *
 *  Description:
 *      BSP pretasking hook.  Called just before drivers are initialized.
 *      Used to setup libc and install any BSP extensions.
 *
 *  NOTES:
 *      Must not use libc (to do io) from here, since drivers are
 *      not yet initialized.
 *
 */
 
void bsp_pretasking_hook(void)
{
  uint32_t        heap_start = BSP_heap_start;    
  uint32_t        heap_size;
  uint32_t        heap_sbrk_spared;
  extern uint32_t _bsp_sbrk_init(uint32_t, uint32_t*);
  extern uint32_t end;

  heap_start = (BSP_heap_start + CPU_ALIGNMENT - 1) & ~(CPU_ALIGNMENT-1);
  heap_size = (uint32_t) &RAM_END;
  heap_size = heap_size - heap_start - Configuration.work_space_size;
  heap_size &= 0xfffffff0;  /* keep it as a multiple of 16 bytes */

#if 0  /*XXXXXXX */
    heap_size = Configuration.work_space_start - (void *)&end;
    heap_size &= 0xfffffff0;  /* keep it as a multiple of 16 bytes */
#endif

#ifdef SHOW_MORE_INIT_SETTINGS
  printk(" HEAP start 0x%x  size %x \n", 
    heap_start, heap_size, 0 );
#endif    

  bsp_libc_init((void *)heap_start, heap_size, 0);
  rsPMCQ1Init();
  ShowMemoryLayout();
}

void zero_bss()
{
  /* prevent these from being accessed in the short data areas */
  extern unsigned long __bss_start[], __SBSS_START__[], __SBSS_END__[];
  extern unsigned long __SBSS2_START__[], __SBSS2_END__[];
  memset(__SBSS_START__, 0, ((unsigned) __SBSS_END__) - ((unsigned)__SBSS_START__));
  memset(__SBSS2_START__, 0, ((unsigned) __SBSS2_END__) - ((unsigned)__SBSS2_START__));
  memset(__bss_start, 0, ((unsigned) __rtems_end) - ((unsigned)__bss_start));
}

void save_boot_params(RESIDUAL* r3, void *r4, void* r5, char *additional_boot_options)
{
#if 0  
  residualCopy = *r3;
  strncpy(loaderParam, additional_boot_options, MAX_LOADER_ADD_PARM);
  loaderParam[MAX_LOADER_ADD_PARM - 1] ='\0';
#endif
}

unsigned int EUMBBAR;

unsigned int get_eumbbar() {
  out_le32( (uint32_t*)0xfec00000, 0x80000078 );
  return in_le32( (uint32_t*)0xfee00000 );
}

void Read_ep1a_config_registers( ppc_cpu_id_t myCpu ) {
  unsigned char value;

  /*
   * Print out the board and revision.
   */

  printk("Board:  ");
  printk( get_ppc_cpu_type_name(myCpu) );

  value = *BOARD_REVISION_REGISTER2 & HARDWARE_ID_MASK;
  if ( value == HARDWARE_ID_PPC5_EP1A )
    printk("  EP1A     ");
  else if ( value == HARDWARE_ID_EP1B ) 
    printk("  EP1B     ");
  else
    printk("  Unknown  ");
 
  value = *BOARD_REVISION_REGISTER2&0x1; 
  printk("Board ID %08x", value);
  if(value == 0x0){
    VME_Slot1 = TRUE;
    printk("VME Slot 1\n");
  }
  else{
    VME_Slot1 = FALSE;
    printk("\n");
  }

  printk("Revision: ");
  value = *BOARD_REVISION_REGISTER1;
  printk("%d%c\n\n", value>>4, 'A'+(value&BUILD_REVISION_MASK) );

  /*
   * Get the CPU, XXX frequency
   */
  value = *EQUIPMENT_PRESENT_REGISTER2 & PLL_CFG_MASK;
  switch( value ) {
    case MHZ_33_66_200:     /* PCI, MEM, & CPU Frequency */
      BSP_processor_frequency = 200000000;
      BSP_bus_frequency       =  33000000;
      break;
    case MHZ_33_100_200:   /* PCI, MEM, & CPU Frequency */
      BSP_processor_frequency = 200000000;
      BSP_bus_frequency       =  33000000;
      break;
    case MHZ_33_66_266:    /* PCI, MEM, & CPU Frequency */
      BSP_processor_frequency = 266000000;
      BSP_bus_frequency       =  33000000;
      break;
    case MHZ_33_66_333:   /* PCI, MEM, & CPU Frequency */
      BSP_processor_frequency = 333000000;
      BSP_bus_frequency       =  33000000;
      break;
    case MHZ_33_100_333:   /* PCI, MEM, & CPU Frequency */
      BSP_processor_frequency = 333000000;
      BSP_bus_frequency       =  33000000;
      break;
    case MHZ_33_100_350:   /* PCI, MEM, & CPU Frequency */
      BSP_processor_frequency = 350000000;
      BSP_bus_frequency       =  33000000;
      break;
    default:
      printk("ERROR: Unknown Processor frequency 0x%02x please fill in bspstart.c\n",value);
      BSP_processor_frequency = 350000000;
      BSP_bus_frequency       =  33000000;
      break;
  }
  printk("Processor Frequency %d  Bus Frequency: %d\n", BSP_processor_frequency, BSP_bus_frequency );
}

/*
 *  bsp_start
 *
 *  This routine does the bulk of the system initialization.
 */

void bsp_start( void )
{
  unsigned char *stack;
  unsigned char *work_space_start;
  ppc_cpu_id_t myCpu;
  ppc_cpu_revision_t myCpuRevision;
  Triv121PgTbl	pt=0;   /*  R = e; */

  /*
   * Get CPU identification dynamically. Note that the get_ppc_cpu_type() function
   * store the result in global variables so that it can be used latter...
   */
  BSP_Increment_Light();
  myCpu         = get_ppc_cpu_type();
  myCpuRevision = get_ppc_cpu_revision();

  EUMBBAR = get_eumbbar(); 
  printk("EUMBBAR 0x%08x\n", EUMBBAR );

  /*
   * Init MMU block address translation to enable hardware
   * access
   */
  setdbat(1, 0xf0000000, 0xf0000000, 0x10000000, IO_PAGE);
  setdbat(2, 0x80000000, 0x80000000, 0x10000000, IO_PAGE);
  setdbat(3, 0x90000000, 0x90000000, 0x10000000, IO_PAGE);
ShowBATS();

  /* 
   * Note this sets BSP_processor_frequency based upon register settings.
   * It must be done prior to setting up hooks.
   */
  Read_ep1a_config_registers( myCpu );
  bsp_clicks_per_usec = BSP_processor_frequency/(BSP_time_base_divisor * 1000);

#if 0   /* XXX - Add back in cache enable when we get this up and running!! */
  /*
   * enables L1 Cache. Note that the L1_caches_enables() codes checks for
   * relevant CPU type so that the reason why there is no use of myCpu...
   */
  L1_caches_enables();
#endif

  /*
   * Initialize the interrupt related settings.
   */
  BSP_intrStackStart = (uint32_t) __rtems_end + INIT_STACK_SIZE;
  BSP_intrStackSize = rtems_configuration_get_interrupt_stack_size();
  BSP_heap_start = BSP_intrStackStart + BSP_intrStackSize;
  printk("Interrupt Stack Start: 0x%x Size: 0x%x  Heap Start: 0x%x\n",
    BSP_intrStackStart, BSP_intrStackSize, BSP_heap_start
  );

  /* tag the bottom (T. Straumann 6/36/2001 <strauman@slac.stanford.edu>) */
  *((uint32_t *)BSP_intrStackStart) = 0;

  /*
   * Initialize default raw exception hanlders.
   */

#ifdef SHOW_MORE_INIT_SETTINGS
  printk("bsp_start: Initialize Exceptions\n");
#endif  
  ppc_exc_cache_wb_check = 0; 
  ppc_exc_initialize(
    PPC_INTERRUPT_DISABLE_MASK_DEFAULT,
    BSP_intrStackStart,
    BSP_intrStackSize
  );

#ifdef SHOW_MORE_INIT_SETTINGS
  printk("bsp_start: Going to start PCI buses scanning and initialization\n");
#endif  
  pci_initialize();

#ifdef SHOW_MORE_INIT_SETTINGS
  printk("Number of PCI buses found is : %d\n", pci_bus_count());
#endif
#ifdef TEST_RAW_EXCEPTION_CODE  
  printk("bsp_start: Testing exception handling Part 1\n");

  /*
   * Cause a software exception
   */
  __asm__ __volatile ("sc");

  /*
   * Check we can still catch exceptions and returned coorectly.
   */
  printk("bsp_start: Testing exception handling Part 2\n");
  __asm__ __volatile ("sc");
#endif  

#ifdef SHOW_MORE_INIT_SETTINGS
  printk("bsp_start: rtems_configuration_get_work_space_size() = %x\n", 
     rtems_configuration_get_work_space_size());
#endif  
  work_space_start = 
    (unsigned char *)&RAM_END - rtems_configuration_get_work_space_size();
  if ( work_space_start <= ((unsigned char *)__rtems_end) + 
        INIT_STACK_SIZE + rtems_configuration_get_interrupt_stack_size()) {
    printk( "bspstart: Not enough RAM!!!\n" );
    bsp_cleanup();
  }
  Configuration.work_space_start = work_space_start;
#ifdef SHOW_MORE_INIT_SETTINGS
  printk("bsp_start: workspace_start = 0x%x\n", 
     work_space_start);
#endif  

  #if ( PPC_USE_DATA_CACHE )
    #if DEBUG
      printk("bsp_start: cache_enable\n");
    #endif
    instruction_cache_enable ();
    data_cache_enable ();
    #if DEBUG
      printk("bsp_start: END PPC_USE_DATA_CACHE\n");
    #endif
  #endif


  /*
   * Initalize RTEMS IRQ system
   */
#ifdef  SHOW_MORE_INIT_SETTINGS
    printk("bspstart: Call BSP_rtems_irq_mng_init\n");
#endif
  BSP_rtems_irq_mng_init(0);
  
  /* Activate the page table mappings only after
   * initializing interrupts because the irq_mng_init()
   * routine needs to modify the text
   */           
  if (pt) {
#ifdef  SHOW_MORE_INIT_SETTINGS
    printk("bspstart: Page table setup finished; will activate it NOW...\n");
#endif
    BSP_pgtbl_activate(pt);
  }

  /*
   * Initialize VME bridge - needs working PCI
   * and IRQ subsystems...
   */
#ifdef SHOW_MORE_INIT_SETTINGS
  printk("bspstart: Going to initialize VME bridge\n");
#endif
  /* VME initialization is in a separate file so apps which don't use
   * VME or want a different configuration may link against a customized
   * routine.
   */
  BSP_vme_config();

#ifdef SHOW_MORE_INIT_SETTINGS
  ShowBATS();
  printk("Exit from bspstart\n");
#endif  
}
