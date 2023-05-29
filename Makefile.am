ACLOCAL_AMFLAGS         = -I config/m4
SUBDIRS = src
MAINTAINERCLEANFILES    = Makefile.in aclocal.m4 configure

EXTRA_DIST = doc examples

docs:
	cd docs/ja/;make latexpdf; make html
	cd docs/en/;make latexpdf; make html

pngdocs:
	cd docs/ja/;sed -i -e "s/mathjax/imgmath/g" conf.py; make latexpdf; make html
	cd docs/en/;sed -i -e "s/mathjax/imgmath/g" conf.py; make latexpdf; make html

docs-clean:
	cd docs/ja/; make clean; sed -i -e "s/imgmath/mathjax/g" conf.py
	cd docs/en/; make clean; sed -i -e "s/imgmath/mathjax/g" conf.py
