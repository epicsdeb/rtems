##
## $Id: Makefile.am,v 1.2 2009/11/30 12:41:10 thomas Exp $
##

MANAGERS = io semaphore event

rtems_tests_PROGRAMS = block07
block07_SOURCES = init.c

dist_rtems_tests_DATA = block07.scn block07.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

block07_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(block07_OBJECTS) $(block07_LDADD)
LINK_LIBS = $(block07_LDLIBS)

block07$(EXEEXT): $(block07_OBJECTS) $(block07_DEPENDENCIES)
	@rm -f block07$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
