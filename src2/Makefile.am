

bin_PROGRAMS = sdl_chomik

sdl_chomik_SOURCES = sdl_chomik.cc

sdl_chomik_LDFLAGS = @sdl_chomik_linker_flags@
sdl_chomik_LDADD = -L../src -lchomik @sdl_chomik_libs@ @SDL2_LIBS@ @SDL2_TTF_LIBS@

sdl_chomikdir = $(datadir)/sdl_chomik

AM_CXXFLAGS = @CXXFLAGS@ -fconcepts -std=c++17 @sdl_chomik_compiler_flags@ \
	-I.. -I../inc \
	-DDATADIR=\"$(datadir)\"

	
