#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([chomik], [0.0.5], [pawel.f.biernacki@gmail.com])
AC_CONFIG_SRCDIR([src/chomik.cc])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE()
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC
LT_INIT

PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

dnl #################################################################################
dnl #
dnl # SDL2 - check the library and header file.
dnl #

	AC_MSG_CHECKING(sdl2)
	AC_CHECK_LIB(SDL2, SDL_Init,
			[],
			[
	AC_MSG_ERROR([The SDL2 library not found. Check that it is installed on your system. If it is not, download and install it 
	(preferably using your configuration manager -	i.e. the application called Add/Remove Programs or something like yum or dnf.) 
	Remember to install the DEVEL version. If it is installed elswehere you can run the script "configure" with the option --libdir=DIR. 
	Execute "configure -h" for more info.])
			])

	AC_SUBST(SDL2_CFLAGS, `pkg-config --cflags sdl2`)
	AC_SUBST(SDL2_LIBS, `pkg-config --libs sdl2`)
	AC_SUBST(CFLAGS, ["$CFLAGS $SDL2_CFLAGS"])
	AC_SUBST(CPPFLAGS, ["$CPPFLAGS $SDL2_CFLAGS"])
	AC_CHECK_HEADERS([SDL2/SDL.h], [], 
		[
	AC_MSG_ERROR(
	[The SDL library was found, but there is no corresponding header file. Check that the DEVEL version of the library 
	is installed on your system.])
		])

dnl #################################################################################
dnl #
dnl # SDL_image - check the library and header file.
dnl #

AC_CHECK_LIB(SDL2_image, IMG_Load, [], [
	AC_MSG_ERROR([
	The SDL2_image library not found. Check that it is installed on your system. If it is not, download and install it 
	(preferably using your configuration manager -	i.e. the application called Add/Remove Programs or something like yum or dnf.) 
	Remember to install the DEVEL version. If it is installed elswehere you can run the script "configure" with the option --libdir=DIR. 
	Execute "configure -h" for more info.
		])
	])

AC_CHECK_HEADERS([SDL2/SDL_image.h], [], 
			[
		AC_MSG_ERROR(
	[The SDL2_image library was found, but there is no corresponding header file. Check that the DEVEL version of the library 
	is installed on your system.])
			])




AC_CONFIG_FILES([Makefile
                 examples/Makefile
                 image/Makefile
                 doc/Makefile
                 dox/Makefile
                 inc/Makefile
                 src/Makefile
                 src2/Makefile
                 tests/Makefile libchomik.pc ])
AC_OUTPUT
