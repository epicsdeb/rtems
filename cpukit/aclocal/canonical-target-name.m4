dnl
dnl $Id: canonical-target-name.m4,v 1.6.6.1 2008/09/10 15:43:54 joel Exp $
dnl

dnl canonicalize target cpu
dnl NOTE: Most rtems targets do not fullfil autoconf's
dnl target naming conventions "processor-vendor-os"
dnl Therefore autoconf's AC_CANONICAL_TARGET will fail for them
dnl and we have to fix it for rtems ourselves 

AC_DEFUN([RTEMS_CANONICAL_TARGET_CPU],
[AC_REQUIRE([AC_CANONICAL_HOST])
AC_MSG_CHECKING(rtems target cpu)
case "${host}" in
  # hpux unix port should go here
  i[[34567]]86-*linux*)		# unix "simulator" port
	RTEMS_CPU=unix
	;;
  i[[34567]]86-*freebsd*) 	# unix "simulator" port
	RTEMS_CPU=unix
	;;
  i[[34567]]86-pc-cygwin*) 	# Cygwin is just enough unix like :)
	RTEMS_CPU=unix
	;;
  no_cpu-*rtems*)
        RTEMS_CPU=no_cpu
	;;
  sparc-sun-solaris*)           # unix "simulator" port
	RTEMS_CPU=unix
	;;
  *) 
	RTEMS_CPU=`echo $host | sed 's%^\([[^-]]*\)-\(.*\)$%\1%'`
	;;
esac
AC_SUBST(RTEMS_CPU)
AC_MSG_RESULT($RTEMS_CPU)
])
