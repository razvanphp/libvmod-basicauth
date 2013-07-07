VMOD_TESTS = \
 test00.vtc\
 test01.vtc\
 test02.vtc

EXTRA_DIST=$(VMOD_TESTS) htpasswd

check:
	cd $(abs_srcdir); \
	for t in $(VMOD_TESTS); do \
	    $(VARNISHSRC)/bin/varnishtest/varnishtest \
                    -Dvarnishd=$(VARNISHSRC)/bin/varnishd/varnishd \
	            -Dvmod_topsrc=$(abs_top_srcdir) \
                    -Dvmod_topbuild=$(abs_top_builddir) $$t; \
        done
