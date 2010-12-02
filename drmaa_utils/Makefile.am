# $Id: Makefile.am 1457 2008-10-03 10:17:28Z lukasz $
#
# FedStage DRMAA utilities library
# Copyright (C) 2006-2008  FedStage Systems
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

GPERF = @GPERF@
GPERFFLAGS = --readonly-tables

YLWRAP = $(top_srcdir)/m4/bison_ylwrap.sh
AM_YFLAGS = -d

RAGEL = @RAGEL@
RAGELFLAGS =

pkgincludedir = $(includedir)/drmaa_utils

# if DRMAA_UTILS_STANDALONE
lib_LTLIBRARIES = libdrmaa_utils.la libdrmaa_base.la
#else !DRMAA_UTILS_STANDALONE
#noinst_LTLIBRARIES = libdrmaa_utils.la libdrmaa_base.la
#endif

libdrmaa_utils_la_SOURCES = \
 compat.c compat.h \
 common.h \
 conf.c conf.h \
 conf_impl.h conf_tab.y \
 datetime.c datetime.h \
 datetime_impl.h datetime_tab.y \
 drmaa_attrib.gperf drmaa_attrib.h \
 environ.c environ.h \
 exception.c exception.h \
 iter.c iter.h \
 job.c job.h \
 logging.c logging.h \
 lookup3.c lookup3.h \
 session.c session.h \
 template.c template.h \
 timedelta.rl \
 thread.c thread.h \
 util.c util.h \
 drmaa_util.c drmaa_util.h \
 xmalloc.c xmalloc.h
libdrmaa_utils_la_LDFLAGS = -version-info 2
if GCC
# allow `%m` in printf-like format string
exception.lo: CFLAGS += -Wno-format
endif

libdrmaa_base_la_SOURCES = \
 drmaa_base.c drmaa_base.h
libdrmaa_base_la_LDFLAGS = -version-info 2
libdrmaa_base_la_LIBADD = libdrmaa_utils.la

include_HEADERS = drmaa.h
# if DRMAA_UTILS_STANDALONE
pkginclude_HEADERS = \
 common.h \
 compat.h \
 conf.h \
 datetime.h \
 drmaa_attrib.h \
 drmaa_base.h \
 drmaa_util.h \
 drmaa.h \
 environ.h \
 exception.h \
 iter.h \
 job.h \
 lookup3.h \
 logging.h \
 session.h \
 template.h \
 thread.h \
 util.h \
 xmalloc.h
# endif

BUILT_SOURCES = \
 datetime_tab.c datetime_tab.h \
 conf_tab.c conf_tab.h
EXTRA_DIST = drmaa_attrib.c timedelta.c

.gperf.c:
	$(GPERF) $(GPERFFLAGS) --output-file=$@ $<
.rl.c:
	$(RAGEL) $(RAGELFLAGS) -o $@ $<

if GCC
drmaa_attrib.lo: CFLAGS += -Wno-missing-field-initializers
# Bison and Ragel generated code have many unused symbols
datetime_tab.lo conf_tab.lo: CFLAGS += -Wno-unused
timedelta.lo: CFLAGS += -Wno-unused
endif

