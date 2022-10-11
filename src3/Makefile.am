

bin_PROGRAMS = http_chomik

http_chomik_SOURCES = http_chomik.cc

http_chomik_LDADD = -L../src -lchomik

http_chomikdir = $(datadir)/http_chomik

AM_CXXFLAGS = @CXXFLAGS@ -fconcepts -std=c++17 \
	-I.. -I../inc \
	-DDATADIR=\"$(datadir)\"


