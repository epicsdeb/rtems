##
## $Id: Makefile.am,v 1.4 2009/07/24 20:43:06 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = sp41
sp41_SOURCES = init.c

dist_rtems_tests_DATA = sp41.scn
dist_rtems_tests_DATA += sp41.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

sp41_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(sp41_OBJECTS) $(sp41_LDADD)
LINK_LIBS = $(sp41_LDLIBS)

sp41$(EXEEXT): $(sp41_OBJECTS) $(sp41_DEPENDENCIES)
	@rm -f sp41$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
