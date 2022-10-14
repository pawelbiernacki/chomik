

bin_PROGRAMS = http_chomik fancy_http_chomik

http_chomik_SOURCES = http_chomik.cc 


http_chomik_LDADD = -L../src -L../src3 -lchomik -lhttpchomik


http_chomikdir = $(datadir)/http_chomik

AM_CXXFLAGS = @CXXFLAGS@ -fconcepts -std=c++17 \
	-I.. -I../inc \
	-DDATADIR=\"$(datadir)\"

http_chomik_DATA = \
	../image/chomik.png \
	../css/chomik.css

fancy_http_chomik_SOURCES = fancy_http_chomik.cc
fancy_http_chomik_LDADD = -L../src -L../src3 -lchomik -lhttpchomik
