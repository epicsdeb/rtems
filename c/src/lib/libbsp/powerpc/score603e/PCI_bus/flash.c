/*
 *
 *  COPYRIGHT (c) 1989, 1990, 1991, 1992, 1993, 1994, 1997.
 *  On-Line Applications Research Corporation (OAR).
 *
 * $Id: flash.c,v 1.5 2004/04/21 10:43:00 ralf Exp $
 */

#include <rtems.h>
#include <assert.h>
#include <stdio.h>

#include <bsp.h>
#include "PCI.h"
/*PAGE
 *
 *  SCORE603e_FLASH_Disable
 */

unsigned int SCORE603e_FLASH_Disable(
  uint32_t                       area                           /* IN  */
)
{
  uint8_t         value;

  value = *SCORE603E_BOARD_CTRL_REG;
  value = value | (~SCORE603E_BRD_FLASH_DISABLE_MASK);
  *SCORE603E_BOARD_CTRL_REG = value;

  return RTEMS_SUCCESSFUL;
}

unsigned int SCORE603e_FLASH_verify_enable()
{
  volatile uint8_t         *Ctrl_Status_Register =
           (void *)SCORE603E_BOARD_CTRL_REG;
  uint8_t          ctrl_value;
  uint32_t         pci_value;

  ctrl_value = *Ctrl_Status_Register;
  if ( ctrl_value & SCORE603E_BRD_FLASH_DISABLE_MASK ) {
    printf ("Flash Writes Disabled by board control register %x\n",
            ctrl_value );
    assert( 0x0 );
  }

  pci_value = Read_pci_device_register( 0x800000A8 );
  if (( pci_value & 0x00001000 ) == 0) {
    printf("Error PCI A8 \n");
    assert( 0x0 );
  }

  pci_value = Read_pci_device_register( 0x800000AC );
  if ( pci_value & 0x02000000) {
    printf("Error PCI AC \n");
    assert( 0x0 );
  }
  return RTEMS_SUCCESSFUL;
}

unsigned int SCORE603e_FLASH_pci_reset_reg(
  uint8_t          reg,
  uint32_t         cmask,
  uint32_t         mask
)
{
  uint32_t         pci_value;
  uint32_t         value;

  pci_value = Read_pci_device_register( reg );
  pci_value &= cmask;
  pci_value |= mask;
  Write_pci_device_register( reg, pci_value );
  value = Read_pci_device_register(  reg );
  if (value != pci_value) {
    printf("Error PCI %x wrote %x read %x\n", reg, pci_value, value);
  }
  return RTEMS_SUCCESSFUL;
}

/*PAGE
 *
 *  SCORE603e_FLASH_Enable_writes
 */
unsigned int SCORE603e_FLASH_Enable_writes(
  uint32_t                       area                           /* IN  */
)
{
  uint8_t          ctrl_value;
  uint32_t         pci_value;

  ctrl_value = *SCORE603E_BOARD_CTRL_REG;
  ctrl_value = ctrl_value & 0xbf;
  *SCORE603E_BOARD_CTRL_REG = ctrl_value;

  pci_value = Read_pci_device_register( 0x800000A8 );
  pci_value |= 0x00001000;
  Write_pci_device_register( 0x800000A8, pci_value );

  pci_value = Read_pci_device_register( 0x800000AC );
  pci_value &=  (~0x02000000);
  Write_pci_device_register( 0x000000AC, pci_value );

  return RTEMS_SUCCESSFUL;
}
