##
## $Id: Makefile.am,v 1.1 2009/07/10 20:28:10 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = spintrcritical05
spintrcritical05_SOURCES = ../spintrcritical01/init.c \
    ../spintrcritical_support/intrcritical.c

dist_rtems_tests_DATA = spintrcritical05.scn
dist_rtems_tests_DATA += spintrcritical05.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

spintrcritical05_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include
AM_CPPFLAGS += -I$(top_srcdir)/spintrcritical_support
AM_CPPFLAGS += -DPRIORITY_NO_TIMEOUT_REVERSE

LINK_OBJS = $(spintrcritical05_OBJECTS) $(spintrcritical05_LDADD)
LINK_LIBS = $(spintrcritical05_LDLIBS)

spintrcritical05$(EXEEXT): $(spintrcritical05_OBJECTS) $(spintrcritical05_DEPENDENCIES)
	@rm -f spintrcritical05$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
