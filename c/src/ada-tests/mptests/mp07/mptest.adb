--
--  MPTEST / BODY
--
--  DESCRIPTION:
--
--  This package is the implementation for Test 7 of the RTEMS
--  Multiprocessor Test Suite.
--
--  DEPENDENCIES: 
--
--  
--
--  COPYRIGHT (c) 1989-1997.
--  On-Line Applications Research Corporation (OAR).
--
--  The license and distribution terms for this file may in
--  the file LICENSE in this distribution or at
--  http://www.rtems.com/license/LICENSE.
--
--  $Id: mptest.adb,v 1.5 2007/10/01 22:58:09 joel Exp $
--

with INTERFACES; use INTERFACES;
with RTEMS;
with TEST_SUPPORT;
with TEXT_IO;
with UNSIGNED32_IO;

package body MPTEST is

--PAGE
--
--  INIT
--

   procedure INIT (
      ARGUMENT : in     RTEMS.TASK_ARGUMENT
   ) is
      STATUS : RTEMS.STATUS_CODES;
   begin

      TEXT_IO.NEW_LINE( 2 );
      TEXT_IO.PUT( "*** TEST 7 -- NODE " );
      UNSIGNED32_IO.PUT(
         TEST_SUPPORT.NODE,
         WIDTH => 1
      );
      TEXT_IO.PUT_LINE( " ***" );

      MPTEST.TASK_NAME( 1 ) := RTEMS.BUILD_NAME(  '1', '1', '1', ' ' );
      MPTEST.TASK_NAME( 2 ) := RTEMS.BUILD_NAME(  '2', '2', '2', ' ' );

      TEXT_IO.PUT_LINE( "Creating Test_task (Global)" );
      RTEMS.TASK_CREATE( 
         MPTEST.TASK_NAME( TEST_SUPPORT.NODE ), 
         TEST_SUPPORT.NODE, 
         2048, 
         RTEMS.TIMESLICE,
         RTEMS.GLOBAL,
         MPTEST.TASK_ID( 1 ),
         STATUS
      );
      TEST_SUPPORT.DIRECTIVE_FAILED( STATUS, "TASK_CREATE" );

      TEXT_IO.PUT_LINE( "Starting Test_task (Global)" );
      RTEMS.TASK_START(
         MPTEST.TASK_ID( 1 ),
         MPTEST.TEST_TASK'ACCESS,
         0,
         STATUS
      );
      TEST_SUPPORT.DIRECTIVE_FAILED( STATUS, "TASK_START" );

      MPTEST.TIMER_NAME( 1 ) := RTEMS.BUILD_NAME(  'T', 'M', '1', ' ' );

      RTEMS.TIMER_CREATE(
         MPTEST.TIMER_NAME( 1 ),
         MPTEST.TIMER_ID( 1 ),
         STATUS
      );
      TEST_SUPPORT.DIRECTIVE_FAILED( STATUS, "TIMER_CREATE" );

      TEXT_IO.PUT_LINE( "Deleting initialization task" );
      RTEMS.TASK_DELETE( RTEMS.SELF, STATUS );
      TEST_SUPPORT.DIRECTIVE_FAILED( STATUS, "TASK_DELETE OF SELF" );

   end INIT;

--PAGE
--
--  STOP_TEST_TSR
--
 
   procedure STOP_TEST_TSR (
      IGNORED1 : in     RTEMS.ID;
      IGNORED2 : in     RTEMS.ADDRESS
   ) is
   begin
 
      MPTEST.STOP_TEST := TRUE;
 
   end STOP_TEST_TSR;
 
--PAGE
--
--  TEST_TASK
--

   procedure TEST_TASK (
      ARGUMENT : in     RTEMS.TASK_ARGUMENT
   ) is
      EVENT_OUT : RTEMS.EVENT_SET;
      STATUS    : RTEMS.STATUS_CODES;
   begin

      MPTEST.STOP_TEST := FALSE;

      if TEST_SUPPORT.NODE = 1 then
         MPTEST.REMOTE_NODE := 2;
      else
         MPTEST.REMOTE_NODE := 1;
      end if;

      TEXT_IO.PUT( "Remote task's name is : " );
      TEST_SUPPORT.PUT_NAME( MPTEST.TASK_NAME( MPTEST.REMOTE_NODE ), TRUE );

      TEXT_IO.PUT_LINE( "Getting TID of remote task" );
      loop
         RTEMS.TASK_IDENT( 
            MPTEST.TASK_NAME( MPTEST.REMOTE_NODE ),
            RTEMS.SEARCH_ALL_NODES,
            MPTEST.REMOTE_TID,
            STATUS
         );

         exit when RTEMS.ARE_STATUSES_EQUAL( STATUS, RTEMS.SUCCESSFUL ); 

      end loop;

      if TEST_SUPPORT.NODE = 1 then

         TEXT_IO.PUT_LINE( "Sending first event to remote task" );
         RTEMS.EVENT_SEND( 
            MPTEST.REMOTE_TID,
            RTEMS.EVENT_16,
            STATUS
         );
         TEST_SUPPORT.DIRECTIVE_FAILED( STATUS, "EVENT_SEND" );

      end if;

      RTEMS.TIMER_FIRE_AFTER(
         MPTEST.TIMER_ID( 1 ),
         5 * TEST_SUPPORT.TICKS_PER_SECOND,
         MPTEST.STOP_TEST_TSR'ACCESS,
         RTEMS.NULL_ADDRESS,
         STATUS
      );
      TEST_SUPPORT.DIRECTIVE_FAILED( STATUS, "TIMER_FIRE_AFTER" );

      loop

         exit when MPTEST.STOP_TEST = TRUE;

         for COUNT in 1 .. MPTEST.PER_DOT
         loop

            RTEMS.EVENT_RECEIVE( 
               RTEMS.EVENT_16,
               RTEMS.DEFAULT_OPTIONS,
               TEST_SUPPORT.TICKS_PER_SECOND,
               EVENT_OUT,
               STATUS
            );
            if RTEMS.ARE_STATUSES_EQUAL( STATUS, RTEMS.TIMEOUT ) then
               TEXT_IO.NEW_LINE;
               TEXT_IO.PUT_LINE(
                   "TA1 - TIMEOUT .. probably OK if the other node exits"
               );
               exit;
            else
               TEST_SUPPORT.DIRECTIVE_FAILED( STATUS, "EVENT_RECEIVE" );
            end if;

            RTEMS.EVENT_SEND( 
               MPTEST.REMOTE_TID,
               RTEMS.EVENT_16,
               STATUS
            );
            TEST_SUPPORT.DIRECTIVE_FAILED( STATUS, "EVENT_SEND" );

         end loop;

         TEST_SUPPORT.PUT_DOT( "." );

      end loop;

      TEXT_IO.NEW_LINE;

      TEXT_IO.PUT_LINE( "*** END OF TEST 7 ***" );

      RTEMS.SHUTDOWN_EXECUTIVE( 0 );

   end TEST_TASK;

end MPTEST;
