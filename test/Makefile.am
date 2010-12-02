# $Id: Makefile.am 1407 2008-09-25 08:51:51Z lukasz $
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

noinst_PROGRAMS = test_exception

test_exception_SOURCES = test_exception.c
test_exception_LDFLAGS = -Wl,-R $(top_builddir)/drmaa_utils
test_exception_LDADD = $(top_builddir)/drmaa_utils/libdrmaa_utils.la

