##
## $Id: Makefile.am,v 1.1 2009/08/05 19:36:57 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = bspcmdline01
bspcmdline01_SOURCES = init.c

dist_rtems_tests_DATA = bspcmdline01.scn
dist_rtems_tests_DATA += bspcmdline01.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

bspcmdline01_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(bspcmdline01_OBJECTS) $(bspcmdline01_LDADD)
LINK_LIBS = $(bspcmdline01_LDLIBS)

bspcmdline01$(EXEEXT): $(bspcmdline01_OBJECTS) $(bspcmdline01_DEPENDENCIES)
	@rm -f bspcmdline01$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
