ACLOCAL_AMFLAGS         = -I config/m4
SUBDIRS = src
MAINTAINERCLEANFILES    = Makefile.in aclocal.m4 configure

EXTRA_DIST = doc examples

docs:
	cd doc/ja/;make latexpdf; make html
	cd doc/en/;make latexpdf; make html

pngdocs:
	cd doc/ja/;sed -i -e "s/mathjax/imgmath/g" conf.py; make latexpdf; make html
	cd doc/en/;sed -i -e "s/mathjax/imgmath/g" conf.py; make latexpdf; make html

docs-clean:
	cd doc/ja/; make clean; sed -i -e "s/imgmath/mathjax/g" conf.py
	cd doc/en/; make clean; sed -i -e "s/imgmath/mathjax/g" conf.py
