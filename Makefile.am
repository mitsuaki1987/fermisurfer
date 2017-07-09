ACLOCAL_AMFLAGS         = -I config/m4
SUBDIRS = src
MAINTAINERCLEANFILES    = Makefile.in aclocal.m4 configure

EXTRA_DIST = doc bin

docs:
	cd doc/ja/;make latexpdfja; make html
	cd doc/en/;make latexpdfja; make html

pngdocs:
	cd doc/ja/;sed -i -e "s/mathjax/pngmath/g" conf.py; make latexpdfja; make html
	cd doc/en/;sed -i -e "s/mathjax/pngmath/g" conf.py; make latexpdfja; make html

docs-clean:
	cd doc/ja/; make clean; sed -i -e "s/pngmath/mathjax/g" conf.py
	cd doc/en/; make clean; sed -i -e "s/pngmath/mathjax/g" conf.py
