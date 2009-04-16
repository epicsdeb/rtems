/*
 *  Rate Monotonic Manager -- Report Statistics for All Periods
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: ratemonreportstatistics.c,v 1.6 2007/12/14 17:37:12 joel Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>

#include <rtems/bspIo.h>

#if defined(RTEMS_ENABLE_NANOSECOND_RATE_MONOTONIC_STATISTICS) || \
    defined(RTEMS_ENABLE_NANOSECOND_CPU_USAGE_STATISTICS)
  #include <rtems/score/timespec.h>

  /* We print to 1/10's of milliseconds */
  #define NANOSECONDS_DIVIDER 1000
  #define PERCENT_FMT     "%04" PRId32
  #define NANOSECONDS_FMT "%06" PRId32
#endif

/*
 *  This directive allows a thread to print the statistics information
 *  on ALL period instances which have non-zero counts using printk.
 *
 *  The implementation of this directive straddles the fence between
 *  inside and outside of RTEMS.  It is presented as part of the Manager
 *  but actually uses other services of the Manager.
 */
void rtems_rate_monotonic_report_statistics_with_plugin(
  void                  *context,
  rtems_printk_plugin_t  print
)
{
  rtems_status_code                      status;
  rtems_id                               id;
  rtems_rate_monotonic_period_statistics the_stats;
  rtems_rate_monotonic_period_status     the_status;
  char                                   name[5];

  if ( !print )
    return;

  (*print)( context, "Period information by period\n" );
#if defined(RTEMS_ENABLE_NANOSECOND_CPU_USAGE_STATISTICS)
  (*print)( context, "--- CPU times are in seconds ---\n" );
#endif
#if defined(RTEMS_ENABLE_NANOSECOND_RATE_MONOTONIC_STATISTICS)
  (*print)( context, "--- Wall times are in seconds ---\n" );
#endif
/*
Layout by columns -- in memory of Hollerith :)

1234567890123456789012345678901234567890123456789012345678901234567890123456789\
   ID     OWNER COUNT MISSED X
ididididid NNNN ccccc mmmmmm X

  Uncomment the following if you are tinkering with the formatting.
  Be sure to test the various cases.
  (*print)( context,"\
1234567890123456789012345678901234567890123456789012345678901234567890123456789\
\n");
*/
  (*print)( context, "   ID     OWNER COUNT MISSED     "
       #ifdef RTEMS_ENABLE_NANOSECOND_CPU_USAGE_STATISTICS
          "     "
       #endif
          "CPU TIME     "
       #ifdef RTEMS_ENABLE_NANOSECOND_CPU_USAGE_STATISTICS
          "    "
       #endif
       #ifdef RTEMS_ENABLE_NANOSECOND_RATE_MONOTONIC_STATISTICS
          "      "
       #endif
          "   WALL TIME\n"
  );
  (*print)( context, "                               "
       #ifdef RTEMS_ENABLE_NANOSECOND_CPU_USAGE_STATISTICS
          "     "
       #endif
          "MIN/MAX/AVG    "
       #ifdef RTEMS_ENABLE_NANOSECOND_CPU_USAGE_STATISTICS
          "    "
       #endif
       #ifdef RTEMS_ENABLE_NANOSECOND_RATE_MONOTONIC_STATISTICS
          "      "
       #endif
          "  MIN/MAX/AVG\n"
  );

  /*
   * Cycle through all possible ids and try to report on each one.  If it
   * is a period that is inactive, we just get an error back.  No big deal.
   */
  for ( id=_Rate_monotonic_Information.minimum_id ;
        id <= _Rate_monotonic_Information.maximum_id ;
        id++ ) {
    status = rtems_rate_monotonic_get_statistics( id, &the_stats );
    if ( status != RTEMS_SUCCESSFUL )
      continue;
    
    /* If the above passed, so should this but check it anyway */
    status = rtems_rate_monotonic_get_status( id, &the_status );
    #if defined(RTEMS_DEBUG)
      if ( status != RTEMS_SUCCESSFUL )
        continue;
    #endif

    name[ 0 ] = '\0';

    if ( the_status.owner ) {
      rtems_object_get_name( the_status.owner, sizeof(name), name );
    }

    /*
     *  Print part of report line that is not dependent on granularity 
     */

    (*print)( context,
      "0x%08" PRIx32 " %4s %5" PRId32 " %6" PRId32 " ",
      id, name,
      the_stats.count, the_stats.missed_count
    );

    /*
     *  If the count is zero, don't print statistics
     */

    if (the_stats.count == 0) {
      (*print)( context, "\n" );
      continue;
    }

    /*
     *  print CPU Usage part of statistics
     */
    {
    #ifdef RTEMS_ENABLE_NANOSECOND_CPU_USAGE_STATISTICS
      struct timespec   cpu_average;

      _Timespec_Divide_by_integer(
         &the_stats.total_cpu_time,
         the_stats.count,
         &cpu_average
      );
      (*print)( context,
        "%" PRId32 "."  NANOSECONDS_FMT "/"        /* min cpu time */
        "%" PRId32 "."  NANOSECONDS_FMT "/"        /* max cpu time */
        "%" PRId32 "."  NANOSECONDS_FMT " ",       /* avg cpu time */
        the_stats.min_cpu_time.tv_sec, 
          the_stats.min_cpu_time.tv_nsec / NANOSECONDS_DIVIDER,
        the_stats.max_cpu_time.tv_sec,
          the_stats.max_cpu_time.tv_nsec / NANOSECONDS_DIVIDER,
        cpu_average.tv_sec,
          cpu_average.tv_nsec / NANOSECONDS_DIVIDER
       );
    #else
      uint32_t ival_cpu, fval_cpu;

      ival_cpu = the_stats.total_cpu_time * 100 / the_stats.count;
      fval_cpu = ival_cpu % 100;
      ival_cpu /= 100;

      (*print)( context,
        "%3" PRId32 "/%4" PRId32 "/%3" PRId32 ".%02" PRId32 " ",
        the_stats.min_cpu_time, the_stats.max_cpu_time, ival_cpu, fval_cpu
      );
    #endif
    }

    /*
     *  print Wall time part of statistics
     */
    {
    #ifdef RTEMS_ENABLE_NANOSECOND_RATE_MONOTONIC_STATISTICS
      struct timespec  wall_average;
      _Timespec_Divide_by_integer(
         &the_stats.total_wall_time,
         the_stats.count,
         &wall_average
      );
      (*print)( context,
        "%" PRId32 "." NANOSECONDS_FMT "/"        /* min wall time */
        "%" PRId32 "." NANOSECONDS_FMT "/"        /* max wall time */
        "%" PRId32 "." NANOSECONDS_FMT "\n",      /* avg wall time */
        the_stats.min_wall_time.tv_sec, 
          the_stats.min_wall_time.tv_nsec / NANOSECONDS_DIVIDER,
        the_stats.max_wall_time.tv_sec,
          the_stats.max_wall_time.tv_nsec / NANOSECONDS_DIVIDER,
        wall_average.tv_sec,
          wall_average.tv_nsec / NANOSECONDS_DIVIDER
      );
    #else
      uint32_t  ival_wall, fval_wall;

      ival_wall = the_stats.total_wall_time * 100 / the_stats.count;
      fval_wall = ival_wall % 100;
      ival_wall /= 100;
      (*print)( context,
        "%3" PRId32 "/%4" PRId32 "/%3" PRId32 ".%02" PRId32 "\n",
        the_stats.min_wall_time, the_stats.max_wall_time, ival_wall, fval_wall
      );
    #endif
    }
  }
}

void rtems_rate_monotonic_report_statistics( void )
{
  rtems_rate_monotonic_report_statistics_with_plugin( NULL, printk_plugin );
}
