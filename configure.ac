#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([chomik], [0.2.1], [pawel.f.biernacki@gmail.com])
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



AC_MSG_CHECKING(--enable-chomik-servers argument)
AC_ARG_ENABLE(chomik-servers,
    [  --enable-chomik-servers          Build the http library and servers.],
    [enable_chomik_servers=$enableval],
    [enable_chomik_servers="yes"])
AC_MSG_RESULT($enable_chomik_servers)
if test "$enable_chomik_servers" = "yes"; then
  AC_SUBST(chomik_servers_dirs, "src3 src4")
fi

AC_MSG_CHECKING(--enable-windows-build argument)
AC_ARG_ENABLE(windows-build,
    [  --enable-windows-build          Build for Windows.],
    [enable_windows_build=$enableval],
    [enable_windows_build="no"])
AC_MSG_RESULT($enable_windows_build)
if test "$enable_windows_build" = "yes"; then
  AC_SUBST(chomik_linker_flags, "-static -static-libgcc -static-libstdc++")
  AC_SUBST(chomik_compiler_flags, "-static")
  AC_SUBST(sdl_chomik_linker_flags, "-static -static-libgcc -static-libstdc++")
  AC_SUBST(sdl_chomik_compiler_flags, "-static")
  AC_SUBST(sdl_chomik_libs, " -lmingw32 -lSDL2main")
fi

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_MSG_CHECKING(--enable-sdl-chomik argument)
AC_ARG_ENABLE(sdl-chomik,
    [  --enable-sdl-chomik				Build SDL_chomik.],
    [enable_sdl_chomik=$enableval],
    [enable_sdl_chomik="yes"])
AC_MSG_RESULT($enable_sdl_chomik)
AC_SUBST(sdl_chomik_dir, "")
if test "$enable_sdl_chomik" = "yes"; then

	AC_SUBST(sdl_chomik_dir, "src2")

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
dnl # SDL2_image - check the library and header file.
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

dnl #################################################################################
dnl #
dnl # SDL2_ttf - check the library and header file.
dnl #

AC_CHECK_LIB(SDL2_ttf, TTF_Init, [], [
	AC_MSG_ERROR([
	The SDL2_ttf library not found. Check that it is installed on your system. If it is not, download and install it
	(preferably using your configuration manager -	i.e. the application called Add/Remove Programs or something like yum or dnf.)
	Remember to install the DEVEL version. If it is installed elswehere you can run the script "configure" with the option --libdir=DIR.
	Execute "configure -h" for more info.
		])
	])

	AC_SUBST(SDL2_TTF_CFLAGS, `pkg-config --cflags SDL2_ttf`)
	AC_SUBST(SDL2_TTF_LIBS, `pkg-config --libs SDL2_ttf`)
	AC_SUBST(CFLAGS, ["$CFLAGS $SDL2_TTF_CFLAGS"])
	AC_SUBST(CPPFLAGS, ["$CPPFLAGS $SDL2_TTF_CFLAGS"])
	AC_CHECK_HEADERS([SDL2/SDL_ttf.h], [],
		[
	AC_MSG_ERROR(
	[The SDL2_ttf library was found, but there is no corresponding header file. Check that the DEVEL version of the library
	is installed on your system.])
		])
fi



AC_CONFIG_FILES([Makefile
                 3d/Makefile
                 css/Makefile
                 examples/Makefile
                 font/Makefile
                 image/Makefile
                 doc/Makefile
                 dox/Makefile
                 inc/Makefile
                 src/Makefile
                 src2/Makefile
                 src3/Makefile
                 src4/Makefile
                 tests/Makefile libchomik.pc libhttpchomik.pc])
AC_OUTPUT
