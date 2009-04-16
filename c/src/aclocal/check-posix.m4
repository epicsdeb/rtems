dnl $Id: check-posix.m4,v 1.3 2003/11/26 06:18:03 ralf Exp $
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
if test "$rtems_cv_HAS_POSIX_API" = "yes"; then
  HAS_POSIX_API="yes";
else
  HAS_POSIX_API="no";
fi
AC_SUBST(HAS_POSIX_API)dnl
])

AC_DEFUN([RTEMS_DEFINE_POSIX_API],
[AC_REQUIRE([RTEMS_CHECK_POSIX_API])dnl
AS_IF(
  [test x"${HAS_POSIX_API}" = x"yes"],
  [AC_DEFINE_UNQUOTED(RTEMS_POSIX_API,1,[if posix api is supported])])
])
