# This file is part of vmod-basicauth 
# Copyright (C) 2013 Sergey Poznyakoff
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
	vcc_if.c \
	vcc_if.h \
	vmod_basicauth.c

BUILT_SOURCES = vcc_if.c vcc_if.h

vcc_if.c vcc_if.h: $(VARNISHSRC)/lib/libvmod_std/vmod.py $(top_srcdir)/src/vmod_basicauth.vcc
	@PYTHON@ $(VARNISHSRC)/lib/libvmod_std/vmod.py $(top_srcdir)/src/vmod_basicauth.vcc

EXTRA_DIST = \
	vmod_basicauth.vcc

CLEANFILES = $(builddir)/vcc_if.c $(builddir)/vcc_if.h

