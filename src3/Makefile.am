


lib_LTLIBRARIES = libhttpchomik.la

libhttpchomik_la_SOURCES = machine.cc server.cc wrapper_base.cc

http_chomikdir = $(datadir)/http_chomik

AM_CXXFLAGS = @CXXFLAGS@ -fconcepts -std=c++17 \
	-I.. -I../inc \
	-DDATADIR=\"$(datadir)\"
