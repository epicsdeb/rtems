##
## $Id: Makefile.am,v 1.1 2009/06/08 21:05:20 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = sp53
sp53_SOURCES = ../sp52/init.c

dist_rtems_tests_DATA = sp53.scn
dist_rtems_tests_DATA += sp53.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

sp53_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include
AM_CPPFLAGS += -DUSE_TIMER_SERVER

LINK_OBJS = $(sp53_OBJECTS) $(sp53_LDADD)
LINK_LIBS = $(sp53_LDLIBS)

sp53$(EXEEXT): $(sp53_OBJECTS) $(sp53_DEPENDENCIES)
	@rm -f sp53$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
