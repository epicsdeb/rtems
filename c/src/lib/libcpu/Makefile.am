##
## $Id: Makefile.am,v 1.11 2003/12/01 12:18:48 ralf Exp $
##

noinst_HEADERS = shared/include/cache.h

noinst_HEADERS += shared/src/cache_aligned_malloc.c \
    shared/src/cache_manager.c

SUBDIRS = @libcpu_cpu_subdir@
DIST_SUBDIRS = @libcpu_cpu_subdir@

include $(top_srcdir)/automake/subdirs.am
include $(top_srcdir)/automake/local.am
