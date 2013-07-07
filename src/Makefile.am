AM_CPPFLAGS = -I$(VARNISHSRC)/include -I$(VARNISHSRC)

dist_man_MANS = vmod_basicauth.3

vmoddir = $(VMODDIR)
vmod_LTLIBRARIES = libvmod_basicauth.la

libvmod_basicauth_la_LDFLAGS = -module -export-dynamic -avoid-version
libvmod_basicauth_la_LIBADD=-lcrypt

libvmod_basicauth_la_SOURCES = \
	basicauth.h\
	md5.c\
	vcc_if.c \
	vcc_if.h \
	vmod_basicauth.c

vcc_if.c vcc_if.h: $(VARNISHSRC)/lib/libvmod_std/vmod.py $(top_srcdir)/src/vmod_basicauth.vcc
	@PYTHON@ $(VARNISHSRC)/lib/libvmod_std/vmod.py $(top_srcdir)/src/vmod_basicauth.vcc

EXTRA_DIST = \
	vmod_basicauth.vcc

CLEANFILES = $(builddir)/vcc_if.c $(builddir)/vcc_if.h

