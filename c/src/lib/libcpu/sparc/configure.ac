## Process this file with autoconf to produce a configure script.
##
## $Id: configure.ac,v 1.25.2.1 2011/02/02 15:17:37 ralf Exp $

AC_PREREQ([2.68])
AC_INIT([rtems-c-src-lib-libcpu-sparc],[_RTEMS_VERSION],[http://www.rtems.org/bugzilla])
AC_CONFIG_SRCDIR([reg_win])
RTEMS_TOP([../../../../..],[../../..])

RTEMS_CANONICAL_TARGET_CPU

AM_INIT_AUTOMAKE([no-define foreign subdir-objects 1.11.1])
AM_MAINTAINER_MODE

RTEMS_ENV_RTEMSBSP

RTEMS_PROJECT_ROOT

RTEMS_PROG_CC_FOR_TARGET
AM_PROG_CC_C_O
RTEMS_CANONICALIZE_TOOLS
RTEMS_PROG_CCAS

AM_CONDITIONAL(has_instruction_cache, test "$RTEMS_CPU_MODEL" = "leon1" \
|| test "$RTEMS_CPU_MODEL" = "leon2" || test "$RTEMS_CPU_MODEL" = "leon3" )

RTEMS_AMPOLISH3

# Explicitly list all Makefiles here
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

