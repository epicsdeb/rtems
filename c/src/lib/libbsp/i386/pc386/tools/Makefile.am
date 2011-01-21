##
## $Id: Makefile.am,v 1.23 2007/01/26 08:25:20 ralf Exp $
##

ACLOCAL_AMFLAGS = -I ../../../../../aclocal

EXTRA_DIST = Spec.doc

# HACK: autoconf wants to transform the name, but RTEMS *.cfgs are not able
# to deal with it.
# FIXME: The installation directory is arguable
transform =
bsptools_bindir = ${prefix}/bin
bsptools_bin_PROGRAMS = $(target_alias)-bin2boot

$(target_alias)-bin2boot_SOURCES = bin2boot.c

## HACK: install into build-tree
all-local: $(TMPINSTALL_FILES)

TMPINSTALL_FILES =

$(PROJECT_ROOT)/@RTEMS_BSP@/bin/$(dirstamp):
	@$(MKDIR_P) $(PROJECT_ROOT)/@RTEMS_BSP@/bin
	@: > $(PROJECT_ROOT)/@RTEMS_BSP@/bin/$(dirstamp)
TMPINSTALL_FILES += $(PROJECT_ROOT)/@RTEMS_BSP@/bin/$(dirstamp)

$(PROJECT_ROOT)/@RTEMS_BSP@/bin/$(target_alias)-bin2boot$(EXEEXT): bin2boot$(EXEEXT) $(PROJECT_ROOT)/@RTEMS_BSP@/bin/$(dirstamp)
	$(INSTALL_PROGRAM) $< $(PROJECT_ROOT)/@RTEMS_BSP@/bin/$(target_alias)-bin2boot$(EXEEXT)
TMPINSTALL_FILES += $(PROJECT_ROOT)/@RTEMS_BSP@/bin/$(target_alias)-bin2boot$(EXEEXT)

CLEANFILES = $(TMPINSTALL_FILES)

include $(top_srcdir)/../../../../../automake/host.am
