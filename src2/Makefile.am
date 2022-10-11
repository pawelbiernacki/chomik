

bin_PROGRAMS = sdl_chomik

sdl_chomik_SOURCES = sdl_chomik.cc

sdl_chomik_LDADD = -L../src -lchomik @SDL2_LIBS@ @SDL2_TTF_LIBS@

sdl_chomikdir = $(datadir)/sdl_chomik

AM_CXXFLAGS = @CXXFLAGS@ -fconcepts -std=c++17 \
	-I.. -I../inc \
	-DDATADIR=\"$(datadir)\"

	
