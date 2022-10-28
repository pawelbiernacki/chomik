

SUBDIRS = 3d css examples font image doc dox inc src src2 $(chomik_servers_dirs) tests

EXTRA_DIST = Doxyfile

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = libchomik.pc libhttpchomik.pc

documents:
	doxygen && cp dox/www_perkun_org.html_files/*.png doc/html
