##
## Process this file with automake to produce Makefile.in
##

SUBDIRS = src

EXTRA_DIST = LICENSE Doxyfile gloox.pc.in gloox.cbp gloox.dsp README README.win32 README.wince INSTALL config.h.win

pkgconfigdir=$(libdir)/pkgconfig
pkgconfig_DATA = gloox.pc

bin_SCRIPTS = gloox-config

apidox:
	doxygen

check: test

test:
	make -C src test

leakcheck: leaktest

leaktest:
	make -C src leaktest

examples:
	make -C src examples
