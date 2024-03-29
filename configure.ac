AC_INIT([FedStage DRMAA utilities library], [2.0.1], [drmaa-pbspro-users@lists.fedstage.com], [drmaa_utils])
AC_PREREQ([2.69])
AC_REVISION([m4_esyscmd_s([git rev-parse HEAD])])
AC_COPYRIGHT([
FedStage DRMAA utilities library
Copyright (C) 2006-2008  FedStage Systems

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
])

DRMAA_UTILS_MAJOR=2
DRMAA_UTILS_MINOR=0
DRMAA_UTILS_MICRO=1
DRMAA_UTILS_VERSION_INFO=2:1:0
AC_SUBST([DRMAA_UTILS_MAJOR])
AC_SUBST([DRMAA_UTILS_MINOR])
AC_SUBST([DRMAA_UTILS_MICRO])
AC_SUBST([DRMAA_UTILS_VERSION_INFO])

AC_CONFIG_SRCDIR([drmaa_utils/xmalloc.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([scripts])

AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
DEVELOPER_MODE="no"
AM_CONDITIONAL([DEVELOPER_MODE], [test "x$DEVELOPER_MODE" = "xyes"])

# command-line arguments:
AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug],[produce code suiteable for debugging and print logs at runtime]))
AC_ARG_ENABLE(development, AS_HELP_STRING([--enable-development],[enable development mode: make additional checks (suiteable for FedStage DRMAA utilities library developers)]))

AC_ARG_WITH(drmaa-utils, AS_HELP_STRING([--with-drmaa-utils=...],[used only to detect that drmaa utils is sub packaged]))
AM_CONDITIONAL([DRMAA_UTILS_STANDALONE],
		[test x$with_drmaa_utils = x])

# programs:
AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_CPP
AC_PROG_INSTALL
LT_INIT
AC_PROG_MAKE_SET
AC_PROG_LN_S

AC_ARG_VAR([SED], [Location of the sed utility])
AC_PATH_PROG(SED, [sed])
AC_SUBST([SED])

AC_ARG_VAR([TR], [Location of the tr utility])
AC_PATH_PROG([TR], [tr])


# code generation tools:
AC_PROG_YACC
AX_GPERF
AC_CHECK_PROGS([RAGEL], [ragel], [sh ../m4/missing-dev-prog.sh ragel])

# reStructuredText processing:
AX_DOCUTILS
# documentation generation:
AC_CHECK_PROGS([DOT], [dot])
if test x$DOT != x; then
	HAVE_DOT=yes
else
	HAVE_DOT=no
fi
AC_SUBST([HAVE_DOT])

# check compiler / set basic flags:
if test x$ac_cv_prog_cc_c99 = xno; then
	AC_MSG_ERROR([C99 compiler is required])
fi

if test x$GCC = xyes; then
	CFLAGS="-pedantic -std=c99 ${CFLAGS}"
fi
AM_CONDITIONAL([GCC], [test x$GCC = xyes])

AX_GCC_WARNINGS()

AH_TEMPLATE([DEBUGGING], [Produce debugging code])
if test x$enable_debug = xyes; then
	AC_DEFINE(DEBUGGING,[1])
	CFLAGS="${CFLAGS} -O0"
else
	CPPFLAGS="-DNDEBUG ${CPPFLAGS}"
fi

AH_TEMPLATE([DEVELOPMENT], [Development mode])
if test x$enable_development = xyes; then
	AC_DEFINE(DEVELOPMENT, [1])
fi

AH_BOTTOM([
#ifndef __GNUC__
#	define __attribute__ /* nothing */
#endif
])

# system:
CPPFLAGS="-D_REENTRANT -D_THREAD_SAFE ${CPPFLAGS}"

AC_CANONICAL_HOST
case "$host_os" in
	*linux*)
		CPPFLAGS="${CPPFLAGS} -D_GNU_SOURCE"
		;;
	*solaris*)
		CPPFLAGS="${CPPFLAGS} -D_XOPEN_SOURCE=500 -D__EXTENSIONS__"
		;;
	*freebsd*)
		;;
	*darwin*)
		CPPFLAGS="${CPPFLAGS} -D_XOPEN_SOURCE=400"
		;;
	*aix*)                                                                                                                                                                                                                                  
		CPPFLAGS="${CPPFLAGS} -D_XOPEN_SOURCE=500 -D_XOPEN_SOURCE_EXTENDED=1 -D__linux__"  
		;;
	*)
		CPPFLAGS="${CPPFLAGS} -D_XOPEN_SOURCE=500 -D__USE_BSD"
		;;
esac


AC_C_BIGENDIAN([BYTEORDER=4321],[BYTEORDER=1234])
AC_DEFINE_UNQUOTED(BYTEORDER, $BYTEORDER, [1234 = LILENDIAN, 4321 = BIGENDIAN])

# project prerequisites:

# libraries:
AX_PTHREAD([CFLAGS="$CFLAGS $PTHREAD_CFLAGS" LIBS="$PTHREAD_LIBS $LIBS"],
		[AC_MSG_ERROR([POSIX threads library is required by DRMAA.])])

# headers:
AC_CHECK_HEADERS([execinfo.h fcntl.h inttypes.h libintl.h limits.h malloc.h stddef.h stdint.h stdlib.h string.h sys/time.h unistd.h])
AC_HEADER_STDBOOL
AC_HEADER_TIME

# types and structures:
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_MODE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_STRUCT_TM
AC_CHECK_MEMBER(struct tm.tm_gmtoff,[AC_DEFINE([HAVE_STRUCT_TM_GMTOFF],[1],[gmtoff in struct tm])],,[#include <time.h>])

# compiler characteristic
AC_C_CONST
AC_C_INLINE
AC_C_VOLATILE

# functions:
AC_FUNC_ALLOCA
AC_FUNC_STRERROR_R
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([ \
	asprintf \
	fstat \
	getcwd \
	gettimeofday \
	localtime_r \
	memset \
	strchr \
	strdup \
	strerror \
	strlcpy \
	strndup \
	strstr \
	vasprintf])
AX_FUNC_GETTID
AX_FUNC_VA_COPY
AC_SEARCH_LIBS([backtrace], [execinfo])

# system services:

# turn on warning after all tests
if test x$GCC = xyes; then
	if test x$enable_development = xyes; then
		CFLAGS="-Werror ${CFLAGS}"
	fi
	CFLAGS="-Wall -W -Wno-unused  $GCC_W_NO_FORMAT_ZERO_LENGTH  $GCC_W_NO_MISSING_FIELD_INITIALIZERS ${CFLAGS}"
fi

# put all usefull paths in config.h
AH_TEMPLATE([DRMAA_DIR_PREFIX], [The installation prefix])
AH_TEMPLATE([DRMAA_DIR_BIN], [Location of binary files])
AH_TEMPLATE([DRMAA_DIR_SYSCONF], [Location of system configuration files])
AH_TEMPLATE([DRMAA_DIR_LOCALSTATE], [Location of variable data files])
AH_TEMPLATE([DRMAA_DIR_DATA], [Location of package data files])

test "$prefix" = NONE && prefix="$ac_default_prefix"
test "$exec_prefix" = NONE && exec_prefix='${prefix}'

for i in bindir prefix localstatedir  sysconfdir datadir; do
        name=`echo $i | $SED -e 's/dir$//' | $TR 'a-z' 'A-Z'`
        x="\$$i"
        eval y="$x"
        while test "$x" != "$y"; do
                x="$y"
                eval y="$x"
        done
        AC_DEFINE_UNQUOTED([DRMAA_DIR_$name], ["$x"])
done


AC_CONFIG_FILES([
	Makefile
	drmaa_utils/Makefile
	test/Makefile
	Doxyfile
])
AC_CONFIG_HEADERS([config.h])
AC_OUTPUT

echo
echo "Run 'make' now."

