##
## $Id: Makefile.am,v 1.40 2010/03/04 06:36:50 ccj Exp $
##

include $(top_srcdir)/automake/multilib.am
include $(top_srcdir)/automake/compile.am

# Pick up nfsclient
if LIBNETWORKING
NFSCLIENT_SUBDIR= src/nfsclient
endif
SUBDIRS = $(NFSCLIENT_SUBDIR)
DIST_SUBDIRS = $(SUBDIRS)

EXTRA_DIST = README

noinst_LIBRARIES = libimfs.a

libimfs_a_SOURCES =

libimfs_a_SOURCES += src/imfs/imfs_chown.c src/imfs/imfs_config.c \
    src/imfs/imfs_creat.c src/imfs/imfs_directory.c src/imfs/imfs_eval.c \
    src/imfs/imfs_free.c src/imfs/imfs_fsunmount.c src/imfs/imfs_gtkn.c \
    src/imfs/imfs_init.c src/imfs/imfs_initsupp.c src/imfs/imfs_link.c \
    src/imfs/imfs_mknod.c src/imfs/imfs_mount.c src/imfs/imfs_fchmod.c \
    src/imfs/imfs_unlink.c src/imfs/imfs_unmount.c src/imfs/imfs_utime.c \
    src/imfs/imfs_ntype.c src/imfs/imfs_stat.c src/imfs/imfs_getchild.c \
    src/imfs/memfile.c src/imfs/imfs_fifo.c src/imfs/deviceio.c \
    src/imfs/imfs_handlers_device.c src/imfs/imfs_handlers_directory.c \
    src/imfs/imfs_handlers_link.c src/imfs/imfs_handlers_memfile.c \
    src/imfs/imfs_debug.c src/imfs/imfs_rmnod.c src/imfs/imfs_symlink.c \
    src/imfs/imfs_readlink.c src/imfs/imfs_fdatasync.c src/imfs/imfs_fcntl.c \
    src/imfs/ioman.c src/imfs/miniimfs_init.c src/imfs/imfs_load_tar.c \
    src/imfs/imfs_rename.c src/imfs/imfs.h \
    src/imfs/deviceerrno.c \
    src/devfs/devfs_init.c src/devfs/devfs_eval.c src/devfs/devfs_mknod.c \
    src/devfs/devfs_show.c src/devfs/devfs_node_type.c \
    src/devfs/devopen.c src/devfs/devread.c src/devfs/devwrite.c \
    src/devfs/devclose.c src/devfs/devioctl.c src/devfs/devstat.c \
    src/devfs/devfs.h

# POSIX FIFO/pipe
libimfs_a_SOURCES += src/pipe/fifo.c src/pipe/pipe.c src/pipe/pipe.h

# dosfs
if LIBDOSFS
noinst_LIBRARIES += libdosfs.a

# libdosfs_FATFS_C_FILES
libdosfs_a_SOURCES = src/dosfs/fat.c src/dosfs/fat.h \
    src/dosfs/fat_fat_operations.c src/dosfs/fat_fat_operations.h \
    src/dosfs/fat_file.c src/dosfs/fat_file.h

# libdosfs_DOSFS_C_FILES
libdosfs_a_SOURCES += src/dosfs/msdos_create.c src/dosfs/msdos_dir.c \
    src/dosfs/msdos_eval.c src/dosfs/msdos_file.c src/dosfs/msdos_free.c \
    src/dosfs/msdos_fsunmount.c src/dosfs/msdos_handlers_dir.c \
    src/dosfs/msdos_handlers_file.c src/dosfs/msdos_init.c \
    src/dosfs/msdos_initsupp.c src/dosfs/msdos_misc.c \
    src/dosfs/msdos_mknod.c src/dosfs/msdos_node_type.c \
    src/dosfs/msdos_conv.c src/dosfs/msdos.h src/dosfs/msdos_format.c \
    src/dosfs/dosfs.h src/dosfs/msdos_rename.c
endif

# RFS
noinst_LIBRARIES += librfs.a
librfs_a_SOURCES = \
    src/rfs/rtems-rfs-bitmaps.c src/rfs/rtems-rfs-block.c \
    src/rfs/rtems-rfs-buffer-bdbuf.c src/rfs/rtems-rfs-buffer.c \
    src/rfs/rtems-rfs-dir-hash.c src/rfs/rtems-rfs-file.c \
    src/rfs/rtems-rfs-group.c src/rfs/rtems-rfs-inode.c \
    src/rfs/rtems-rfs-rtems-dev.c src/rfs/rtems-rfs-rtems-utils.c \
    src/rfs/rtems-rfs-rtems.c src/rfs/rtems-rfs-shell.c \
    src/rfs/rtems-rfs-bitmaps-ut.c src/rfs/rtems-rfs-dir.c \
    src/rfs/rtems-rfs-file-system.c src/rfs/rtems-rfs-format.c \
    src/rfs/rtems-rfs-link.c src/rfs/rtems-rfs-mutex.c \
    src/rfs/rtems-rfs-rtems-dir.c src/rfs/rtems-rfs-rtems-file.c \
    src/rfs/rtems-rfs-trace.c

# ---
include $(srcdir)/preinstall.am
include $(top_srcdir)/automake/local.am
