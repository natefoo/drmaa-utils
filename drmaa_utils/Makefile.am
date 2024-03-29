# $Id$
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
RAGEL = @RAGEL@
RAGELFLAGS =

YLWRAP = $(top_srcdir)/m4/bison_ylwrap.sh
AM_YFLAGS = -d

noinst_LTLIBRARIES = libdrmaa_utils.la
COMMON_SOURCES = \
 compat.c compat.h \
 common.h \
 conf.c conf.h \
 conf_impl.h conf_tab.y \
 datetime.c datetime.h \
 datetime_impl.h datetime_tab.y \
 drmaa_attrib.c drmaa_attrib.h \
 environ.c environ.h \
 exception.c exception.h \
 iter.c iter.h \
 fsd_job.c job.h \
 logging.c logging.h \
 lookup3.c lookup3.h \
 template.c template.h \
 timedelta.c \
 thread.c thread.h \
 fsd_util.c util.h \
 drmaa_util.c drmaa_util.h \
 xmalloc.c xmalloc.h \
 exec.c exec.h

libdrmaa_utils_la_SOURCES = $(COMMON_SOURCES) \
 fsd_session.c session.h \
 drmaa_base.c drmaa_base.h


libdrmaa_utils_la_LDFLAGS = -static
libdrmaa_utils_la_CPPFLAGS = -fPIC

if GCC 
if !DEVELOPER_MODE
# allow `%m` in printf-like format string
# Bison and Ragel generated code have many unused symbols
libdrmaa_utils_la_CPPFLAGS +=  -Wno-unused
endif
endif

include_HEADERS = drmaa.h

BUILT_SOURCES = \
 datetime_tab.c datetime_tab.h \
 conf_tab.c conf_tab.h \
 drmaa_attrib.c \
 timedelta.c

EXTRA_DIST = drmaa_attrib.gperf timedelta.rl

if DEVELOPER_MODE
CLEANFILES = $(BUILT_SOURCES)
endif

drmaa_attrib.c: drmaa_attrib.gperf
	$(GPERF) $(GPERFFLAGS) --output-file=drmaa_attrib.c drmaa_attrib.gperf
timedelta.c: timedelta.rl
	$(RAGEL) $(RAGELFLAGS) -o timedelta.c timedelta.rl
	
bin_PROGRAMS = drmaa-run  drmaa-run-bulk drmaa-job-ps hpc-bash

drmaa_run_SOURCES = $(COMMON_SOURCES) \
 drmaa_run.c
 
drmaa_run_bulk_SOURCES = $(COMMON_SOURCES) \
 drmaa_run_bulk.c

drmaa_job_ps_SOURCES = $(COMMON_SOURCES) \
 drmaa_job_ps.c

drmaa_run_LDFLAGS = -ldl
drmaa_run_bulk_LDFLAGS = -ldl
drmaa_job_ps_LDFLAGS = -ldl

hpc_bash_SOURCES = $(COMMON_SOURCES) \
 hpc_bash.c


