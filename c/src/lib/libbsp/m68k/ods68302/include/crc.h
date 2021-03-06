/*****************************************************************************/
/*
  $Id: crc.h,v 1.3 2004/04/21 10:42:54 ralf Exp $

   CRC 16 Calculate Interface

*/
/*****************************************************************************/

#ifndef _CRC_H_
#define _CRC_H_

 /* ----
      F U N C T I O N S
  */
#if __cplusplus
extern "C"
{
#endif

uint16_t         calc_crc(void *data, uint32_t         count);

#if __cplusplus
}
#endif
#endif
