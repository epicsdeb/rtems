##
## $Id: Makefile.am,v 1.23 2009/04/03 12:48:13 ralf Exp $
##

MANAGERS = io dual_ported_memory semaphore clock

rtems_tests_PROGRAMS = sp23
sp23_SOURCES = init.c task1.c system.h

dist_rtems_tests_DATA = sp23.scn
dist_rtems_tests_DATA += sp23.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

sp23_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(sp23_OBJECTS) $(sp23_LDADD)
LINK_LIBS = $(sp23_LDLIBS)

sp23$(EXEEXT): $(sp23_OBJECTS) $(sp23_DEPENDENCIES)
	@rm -f sp23$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
