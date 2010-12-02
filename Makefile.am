# $Id: Makefile.am 1541 2008-10-21 11:58:51Z lukasz $
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

ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = \
 apidoc \
 autoclean.sh \
 autogen.sh \
 m4/bison_ylwrap.sh \
 m4/missing-dev-prog.sh

SUBDIRS = drmaa_utils test

apidoc-timestamp:
	if [ ! -f $@ \
		-o `find $(top_srcdir) \( -name '*.c' -o -name '*.h' \) -a -newer $@ \
			| wc -l` -gt 0 \
		-o Doxyfile -nt $@ \
		]; then \
			rm -rf apidoc; \
			doxygen && touch $@ || exit 1; \
	fi
dist_data_DATA = apidoc-timestamp
apidoc: apidoc-timestamp
