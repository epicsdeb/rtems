## $Id: Makefile.am,v 1.6 2009/12/14 18:28:02 joel Exp $

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../../../testsuites/automake/compile.am

include $(top_srcdir)/ada.am

AM_ADAFLAGS += -I$(srcdir)/..

noinst_PROGRAMS = mp08_ada_mp08_node1
mp08_ada_mp08_node1_SOURCES = mp08_node1.adb ../mptest.adb config.h
mp08_ada_mp08_node1_SOURCES += ../mptest.ads
mp08_ada_mp08_node1_SOURCES += ../../../support/init.c

mp08_ada_mp08_node1$(EXEEXT): mp08_node1.adb ../mptest.adb init.$(OBJEXT)
	$(GNATCOMPILE) -margs -a $< -o $@

scndir = $(rtems_ada_testsdir)
dist_scn_DATA = ada_mp08-node1.scn

include $(top_srcdir)/../../../testsuites/automake/local.am
