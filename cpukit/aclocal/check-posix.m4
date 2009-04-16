dnl $Id: check-posix.m4,v 1.4 2003/11/27 14:36:23 ralf Exp $
dnl
AC_DEFUN([RTEMS_CHECK_POSIX_API],
[dnl
AC_REQUIRE([RTEMS_CANONICAL_TARGET_CPU])dnl
AC_REQUIRE([RTEMS_ENABLE_POSIX])dnl

AC_CACHE_CHECK([whether CPU supports libposix],
  rtems_cv_HAS_POSIX_API,
  [dnl
    case "$RTEMS_CPU" in
    unix*)
      rtems_cv_HAS_POSIX_API="no"
      ;;
    *)
      if test "${RTEMS_HAS_POSIX_API}" = "yes"; then
        rtems_cv_HAS_POSIX_API="yes";
      else
        rtems_cv_HAS_POSIX_API="disabled";
      fi
      ;;
    esac])
])
