# This file is part of vmod-basicauth 
# Copyright (C) 2013-2014 Sergey Poznyakoff
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

AM_CPPFLAGS = -I$(VARNISHSRC)/include -I$(VARNISHSRC)

dist_man_MANS = vmod-basicauth.3 vmod_basicauth.3

vmoddir = $(VMODDIR)
vmod_LTLIBRARIES = libvmod_basicauth.la

libvmod_basicauth_la_LDFLAGS = -module -export-dynamic -avoid-version
libvmod_basicauth_la_LIBADD=-lcrypt

libvmod_basicauth_la_SOURCES = \
	basicauth.h\
	md5.c\
	sha1.c\
	sha1.h\
	vmod_basicauth.c

nodist_libvmod_basicauth_la_SOURCES = vcc_if.c vcc_if.h

vmod_basicauth.lo: vcc_if.h

CLEANFILES = vcc_if.c vcc_if.h *.rst

if VARNISH4
  vmodtool = $(VARNISHSRC)/lib/libvcc/vmodtool.py
  vmodtoolargs = --strict
  vccfile = $(top_srcdir)/src/vmod_basicauth.vcc
else
  vmodtool = $(VARNISHSRC)/lib/libvmod_std/vmod.py
  vmodtoolargs =
  vccfile = vmod_basicauth.vcc3

vmod_basicauth.vcc3: $(top_srcdir)/src/vmod_basicauth.vcc
  CLEANFILES += vmod_basicauth.vcc3
endif

.vcc.vcc3:
	sed -n '/^\$$/{s///;s/\(Module  *[^ ][^ ]*\).*/\1/;p}' $< > $@

vcc_if.c vcc_if.h: $(vmodtool) $(vccfile)
	@PYTHON@ $(vmodtool) $(vmodtoolargs) $(vccfile)

EXTRA_DIST = \
	vmod_basicauth.vcc


