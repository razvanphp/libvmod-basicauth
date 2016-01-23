# This file is part of vmod-basicauth
# Copyright (C) 2013-2016 Sergey Poznyakoff
#
# Vmod-basicauth is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# Vmod-basicauth is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with vmod-basicauth.  If not, see <http://www.gnu.org/licenses/>.

VMOD_TESTS = \
 test00.vtc\
 test01.vtc\
 test02.vtc\
 test03.vtc

EXTRA_DIST=$(VMOD_TESTS) htpasswd

check:
	$(AM_V_at)cd $(abs_srcdir); \
	for t in $(VMOD_TESTS); do \
	    $(VARNISHSRC)/bin/varnishtest/varnishtest \
                    -Dvarnishd=$(VARNISHSRC)/bin/varnishd/varnishd \
	            -Dvmod_topsrc=$(abs_top_srcdir) \
                    -Dvmod_topbuild=$(abs_top_builddir) $$t; \
        done
