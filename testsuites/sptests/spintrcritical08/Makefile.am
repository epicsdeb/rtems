##
## $Id: Makefile.am,v 1.1 2009/07/21 15:16:57 joel Exp $
##

MANAGERS = all

rtems_tests_PROGRAMS = spintrcritical08
spintrcritical08_SOURCES = init.c \
    ../spintrcritical_support/intrcritical.c

dist_rtems_tests_DATA = spintrcritical08.scn
dist_rtems_tests_DATA += spintrcritical08.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

spintrcritical08_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include
AM_CPPFLAGS += -I$(top_srcdir)/spintrcritical_support

LINK_OBJS = $(spintrcritical08_OBJECTS) $(spintrcritical08_LDADD)
LINK_LIBS = $(spintrcritical08_LDLIBS)

spintrcritical08$(EXEEXT): $(spintrcritical08_OBJECTS) $(spintrcritical08_DEPENDENCIES)
	@rm -f spintrcritical08$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
