##
## $Id: Makefile.am,v 1.23 2009/04/03 12:45:16 ralf Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = itronhello
itronhello_SOURCES = init.c system.h

dist_rtems_tests_DATA = itronhello.scn
dist_rtems_tests_DATA += itronhello.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

itronhello_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(itronhello_OBJECTS) $(itronhello_LDADD)
LINK_LIBS = $(itronhello_LDLIBS)

itronhello$(EXEEXT): $(itronhello_OBJECTS) $(itronhello_DEPENDENCIES)
	@rm -f itronhello$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
