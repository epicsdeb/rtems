/*
 * This file contains the RTC driver table for Motorola shared BSPs.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 *
 * $Id: todcfg.c,v 1.5 2005/05/10 02:46:41 ralf Exp $
 */

#include <bsp.h>
#include <libcpu/io.h>
#include <libchip/rtc.h>
#include <libchip/m48t08.h>

/* Forward function declaration */
#if !defined(mvme2100)
uint32_t mvmertc_get_register( uint32_t, uint8_t );
void mvmertc_set_register( uint32_t, uint8_t, uint32_t );
#endif

#ifdef UEIPAC83XX
static rtc_fns ds1390_fns;
#endif

/* The following table configures the RTC drivers used in this BSP */
rtc_tbl RTC_Table[] = {
    {
        "/dev/rtc",                /* sDeviceName */
#ifndef UEIPAC83XX
        RTC_M48T08,                /* deviceType  -- actually M48T59 */
        &m48t08_fns,               /* pDeviceFns */
#else
        RTC_CUSTOM,
        &ds1390_fns,
#endif
        rtc_probe,                 /* deviceProbe */
        NULL,                      /* pDeviceParams */
#if defined(mvme2100)
        0xFFE81ff8,                /* ulCtrlPort1 */
        0x00,                      /* ulDataPort */
        m48t08_get_register,       /* getRegister */
        m48t08_set_register        /* setRegister */
#else
#if defined(UEIPAC83XX)
        0xA00D0000,                /* ulCtrlPort1 */
#else
        0xFFE81ff8,                /* ulCtrlPort1 */
#endif
        0x00,                      /* ulDataPort */
        mvmertc_get_register,      /* getRegister */
        mvmertc_set_register       /* setRegister */
#endif
    }
};

/* Some information used by the RTC driver */

#define NUM_RTCS (sizeof(RTC_Table)/sizeof(rtc_tbl))

size_t RTC_Count = NUM_RTCS;

rtems_device_minor_number RTC_Minor;

#if !defined(mvme2100)
#include <rtems/bspIo.h>
void mvmertc_set_register(
  uint32_t base,
  uint8_t  reg,
  uint32_t value
)
{
  printk( "RTC SUPPORT NOT IMPLEMENTED ON THIS BOARD\n");
}

uint32_t mvmertc_get_register(
  uint32_t base,
  uint8_t  reg
)
{
  printk( "RTC SUPPORT NOT IMPLEMENTED ON THIS BOARD\n");
  return 0;
}
#endif

#if defined(UEIPAC83XX)
/* Based on UEIPAC 834x Linux BSP
 * Contact: Frederic Villeneuve <fvilleneuve@ueidaq.com>
 */
#define DQL_CLI_LSR     (0x0000)     /* R layer status register */
#define DQL_CLI_LCR     (0x0000)     /* W layer control register */
#define DQL_CLI_ISDR    (0x0004)     /* R isolated side RX data register */
#define DQL_CLI_ISTR    (0x0004)     /* W isolated side TX transmit register */
#define DQL_CLI_LID     (0x0008)     /* R layer identification {u16 BOM,u16 Model} */


#define DQL_LSR_IDR     (1UL<<27)  /* IS-data ready */
#define DQL_LSR_IBT     (1UL<<26)  /* IS-interface TX busy */
#define DQL_LSR_IBR     (1UL<<25)  /* IS-interface RX busy */

#define DQL_LCR_LIOE     (1UL<<31)  /* Layer I/O enable - controls iso_cmd line */
#define DQL_LCR_LRE      (1UL<<30)  /* Layer reset enable */

#define DQ_L2_DNRP_STS       0x2000  /* R ADC/Fan status */
#define DQ_L2_DNRP_CFG       0x2000  /* W ADC/Fan configuration */
#define DQ_L2_DNRP_LED       0x2004  /* R/W LED */

/* DS1390 registers */
#define DS1390_WRITE        0x8000    /* write modifier */
#define DS1390_SUBSECONDS   0x0000     /* <0.1s><0.01s> [aaaabbbb] BCD */
#define DS1390_SECONDS      0x0100     /* <10s><1s>     [0aaabbbb] BCD */
#define DS1390_MINUTES      0x0200     /* <10m><1m>     [0aaabbbb] BCD */
#define DS1390_HOURS        0x0300     /* <12/24><AM/PM><10h><1h> [0abchhhh] BCD */
#define DS1390_DAY          0x0400     /* <1-7> [00000ddd] BCD */
#define DS1390_DATE         0x0500     /* <10d><1d>    [00ddeeee] BCD */
#define DS1390_MONTH        0x0600     /* <c><10m><1m> [c00tmmmm] BCD */
#define DS1390_YEAR         0x0700     /* <10y><1y>    [yyyyzzzz] BCD */
#define DS1390_CONTROL      0x0d00     /* control register */
#define DS1390_STATUS       0x0e00     /* control register */
#define DS1390_TRICKLE      0x0f00     /* trickle charger control */

#define XIN(addr) in_be32((unsigned int*)(addr))
#define XOUT(addr,val) out_be32((unsigned int*)(addr), val)

static
int uei_spiread(volatile unsigned char *addr, uint16_t reg, unsigned char *rval)
{
    uint32_t ret;
    unsigned int timo = 0x1ffff;

    XOUT(addr+DQL_CLI_ISTR, reg);

    while(timo--) {
        ret = XIN(addr + DQL_CLI_LSR);
        if(ret & DQL_LSR_IDR) {
            *rval = XIN(addr + DQL_CLI_ISDR) & 0xff;
            return 0;
        }
    }
    ret = XIN(addr + DQL_CLI_ISDR);
    return 1;
}

static
int uei_spiwrite(volatile unsigned char *addr, uint16_t reg, uint8_t val)
{
    unsigned int timo = 0x1ffff;

    XOUT(addr+DQL_CLI_ISTR, reg&DS1390_WRITE&val);

    while(timo--) {
        if(!(XIN(addr+DQL_CLI_LSR) & DQL_LSR_IBT))
            return 0;
    }
    return 1;
}

#define BIN2BCD8(V) ( (((V)/10)<<4) + ((V)%10) )

#define BCD2BIN8(V) ( ((V)>>4)*10 + ((V)&0xf) )

static
void ds1390_init(int minor)
{
    volatile unsigned char *rtcaddr = (unsigned char *)RTC_Table[minor].ulCtrlPort1;

    /* RTC chip located on power layer (aka. board).  Must be enabled */
    XOUT(rtcaddr + DQL_CLI_LCR, 0); /* force disable? */
    XOUT(rtcaddr + DQL_CLI_LCR, DQL_LCR_LIOE);

    /* setup battery trickle charger.
     * 2K ohm resistor and no diode
     */
    if(uei_spiwrite(rtcaddr, DS1390_TRICKLE, 0xA6))
        printk("Failed to setup RTC trickle charging!\n");
}

static
int ds1390_gettime(int minor, rtems_time_of_day *time)
{
    uint8_t scratch;
    rtems_time_of_day temp;
    volatile unsigned char *rtcaddr = (unsigned char *)RTC_Table[minor].ulCtrlPort1;

    if(uei_spiread(rtcaddr, DS1390_MONTH, &scratch))
        return 1;

    temp.year = scratch&0x80 ? 2000 : 1900;
    temp.month = BCD2BIN8(scratch&0x1f);

    if(uei_spiread(rtcaddr, DS1390_YEAR, &scratch))
        return 1;
    temp.year += BCD2BIN8(scratch);

    if(uei_spiread(rtcaddr, DS1390_DATE, &scratch))
        return 1;
    temp.day = BCD2BIN8(scratch) & 0x3f;

    if(uei_spiread(rtcaddr, DS1390_HOURS, &scratch))
        return 1;
    temp.hour = BCD2BIN8(scratch) & 0x3f;

    if(uei_spiread(rtcaddr, DS1390_MINUTES, &scratch))
        return 1;
    temp.minute = BCD2BIN8(scratch) & 0x7f;

    if(uei_spiread(rtcaddr, DS1390_SECONDS, &scratch))
        return 1;
    temp.second = BCD2BIN8(scratch) & 0x7f;

    *time = temp;
    return 0;
}

static
int ds1390_settime(int minor, rtems_time_of_day *time)
{
    int ret=0;
    uint8_t scratch;
    volatile unsigned char *rtcaddr = (unsigned char *)RTC_Table[minor].ulCtrlPort1;

    ret |= uei_spiwrite(rtcaddr, DS1390_YEAR, BIN2BCD8(time->year % 100));

    scratch = BIN2BCD8(time->month);
    scratch |= (time->year >= 2000) ? 0x80:0;

    ret |= uei_spiwrite(rtcaddr, DS1390_MONTH, scratch);

    ret |= uei_spiwrite(rtcaddr, DS1390_DATE, BIN2BCD8(time->day));
    ret |= uei_spiwrite(rtcaddr, DS1390_HOURS, BIN2BCD8(time->hour));
    ret |= uei_spiwrite(rtcaddr, DS1390_MINUTES, BIN2BCD8(time->minute));
    ret |= uei_spiwrite(rtcaddr, DS1390_SECONDS, BIN2BCD8(time->second));

    return ret;
}

static rtc_fns ds1390_fns =
{&ds1390_init, &ds1390_gettime, &ds1390_settime};
#endif
