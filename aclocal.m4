# generated automatically by aclocal 1.15 -*- Autoconf -*-

# Copyright (C) 1996-2014 Free Software Foundation, Inc.

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

m4_ifndef([AC_CONFIG_MACRO_DIRS], [m4_defun([_AM_CONFIG_MACRO_DIRS], [])m4_defun([AC_CONFIG_MACRO_DIRS], [_AM_CONFIG_MACRO_DIRS($@)])])
m4_ifndef([AC_AUTOCONF_VERSION],
  [m4_copy([m4_PACKAGE_VERSION], [AC_AUTOCONF_VERSION])])dnl
m4_if(m4_defn([AC_AUTOCONF_VERSION]), [2.69],,
[m4_warning([this file was generated for autoconf 2.69.
You have another version of autoconf.  It may work, but is not guaranteed to.
If you have problems, you may need to regenerate the build system entirely.
To do so, use the procedure documented by the package, typically 'autoreconf'.])])

# ===========================================================================
#        http://www.gnu.org/software/autoconf-archive/ax_check_gl.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_CHECK_GL
#
# DESCRIPTION
#
#   Check for an OpenGL implementation. If GL is found, the required
#   compiler and linker flags are included in the output variables
#   "GL_CFLAGS", "GL_LIBS", "GL_LDFLAGS", respectively. If no usable GL
#   implementation is found, "no_gl" is set to "yes".
#
#   You could disable OpenGL using --with-gl=no
#
#   You could choose a specific OpenGL libs using --with-gl=lib_name
#
#   Under darwin, cygwin and mingw target you could prefer the OpenGL
#   implementation that link with X setting --with-gl=x or without X support
#   with --with-gl=nox. Notes that this script try to guess the right
#   implementation.
#
#   If the header "GL/gl.h" is found, "HAVE_GL_GL_H" is defined. If the
#   header "OpenGL/gl.h" is found, HAVE_OPENGL_GL_H is defined. These
#   preprocessor definitions may not be mutually exclusive.
#
#   You should use something like this in your headers:
#
#     #if defined(HAVE_WINDOWS_H) && defined(_WIN32)
#     # include <windows.h>
#     #endif
#     #ifdef HAVE_GL_GL_H
#     # include <GL/gl.h>
#     #elif defined(HAVE_OPENGL_GL_H)
#     # include <OpenGL/gl.h>
#     #else
#     # error no gl.h
#     #endif
#
# LICENSE
#
#   Copyright (c) 2009 Braden McDaniel <braden@endoframe.com>
#   Copyright (c) 2012 Bastien Roucaries <roucaries.bastien+autoconf@gmail.com>
#
#   This program is free software; you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation; either version 2 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.

#serial 17

m4_define([_AX_CHECK_GL_PROGRAM],
          [AC_LANG_PROGRAM([[
# if defined(HAVE_WINDOWS_H) && defined(_WIN32)
#   include <windows.h>
# endif
# ifdef HAVE_GL_GL_H
#   include <GL/gl.h>
# elif defined(HAVE_OPENGL_GL_H)
#   include <OpenGL/gl.h>
# else
#   error no gl.h
# endif
]],[[glBegin(0)]])])

dnl Default include : add windows.h
dnl see http://www.opengl.org/wiki/Platform_specifics:_Windows
dnl (acceded 20120801)
AC_DEFUN([_AX_CHECK_GL_INCLUDES_DEFAULT],dnl
[
  AC_INCLUDES_DEFAULT
  [
  # if defined(HAVE_WINDOWS_H) && defined(_WIN32)
  #   include <windows.h>
  # endif
  ]
])

dnl local save flags
AC_DEFUN([_AX_CHECK_GL_SAVE_FLAGS],
[dnl
ax_check_gl_saved_libs="${LIBS}"
ax_check_gl_saved_cflags="${CFLAGS}"
ax_check_gl_saved_cppflags="${CPPFLAGS}"
ax_check_gl_saved_ldflags="${LDFLAGS}"
])

dnl local restore flags
AC_DEFUN([_AX_CHECK_GL_RESTORE_FLAGS],
[dnl
LIBS="${ax_check_gl_saved_libs}"
CFLAGS="${ax_check_gl_saved_cflags}"
CPPFLAGS="${ax_check_gl_saved_cppflags}"
LDFLAGS="${ax_check_gl_saved_ldflags}"
])

dnl default switch case failure
AC_DEFUN([_AX_CHECK_MSG_FAILURE_ORDER],
[dnl
 AC_MSG_FAILURE([Order logic in ax_check_gl is buggy])
])

# set the varible ax_check_gl_need_x
# this variable determine if we need opengl that link with X
# value are default aka try the first library wether if it link or not with x
# yes that means we need a opengl with x
# no that means we do not need an opengl with x
AC_DEFUN([_AX_CHECK_GL_NEED_X],
[dnl
 # do not check if empty : allow a subroutine to modify the choice
 AS_IF([test "X$ax_check_gl_need_x" = "X"],
       [ax_check_gl_need_x="default"
        AS_IF([test "X$no_x" = "Xyes"],[ax_check_gl_need_x="no"])
        AS_IF([test "X$ax_check_gl_want_gl" = "Xnox"],[ax_check_gl_need_x="no"])
        AS_IF([test "X$ax_check_gl_want_gl" = "Xx"],[ax_check_gl_need_x="yes"])])
])

# compile the example program
AC_DEFUN([_AX_CHECK_GL_COMPILE],
[dnl
 AC_LANG_PUSH([C])
 _AX_CHECK_GL_SAVE_FLAGS()
 CFLAGS="${GL_CFLAGS} ${CFLAGS}"
 AC_COMPILE_IFELSE([_AX_CHECK_GL_PROGRAM],
                   [ax_check_gl_compile_opengl="yes"],
                   [ax_check_gl_compile_opengl="no"])
 _AX_CHECK_GL_RESTORE_FLAGS()
 AC_LANG_POP([C])
])

# compile the example program (cache)
AC_DEFUN([_AX_CHECK_GL_COMPILE_CV],
[dnl
 AC_CACHE_CHECK([for compiling a minimal OpenGL program],[ax_cv_check_gl_compile_opengl],
                [_AX_CHECK_GL_COMPILE()
                 ax_cv_check_gl_compile_opengl="${ax_check_gl_compile_opengl}"])
 ax_check_gl_compile_opengl="${ax_cv_check_gl_compile_opengl}"
])

# link the example program
AC_DEFUN([_AX_CHECK_GL_LINK],
[dnl
 AC_LANG_PUSH([C])
 _AX_CHECK_GL_SAVE_FLAGS()
 CFLAGS="${GL_CFLAGS} ${CFLAGS}"
 LIBS="${GL_LIBS} ${LIBS}"
 LDFLAGS="${GL_LDFLAGS} ${LDFLAGS}"
 AC_LINK_IFELSE([_AX_CHECK_GL_PROGRAM],
                [ax_check_gl_link_opengl="yes"],
                [ax_check_gl_link_opengl="no"])
 _AX_CHECK_GL_RESTORE_FLAGS()
 AC_LANG_POP([C])
])

# link the example program (cache)
AC_DEFUN([_AX_CHECK_GL_LINK_CV],
[dnl
 AC_CACHE_CHECK([for linking a minimal OpenGL program],[ax_cv_check_gl_link_opengl],
                [_AX_CHECK_GL_LINK()
                 ax_cv_check_gl_link_opengl="${ax_check_gl_link_opengl}"])
 ax_check_gl_link_opengl="${ax_cv_check_gl_link_opengl}"
])

dnl Check headers manually (default case)
AC_DEFUN([_AX_CHECK_GL_MANUAL_HEADERS_DEFAULT],
[AC_REQUIRE([AC_PATH_XTRA])
 AC_LANG_PUSH([C])
 _AX_CHECK_GL_SAVE_FLAGS()
 CFLAGS="${GL_CFLAGS} ${CFLAGS}"
 # see comment in _AX_CHECK_GL_INCLUDES_DEFAULT
 AC_CHECK_HEADERS([windows.h],[],[],[AC_INCLUDES_DEFAULT])
 # FIXME: use extra cflags
 AC_CHECK_HEADERS([GL/gl.h],[ax_check_gl_have_headers="yes"],
                            [ax_check_gl_have_headers_headers="no"],
			    [_AX_CHECK_GL_INCLUDES_DEFAULT()])
 # do not try darwin specific OpenGl/gl.h
 _AX_CHECK_GL_RESTORE_FLAGS()
 AC_LANG_POP([C])
])

# darwin headers without X
AC_DEFUN([_AX_CHECK_GL_MANUAL_HEADERS_DARWIN_NOX],[
 AC_LANG_PUSH([C])
 _AX_CHECK_GL_SAVE_FLAGS()
 # FIXME: use -framework opengl as an extra cflags
 CFLAGS="-framework opengl ${GL_CFLAGS} ${CFLAGS}"
 AC_CHECK_HEADERS([OpenGL/gl.h],[ax_check_gl_have_headers="yes"],
                                [ax_check_gl_have_headers_headers="no"],
			        [_AX_CHECK_GL_INCLUDES_DEFAULT()])
 AS_IF([test "X$ax_check_gl_have_headers" = "yes"],
       [GL_CFLAGS="-framework opengl ${GL_CFLAGS}"])
 _AX_CHECK_GL_SAVE_FLAGS()
 AC_LANG_POP([C])
])

# check header for darwin
AC_DEFUN([_AX_CHECK_GL_MANUAL_HEADERS_DARWIN],
[AC_REQUIRE([_AX_CHECK_GL_NEED_X])dnl
 AS_CASE(["$ax_check_gl_order"],
         # try to use framework
         ["gl"],[_AX_CHECK_GL_MANUAL_HEADERS_DARWIN_NOX()],
	 # try to use framework then mesa (X)
	 ["gl mesagl"],[
	   _AX_CHECK_GL_MANUAL_HEADERS_DARWIN_NOX()
	   AS_IF([test "X$ax_check_gl_have_headers" = "yes"],
	         [ax_check_gl_order="gl"
		  ax_check_gl_need_x="yes"],
		 [ax_check_gl_order="mesagl gl"
		  ax_check_gl_need_x="no"]
		  # retry with general test
		  _AX_CHECK_GL_MANUAL_HEADERS_DEFAULT()])],
         ["mesagl gl"],[
	   _AX_CHECK_GL_MANUAL_HEADERS_DEFAULT()
	   AS_IF([test "X$ax_check_gl_have_headers" = "yes"],
	         [ax_check_gl_order="mesagl gl"
		  ax_check_gl_need_x="no"],
		 [ax_check_gl_order="gl"
		  ax_check_gl_need_x="yes"
		  # retry with framework
		  _AX_CHECK_GL_MANUAL_HEADERS_DARWIN_NOX()])],
        [_AX_CHECK_MSG_FAILURE_ORDER()])
])

dnl Check headers manually: subroutine must set ax_check_gl_have_headers={yes,no}
AC_DEFUN([_AX_CHECK_GL_MANUAL_HEADERS],
[AC_REQUIRE([AC_CANONICAL_HOST])
 AS_CASE([${host}],
         [*-darwin*],[_AX_CHECK_GL_MANUAL_HEADERS_DARWIN],
	 [_AX_CHECK_GL_MANUAL_HEADERS_DEFAULT()])
 AC_CACHE_CHECK([for OpenGL headers],[ax_cv_check_gl_have_headers],
               	[ax_cv_check_gl_have_headers="${ax_check_gl_have_headers}"])
])

# dnl try to found library (generic case)
# dnl $1 is set to the library to found
AC_DEFUN([_AX_CHECK_GL_MANUAL_LIBS_GENERIC],
[dnl
 ax_check_gl_manual_libs_generic_extra_libs="$1"
 AS_IF([test "X$ax_check_gl_manual_libs_generic_extra_libs" = "X"],
       [AC_MSG_ERROR([AX_CHECK_GL_MANUAL_LIBS_GENERIC argument must no be empty])])

 AC_LANG_PUSH([C])
 _AX_CHECK_GL_SAVE_FLAGS()
 CFLAGS="${GL_CFLAGS} ${CFLAGS}"
 LIBS="${GL_LIBS} ${LIBS}"
 AC_SEARCH_LIBS([glBegin],[$ax_check_gl_manual_libs_generic_extra_libs],
                [ax_check_gl_lib_opengl="yes"],
                [ax_check_gl_lib_opengl="no"])
 AS_CASE([$ac_cv_search_glBegin],
         ["none required"],[],
 	 [no],[],
 	 [GL_LIBS="${ac_cv_search_glBegin} ${GL_LIBS}"])
  _AX_CHECK_GL_RESTORE_FLAGS()
  AC_LANG_PUSH([C])
])

# dnl try to found lib under darwin
# darwin opengl hack
# see http://web.archive.org/web/20090410052741/http://developer.apple.com/qa/qa2007/qa1567.html
# and http://web.eecs.umich.edu/~sugih/courses/eecs487/glut-howto/
AC_DEFUN([_AX_CHECK_GL_MANUAL_LIBS_DARWIN],
[# ldhack list
 ldhack1 = "-Wl,-framework,OpenGL"
 ldhack2 = "-Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib"
 ldhack3 = "$ldhack1,$ldhack2"

 # select hack
 AS_IF([test "X$ax_check_gl_need_x" = "Xyes"],
       [# libs already set by -framework cflag
        darwinlibs=""
        ldhacks="$ldhack1 $ldhack2 $ldhack3"],
       [# do not use framework ldflags in case of x version
        darwinlibs="GL gl MesaGL"
        ldhack="$ldhack2"])

 ax_check_gl_link_opengl="no"
 for extralibs in " " $darwinlibs; do
   for extraldflags in " " ldhacks; do
       AC_LANG_PUSH([C])
        _AX_CHECK_GL_SAVE_FLAGS()
       CFLAGS="${GL_CFLAGS} ${CFLAGS}"
       LIBS="$extralibs ${GL_LIBS} ${LIBS}"
       LDFLAGS="$extraldflags ${GL_LDFLAGS} ${LDFLAGS}"
       AC_LINK_IFELSE([_AX_CHECK_GL_PROGRAM],
                      [ax_check_gl_link_opengl="yes"],
                      [ax_check_gl_link_opengl="no"])
       _AX_CHECK_GL_RESTORE_FLAGS()
       AC_LANG_POP([C])
       AS_IF([test "X$ax_check_gl_link_opengl" = "Xyes"],[break])
   done;
   AS_IF([test "X$ax_check_gl_link_opengl" = "Xyes"],[break])
 done;
 GL_LIBS="$extralibs ${GL_LIBS}"
 GL_LDFLAGS="$extraldflags ${GL_LDFLAGS}"
])

dnl Check library manually: subroutine must set
dnl $ax_check_gl_lib_opengl={yes,no}
AC_DEFUN([_AX_CHECK_GL_MANUAL_LIBS],
[AC_REQUIRE([AC_CANONICAL_HOST])
 AS_CASE([${host}],
         [*-darwin*],[_AX_CHECK_GL_MANUAL_LIBS_DARWIN()],
         # try first cygwin version
         [*-cygwin*|*-mingw*],[
	   AS_CASE(["$ax_check_gl_order"],
	           ["gl"],[_AX_CHECK_GL_MANUAL_LIBS_GENERIC([opengl32])],
		   ["gl mesagl"],[_AX_CHECK_GL_MANUAL_LIBS_GENERIC([opengl32 GL gl MesaGL])],
		   ["mesagl gl"],[_AX_CHECK_GL_MANUAL_LIBS_GENERIC([GL gl MesaGL opengl32])],
		   [_AX_CHECK_MSG_FAILURE_ORDER()])],
	 [AS_CASE(["$ax_check_gl_order"],
                  ["gl"],[_AX_CHECK_GL_MANUAL_LIBS_GENERIC([GL gl])],
		  ["gl mesagl"],[_AX_CHECK_GL_MANUAL_LIBS_GENERIC([GL gl MesaGL])],
		  ["mesagl gl"],[_AX_CHECK_GL_MANUAL_LIBS_GENERIC([MesaGL GL gl])],
		  [_AX_CHECK_MSG_FAILURE_ORDER()])]
		  )

 AC_CACHE_CHECK([for OpenGL libraries],[ax_cv_check_gl_lib_opengl],
               	[ax_cv_check_gl_lib_opengl="${ax_check_gl_lib_opengl}"])
 ax_check_gl_lib_opengl="${ax_cv_check_gl_lib_opengl}"
])

# manually check aka old way
AC_DEFUN([_AX_CHECK_GL_MANUAL],
[AC_REQUIRE([AC_CANONICAL_HOST])dnl
 AC_REQUIRE([AC_PATH_XTRA])dnl

 no_gl="yes"

 _AX_CHECK_GL_MANUAL_HEADERS()
 AS_IF([test "X$ax_check_gl_have_headers" = "Xyes"],
       [_AX_CHECK_GL_COMPILE_CV()],
       [ax_check_gl_compile_opengl=no])

 AS_IF([test "X$ax_check_gl_compile_opengl" = "Xyes"],
       [_AX_CHECK_GL_MANUAL_LIBS],
       [ax_check_gl_lib_opengl=no])

 AS_IF([test "X$ax_check_gl_lib_opengl" = "Xyes"],
       [_AX_CHECK_GL_LINK_CV()],
       [ax_check_gl_link_opengl=no])

 AS_IF([test "X$ax_check_gl_link_opengl" = "Xyes"],
       [no_gl="no"],
       [no_gl="yes"])
])dnl


# try to test using pkgconfig: set ax_check_gl_pkg_config=no if not found
AC_DEFUN([_AX_CHECK_GL_PKG_CONFIG],dnl
[dnl
 AC_REQUIRE([PKG_PROG_PKG_CONFIG])

 dnl First try mesagl
 AS_CASE(["$ax_check_gl_order"],
         ["gl"],[PKG_CHECK_MODULES([GL],[mesagl],
	                  [ax_check_gl_pkg_config=yes],
			  [ax_check_gl_pkg_config=no])],
	 ["gl mesagl"],[PKG_CHECK_MODULES([GL],[gl],
	                  [ax_check_gl_pkg_config=yes],
			  [PKG_CHECK_MODULES([GL],[mesagl],
	                         [ax_check_gl_pkg_config=yes],
				 [ax_check_gl_pkg_config=no])])],
	 ["mesagl gl"],[PKG_CHECK_MODULES([GL],[mesagl],
	                  [ax_check_gl_pkg_config=yes],
			  [PKG_CHECK_MODULES([GL],[gl],
	                         [ax_check_gl_pkg_config=yes],
				 [ax_check_gl_pkg_config=no])])],
	 [_AX_CHECK_MSG_FAILURE_ORDER])

 AS_IF([test "X$ax_check_gl_pkg_config" = "Xyes"],[
        # check headers
        AC_LANG_PUSH([C])
 	_AX_CHECK_GL_SAVE_FLAGS()
        CFLAGS="${GL_CFLAGS} ${CFLAGS}"
        AC_CHECK_HEADERS([windows.h],[],[],[AC_INCLUDES_DEFAULT])
        AC_CHECK_HEADERS([GL/gl.h OpenGL/gl.h],
                         [ax_check_gl_have_headers="yes";break],
                         [ax_check_gl_have_headers_headers="no"],
			 [_AX_CHECK_GL_INCLUDES_DEFAULT()])
        _AX_CHECK_GL_RESTORE_FLAGS()
	AC_LANG_POP([C])
	AC_CACHE_CHECK([for OpenGL headers],[ax_cv_check_gl_have_headers],
               	       [ax_cv_check_gl_have_headers="${ax_check_gl_have_headers}"])

        # pkgconfig library are suposed to work ...
        AS_IF([test "X$ax_cv_check_gl_have_headers" = "Xno"],
              [AC_MSG_ERROR("Pkgconfig detected OpenGL library has no headers!")])

	_AX_CHECK_GL_COMPILE_CV()
	AS_IF([test "X$ax_cv_check_gl_compile_opengl" = "Xno"],
              [AC_MSG_ERROR("Pkgconfig detected opengl library could not be used for compiling minimal program!")])

	_AX_CHECK_GL_LINK_CV()
	AS_IF([test "X$ax_cv_check_gl_link_opengl" = "Xno"],
              [AC_MSG_ERROR("Pkgconfig detected opengl library could not be used for linking minimal program!")])
  ],[ax_check_gl_pkg_config=no])
])

# test if gl link with X
AC_DEFUN([_AX_CHECK_GL_WITH_X],
[
 # try if opengl need X
 AC_LANG_PUSH([C])
 _AX_CHECK_GL_SAVE_FLAGS()
 CFLAGS="${GL_CFLAGS} ${CFLAGS}"
 LIBS="${GL_LIBS} ${LIBS}"
 LDFLAGS="${GL_LDFLAGS} ${LDFLAGS}"
 AC_LINK_IFELSE([AC_LANG_CALL([], [glXQueryVersion])],
                [ax_check_gl_link_implicitly_with_x="yes"],
   	        [ax_check_gl_link_implicitly_with_x="no"])
 _AX_CHECK_GL_RESTORE_FLAGS()
 AC_LANG_POP([C])
])

# internal routine: entry point if gl not disable
AC_DEFUN([_AX_CHECK_GL],[dnl
 AC_REQUIRE([PKG_PROG_PKG_CONFIG])
 AC_REQUIRE([AC_PATH_X])dnl

 # does we need X or not
 _AX_CHECK_GL_NEED_X()

 # try first pkgconfig
 AC_MSG_CHECKING([for a working OpenGL implementation by pkg-config])
 AS_IF([test "X${PKG_CONFIG}" = "X"],
       [ AC_MSG_RESULT([no])
         ax_check_gl_pkg_config=no],
       [ AC_MSG_RESULT([yes])
         _AX_CHECK_GL_PKG_CONFIG()])

 # if no pkgconfig or pkgconfig fail try manual way
 AS_IF([test "X$ax_check_gl_pkg_config" = "Xno"],
       [_AX_CHECK_GL_MANUAL()],
       [no_gl=no])

 # test if need to test X compatibility
 AS_IF([test $no_gl = no],
       [# test X compatibility
 	AS_IF([test X$ax_check_gl_need_x != "Xdefault"],
       	      [AC_CACHE_CHECK([wether OpenGL link implictly with X],[ax_cv_check_gl_link_with_x],
                              [_AX_CHECK_GL_WITH_X()
                               ax_cv_check_gl_link_with_x="${ax_check_gl_link_implicitly_with_x}"])
                               AS_IF([test "X${ax_cv_check_gl_link_with_x}" = "X${ax_check_gl_need_x}"],
                                     [no_gl="no"],
                                     [no_gl=yes])])
	     ])
])

# ax_check_gl entry point
AC_DEFUN([AX_CHECK_GL],
[AC_REQUIRE([AC_PATH_X])dnl
 AC_REQUIRE([AC_CANONICAL_HOST])

 AC_ARG_WITH([gl],
  [AS_HELP_STRING([--with-gl@<:@=ARG@:>@],
    [use opengl (ARG=yes),
     using the specific lib (ARG=<lib>),
     using the OpenGL lib that link with X (ARG=x),
     using the OpenGL lib that link without X (ARG=nox),
     or disable it (ARG=no)
     @<:@ARG=yes@:>@ ])],
    [
    AS_CASE(["$withval"],
            ["no"|"NO"],[ax_check_gl_want_gl="no"],
	    ["yes"|"YES"],[ax_check_gl_want_gl="yes"],
	    [ax_check_gl_want_gl="$withval"])
    ],
    [ax_check_gl_want_gl="yes"])

 dnl compatibility with AX_HAVE_OPENGL
 AC_ARG_WITH([Mesa],
    [AS_HELP_STRING([--with-Mesa@<:@=ARG@:>@],
    [Prefer the Mesa library over a vendors native OpenGL (ARG=yes except on mingw ARG=no),
     @<:@ARG=yes@:>@ ])],
    [
    AS_CASE(["$withval"],
            ["no"|"NO"],[ax_check_gl_want_mesa="no"],
	    ["yes"|"YES"],[ax_check_gl_want_mesa="yes"],
	    [AC_MSG_ERROR([--with-mesa flag is only yes no])])
    ],
    [ax_check_gl_want_mesa="default"])

 # check consistency of parameters
 AS_IF([test "X$have_x" = "Xdisabled"],
       [AS_IF([test X$ax_check_gl_want_gl = "Xx"],
              [AC_MSG_ERROR([You prefer OpenGL with X and asked for no X support])])])

 AS_IF([test "X$have_x" = "Xdisabled"],
       [AS_IF([test X$x_check_gl_want_mesa = "Xyes"],
              [AC_MSG_WARN([You prefer mesa but you disable X. Disable mesa because mesa need X])
	       ax_check_gl_want_mesa="no"])])

 # mesa default means yes except on mingw
 AC_MSG_CHECKING([wether we should prefer mesa for opengl implementation])
 AS_IF([test X$ax_check_gl_want_mesa = "Xdefault"],
       [AS_CASE([${host}],
                [*-mingw*],[ax_check_gl_want_mesa=no],
		[ax_check_gl_want_mesa=yes])])
 AC_MSG_RESULT($ax_check_gl_want_mesa)

 # set default guess order
 AC_MSG_CHECKING([for a working OpenGL order detection])
 AS_IF([test "X$no_x" = "Xyes"],
       [ax_check_gl_order="gl"],
       [AS_IF([test X$ax_check_gl_want_mesa = "Xyes"],
              [ax_check_gl_order="mesagl gl"],
	      [ax_check_gl_order="gl mesagl"])])
 AC_MSG_RESULT($ax_check_gl_order)

 # set flags
 no_gl="yes"
 have_GL="no"

 # now do the real testing
 AS_IF([test X$ax_check_gl_want_gl != "Xno"],
       [_AX_CHECK_GL()])

 AC_MSG_CHECKING([for a working OpenGL implementation])
 AS_IF([test "X$no_gl" = "Xno"],
       [have_GL="yes"
        AC_MSG_RESULT([yes])
        AC_MSG_CHECKING([for CFLAGS needed for OpenGL])
        AC_MSG_RESULT(["${GL_CFLAGS}"])
        AC_MSG_CHECKING([for LIBS needed for OpenGL])
        AC_MSG_RESULT(["${GL_LIBS}"])
        AC_MSG_CHECKING([for LDFLAGS needed for OpenGL])
        AC_MSG_RESULT(["${GL_LDFLAGS}"])],
       [AC_MSG_RESULT([no])
        GL_CFLAGS=""
        GL_LIBS=""
        GL_LDFLAGS=""])

 AC_SUBST([GL_CFLAGS])
 AC_SUBST([GL_LIBS])
 AC_SUBST([GL_LDFLAGS])
])

# ===========================================================================
#       http://www.gnu.org/software/autoconf-archive/ax_check_glu.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_CHECK_GLU
#
# DESCRIPTION
#
#   Check for GLU. If GLU is found, the required preprocessor and linker
#   flags are included in the output variables "GLU_CFLAGS" and "GLU_LIBS",
#   respectively. If no GLU implementation is found, "no_glu" is set to
#   "yes".
#
#   If the header "GL/glu.h" is found, "HAVE_GL_GLU_H" is defined. If the
#   header "OpenGL/glu.h" is found, HAVE_OPENGL_GLU_H is defined. These
#   preprocessor definitions may not be mutually exclusive.
#
#   You should use something like this in your headers:
#
#     # if defined(HAVE_WINDOWS_H) && defined(_WIN32)
#     #  include <windows.h>
#     # endif
#     # if defined(HAVE_GL_GLU_H)
#     #  include <GL/glu.h>
#     # elif defined(HAVE_OPENGL_GLU_H)
#     #  include <OpenGL/glu.h>
#     # else
#     #  error no glu.h
#     # endif
#
#   Some implementations (in particular, some versions of Mac OS X) are
#   known to treat the GLU tesselator callback function type as "GLvoid
#   (*)(...)" rather than the standard "GLvoid (*)()". If the former
#   condition is detected, this macro defines "HAVE_VARARGS_GLU_TESSCB".
#
# LICENSE
#
#   Copyright (c) 2009 Braden McDaniel <braden@endoframe.com>
#   Copyright (c) 2013 Bastien Roucaries <roucaries.bastien+autoconf@gmail.com>
#
#   This program is free software; you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation; either version 2 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.

#serial 18

# exemple program
m4_define([_AX_CHECK_GLU_PROGRAM],
          [AC_LANG_PROGRAM([[
# if defined(HAVE_WINDOWS_H) && defined(_WIN32)
#   include <windows.h>
# endif
# ifdef HAVE_GL_GLU_H
#   include <GL/glu.h>
# elif defined(HAVE_OPENGL_GLU_H)
#   include <OpenGL/glu.h>
# else
#   error no glu.h
# endif
]],[[gluBeginCurve(0)]])])


dnl Default include : add windows.h
dnl see http://www.opengl.org/wiki/Platform_specifics:_Windows
dnl (acceded 20120801)
AC_DEFUN([_AX_CHECK_GLU_INCLUDES_DEFAULT],dnl
[
  AC_INCLUDES_DEFAULT
  [
  # if defined(HAVE_WINDOWS_H) && defined(_WIN32)
  #   include <windows.h>
  # endif
  ]
])

dnl local save flags
AC_DEFUN([_AX_CHECK_GLU_SAVE_FLAGS],
[dnl
ax_check_glu_saved_libs="${LIBS}"
ax_check_glu_saved_cflags="${CFLAGS}"
ax_check_glu_saved_cppflags="${CPPFLAGS}"
ax_check_glu_saved_ldflags="${LDFLAGS}"
])


dnl local restore flags
AC_DEFUN([_AX_CHECK_GLU_RESTORE_FLAGS],
[dnl
LIBS="${ax_check_glu_saved_libs}"
CFLAGS="${ax_check_glu_saved_cflags}"
CPPFLAGS="${ax_check_glu_saved_cppflags}"
LDFLAGS="${ax_check_glu_saved_ldflags}"
])


# compile the example program
AC_DEFUN([_AX_CHECK_GLU_COMPILE],
[dnl
 AC_LANG_PUSH([C])
 _AX_CHECK_GLU_SAVE_FLAGS()
 CFLAGS="${GLU_CFLAGS} ${CFLAGS}"
 AC_COMPILE_IFELSE([_AX_CHECK_GLU_PROGRAM],
                   [ax_check_glu_compile_opengl="yes"],
                   [ax_check_glu_compile_opengl="no"])
 _AX_CHECK_GLU_RESTORE_FLAGS()
 AC_LANG_POP([C])
])

# compile the example program (cache)
AC_DEFUN([_AX_CHECK_GLU_COMPILE_CV],
[dnl
 AC_CACHE_CHECK([for compiling a minimal OpenGL Utility (GLU) program],[ax_cv_check_glu_compile_opengl],
                [_AX_CHECK_GLU_COMPILE()
                 ax_cv_check_glu_compile_opengl="${ax_check_glu_compile_opengl}"])
 ax_check_glu_compile_opengl="${ax_cv_check_glu_compile_opengl}"
])

# link the example program
AC_DEFUN([_AX_CHECK_GLU_LINK],
[dnl
 AC_LANG_PUSH([C])
 _AX_CHECK_GLU_SAVE_FLAGS()
 CFLAGS="${GLU_CFLAGS} ${CFLAGS}"
 LIBS="${GLU_LIBS} ${LIBS}"
 LDFLAGS="${GLU_LDFLAGS} ${LDFLAGS}"
 AC_LINK_IFELSE([_AX_CHECK_GLU_PROGRAM],
                [ax_check_glu_link_opengl="yes"],
                [ax_check_glu_link_opengl="no"])
 _AX_CHECK_GLU_RESTORE_FLAGS()
 AC_LANG_POP([C])
])

# link the example program (cache)
AC_DEFUN([_AX_CHECK_GLU_LINK_CV],
[dnl
 AC_CACHE_CHECK([for linking a minimal OpenGL Utility (GLU) program],[ax_cv_check_glu_link_opengl],
                [_AX_CHECK_GLU_LINK()
                 ax_cv_check_glu_link_opengl="${ax_check_glu_link_opengl}"])
 ax_check_glu_link_opengl="${ax_cv_check_glu_link_opengl}"
])

dnl Check headers manually (default case)
AC_DEFUN([_AX_CHECK_GLU_HEADERS],
[AC_LANG_PUSH([C])
 _AX_CHECK_GLU_SAVE_FLAGS()
 CFLAGS="${GLU_CFLAGS} ${CFLAGS}"
 # see comment in _AX_CHECK_GL_INCLUDES_DEFAULT
 AC_CHECK_HEADERS([windows.h],[],[],[AC_INCLUDES_DEFAULT])
 AC_CHECK_HEADERS([GL/glu.h OpenGL/glu.h],
                         [ax_check_glu_have_headers="yes";break],
                         [ax_check_glu_have_headers_headers="no"],
			 [_AX_CHECK_GLU_INCLUDES_DEFAULT()])
 # do not try darwin specific OpenGl/gl.h
 _AX_CHECK_GLU_RESTORE_FLAGS()
 AC_LANG_POP([C])
])

# check tesselation callback function signature.
m4_define([_AX_CHECK_GLU_VARARGS_TESSVB_PROGRAM],
[AC_LANG_PROGRAM([[
# if defined(HAVE_WINDOWS_H) && defined(_WIN32)
#   include <windows.h>
# endif
# ifdef HAVE_GL_GLU_H
#   include <GL/glu.h>
# elif defined(HAVE_OPENGL_GLU_H)
#   include <OpenGL/glu.h>
# else
#   error no glu.h
# endif
]],
[[GLvoid (*func)(...); gluTessCallback(0, 0, func)]])
])

# compile the tesselation callback function program
# test with c++
AC_DEFUN([_AX_CHECK_GLU_COMPILE_VARARGS_TESSVB_PROGRAM],
[AC_REQUIRE([AC_PROG_CXX])dnl

 AC_LANG_PUSH([C++])
 _AX_CHECK_GLU_SAVE_FLAGS()
 CFLAGS="${GLU_CFLAGS} ${CFLAGS}"
 AC_COMPILE_IFELSE([_AX_CHECK_GLU_VARARGS_TESSVB_PROGRAM],
                   [ax_check_glu_compile_varargs_tessvb_program="yes"],
                   [ax_check_glu_compile_varargs_tessvb_program="no"])
 _AX_CHECK_GLU_RESTORE_FLAGS()
 AC_LANG_POP([C++])
])


#
# Some versions of Mac OS X include a broken interpretation of the GLU
# tesselation callback function signature.
#
AC_DEFUN([_AX_CHECK_GLU_VARARGS_TESSVB],
[
AC_CACHE_CHECK([for varargs OpenGL Utility (GLU) tesselator callback function type],
                [ax_cv_varargs_glu_tesscb],
		[_AX_CHECK_GLU_COMPILE_VARARGS_TESSVB_PROGRAM
		 ax_cv_varargs_glu_tesscb="${ax_check_glu_compile_varargs_tessvb_program}"])
ax_check_glu_compile_varargs_tessvb_program="${ax_cv_varargs_glu_tesscb}"

AS_IF([test X$ax_cv_varargs_glu_tesscb = Xyes],
      [AC_DEFINE([HAVE_VARARGS_GLU_TESSCB], [1],
                 [Use nonstandard varargs form for the GLU tesselator callback])])
])


# dnl try to found library (generic case)
# dnl $1 is set to the library to found
AC_DEFUN([_AX_CHECK_GLU_MANUAL_LIBS_GENERIC],
[dnl
 ax_check_glu_manual_libs_generic_extra_libs="$1"
 AS_IF([test "X$ax_check_glu_manual_libs_generic_extra_libs" = "X"],
       [AC_MSG_ERROR([AX_CHECK_GLU_MANUAL_LIBS_GENERIC argument must no be empty])])

 AC_LANG_PUSH([C])
 _AX_CHECK_GLU_SAVE_FLAGS()
 CFLAGS="${GLU_CFLAGS} ${CFLAGS}"
 LIBS="${GLU_LIBS} ${LIBS}"
 AC_SEARCH_LIBS([gluBeginCurve],[$ax_check_glu_manual_libs_generic_extra_libs],
                [ax_check_glu_lib_opengl="yes"],
                [ax_check_glu_lib_opengl="no"])
 AS_CASE([$ac_cv_search_gluBeginCurve],
         ["none required"],[],
 	 [no],[],
 	 [GLU_LIBS="${ac_cv_search_gluBeginCurve} ${GLU_LIBS}"])
  _AX_CHECK_GLU_RESTORE_FLAGS()
  AC_LANG_PUSH([C])
])


dnl Check library manually: subroutine must set
dnl $ax_check_gl_lib_opengl={yes,no}
AC_DEFUN([_AX_CHECK_GLU_MANUAL_LIBS],
[AC_REQUIRE([AC_CANONICAL_HOST])
 GLU_LIBS="${GLU_LIBS} ${GL_LIBS}"
 AS_CASE([${host}],
         # try first cygwin version
         [*-cygwin*],[_AX_CHECK_GLU_MANUAL_LIBS_GENERIC([GLU glu MesaGLU glu32])],
         # try first native
	 [*-mingw*],[_AX_CHECK_GLU_MANUAL_LIBS_GENERIC([glu32 GLU glu MesaGLU])],
	 [_AX_CHECK_GLU_MANUAL_LIBS_GENERIC([GLU glu MesaGLU])])

 AC_CACHE_CHECK([for OpenGL Utility (GLU) libraries],[ax_cv_check_glu_lib_opengl],
               	[ax_cv_check_glu_lib_opengl="${ax_check_glu_lib_opengl}"])
 ax_check_glu_lib_opengl="${ax_cv_check_glu_lib_opengl}"
])


dnl Manual way to detect GLU
AC_DEFUN([_AX_CHECK_GLU_MANUAL],
[dnl

# inherit cflags
GLU_CFLAGS="${GLU_CFLAGS} ${GL_CFLAGS}"

# check headers
_AX_CHECK_GLU_HEADERS

AS_IF([test "X$ax_check_glu_have_headers" = "Xyes"],
      [_AX_CHECK_GLU_MANUAL_LIBS],
      [ax_check_glu_lib_opengl="no"])

AS_IF([test "X$ax_check_glu_lib_opengl" = "Xyes"],
      [_AX_CHECK_GLU_COMPILE_CV()],
      [ax_cv_check_glu_compile_opengl="no"])

AS_IF([test "X$ax_cv_check_glu_compile_opengl" = "Xyes"],
      [_AX_CHECK_GLU_LINK_CV()],
      [ax_cv_check_glu_link_opengl="no"])

AS_IF([test "X$ax_cv_check_glu_link_opengl" = "Xyes"],
      [no_glu="no"],
      [no_glu="yes"])
])

# detect using pkgconfig
AC_DEFUN([_AX_CHECK_GLU_PKG_CONFIG],
[
 AC_REQUIRE([PKG_PROG_PKG_CONFIG])

 PKG_CHECK_MODULES([GLU],[glu],[ax_check_glu_pkg_config=yes],[ax_check_glu_pkg_config=no])

 AS_IF([test "X$ax_check_glu_pkg_config" = "Xyes"],[
        # check headers
        AC_LANG_PUSH([C])
 	_AX_CHECK_GLU_SAVE_FLAGS()
        CFLAGS="${GLU_CFLAGS} ${CFLAGS}"
        AC_CHECK_HEADERS([windows.h],[],[],[AC_INCLUDES_DEFAULT])
        AC_CHECK_HEADERS([GL/glu.h OpenGL/glu.h],
                         [ax_check_glu_have_headers="yes";break],
                         [ax_check_glu_have_headers_headers="no"],
			 [_AX_CHECK_GLU_INCLUDES_DEFAULT()])
        _AX_CHECK_GLU_RESTORE_FLAGS()
	AC_LANG_POP([C])
	AC_CACHE_CHECK([for OpenGL Utility (GLU) headers],[ax_cv_check_glu_have_headers],
               	       [ax_cv_check_glu_have_headers="${ax_check_glu_have_headers}"])

        # pkgconfig library are suposed to work ...
        AS_IF([test "X$ax_cv_check_glu_have_headers" = "Xno"],
              [AC_MSG_ERROR("Pkgconfig detected OpenGL Utility (GLU) library has no headers!")])

	_AX_CHECK_GLU_COMPILE_CV()
	AS_IF([test "X$ax_cv_check_glu_compile_opengl" = "Xno"],
              [AC_MSG_ERROR("Pkgconfig detected OpenGL Utility (GLU) library could not be used for compiling minimal program!")])

	_AX_CHECK_GLU_LINK_CV()
	AS_IF([test "X$ax_cv_check_glu_link_opengl" = "Xno"],
              [AC_MSG_ERROR("Pkgconfig detected OpenGL Utility (GLU) library could not be used for linking minimal program!")])
  ])
])

# entry point
AC_DEFUN([AX_CHECK_GLU],dnl
[
 AC_REQUIRE([AX_CHECK_GL])
 AC_REQUIRE([PKG_PROG_PKG_CONFIG])

 # set flags
 no_glu="yes"
 have_GLU="no"

 AC_MSG_CHECKING([for a working OpenGL Utility (GLU) implementation by pkg-config])
 # try first pkgconfig
 AS_IF([test "X${PKG_CONFIG}" = "X"],
       [AC_MSG_RESULT([no])
        ax_check_glu_pkg_config=no],
       [AC_MSG_RESULT([yes])
        _AX_CHECK_GLU_PKG_CONFIG()])

 # if no pkg-config or pkg-config fail try manual way
 AS_IF([test "X$ax_check_glu_pkg_config" = "Xno"],
       [_AX_CHECK_GLU_MANUAL()],
       [no_glu=no])

 # check broken implementation
 AS_IF([test "X$no_glu" = "Xno"],
       [_AX_CHECK_GLU_VARARGS_TESSVB],[])

 AC_MSG_CHECKING([for a working OpenGL Utility (GLU) implementation])
 AS_IF([test "X$no_glu" = "Xno"],
       [have_GLU="yes"
        AC_MSG_RESULT([yes])
        AC_MSG_CHECKING([for CFLAGS needed for OpenGL Utility (GLU)])
        AC_MSG_RESULT(["${GLU_CFLAGS}"])
        AC_MSG_CHECKING([for LIBS needed for OpenGL Utility (GLU)])
        AC_MSG_RESULT(["${GLU_LIBS}"])
        AC_MSG_CHECKING([for LDFLAGS needed for OpenGL Utility (GLU)])
        AC_MSG_RESULT(["${GLU_LDFLAGS}"])],
       [AC_MSG_RESULT([no])
        GLU_CFLAGS=""
        GLU_LIBS=""
        GLU_LDFLAGS=""])

 AC_SUBST([GLU_CFLAGS])
 AC_SUBST([GLU_LIBS])
 AC_SUBST([GLU_LDFLAGS])

])

# ===========================================================================
#       http://www.gnu.org/software/autoconf-archive/ax_check_glut.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_CHECK_GLUT
#
# DESCRIPTION
#
#   Check for GLUT. If GLUT is found, the required compiler and linker flags
#   are included in the output variables "GLUT_CFLAGS" and "GLUT_LIBS",
#   respectively. If GLUT is not found, "no_glut" is set to "yes".
#
#   If the header "GL/glut.h" is found, "HAVE_GL_GLUT_H" is defined. If the
#   header "GLUT/glut.h" is found, HAVE_GLUT_GLUT_H is defined. These
#   preprocessor definitions may not be mutually exclusive.
#
#   You should use something like this in your headers:
#
#     # if HAVE_WINDOWS_H && defined(_WIN32)
#     #  include <windows.h>
#     # endif
#     # if defined(HAVE_GL_GLUT_H)
#     #  include <GL/glut.h>
#     # elif defined(HAVE_GLUT_GLUT_H)
#     #  include <GLUT/glut.h>
#     # else
#     #  error no glut.h
#     # endif
#
# LICENSE
#
#   Copyright (c) 2009 Braden McDaniel <braden@endoframe.com>
#   Copyright (c) 2013 Bastien Roucaries <roucaries.bastien+autoconf@gmail.com>
#
#   This program is free software; you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation; either version 2 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.

#serial 14

dnl local save flags
AC_DEFUN([_AX_CHECK_GLUT_SAVE_FLAGS],
[dnl
ax_check_glut_saved_libs="${LIBS}"
ax_check_glut_saved_cflags="${CFLAGS}"
ax_check_glut_saved_cppflags="${CPPFLAGS}"
ax_check_glut_saved_ldflags="${LDFLAGS}"
])


dnl local restore flags
AC_DEFUN([_AX_CHECK_GLUT_RESTORE_FLAGS],
[dnl
LIBS="${ax_check_glut_saved_libs}"
CFLAGS="${ax_check_glut_saved_cflags}"
CPPFLAGS="${ax_check_glut_saved_cppflags}"
LDFLAGS="${ax_check_glut_saved_ldflags}"
])

dnl Default include : add windows.h
dnl see http://www.opengl.org/wiki/Platform_specifics:_Windows
dnl (acceded 20120801)
AC_DEFUN([_AX_CHECK_GLUT_INCLUDES_DEFAULT],dnl
[
  AC_INCLUDES_DEFAULT
  [
  # if defined(HAVE_WINDOWS_H) && defined(_WIN32)
  #   include <windows.h>
  # endif
  ]
])

m4_define([_AX_CHECK_GLUT_PROGRAM],
          [AC_LANG_PROGRAM([[
# if HAVE_WINDOWS_H && defined(_WIN32)
#   include <windows.h>
# endif
# ifdef HAVE_GL_GLUT_H
#   include <GL/glut.h>
# elif defined(HAVE_GLUT_GLUT_H)
#   include <GLUT/glut.h>
# else
#   error no glut.h
# endif]],
[[glutMainLoop()]])])


dnl Check headers manually (default case)
AC_DEFUN([_AX_CHECK_GLUT_HEADERS],
[AC_LANG_PUSH([C])
 _AX_CHECK_GLUT_SAVE_FLAGS()
 CFLAGS="${GLUT_CFLAGS} ${CFLAGS}"
 # see comment in _AX_CHECK_GL_INCLUDES_DEFAULT
 AC_CHECK_HEADERS([windows.h],[],[],[AC_INCLUDES_DEFAULT])
 AC_CHECK_HEADERS([GL/glut.h OpenGL/glut.h],
                         [ax_check_glut_have_headers="yes";break],
                         [ax_check_glut_have_headers_headers="no"],
			 [_AX_CHECK_GLUT_INCLUDES_DEFAULT()])
 # do not try darwin specific OpenGl/gl.h
 _AX_CHECK_GLUT_RESTORE_FLAGS()
 AC_LANG_POP([C])
])

# dnl try to found library (generic case)
# dnl $1 is set to the library to found
AC_DEFUN([_AX_CHECK_GLUT_MANUAL_LIBS_GENERIC],
[dnl
 ax_check_glut_manual_libs_generic_extra_libs="$1"
 AS_IF([test "X$ax_check_glut_manual_libs_generic_extra_libs" = "X"],
       [AC_MSG_ERROR([AX_CHECK_GLUT_MANUAL_LIBS_GENERIC argument must no be empty])])

 AC_LANG_PUSH([C])
 _AX_CHECK_GLUT_SAVE_FLAGS()
 CFLAGS="${GLUT_CFLAGS} ${CFLAGS}"
 LIBS="${GLUT_LIBS} ${LIBS}"
 AC_SEARCH_LIBS([glutMainLoop],[$ax_check_glut_manual_libs_generic_extra_libs],
                [ax_check_glut_lib_opengl="yes"],
                [ax_check_glut_lib_opengl="no"])
 AS_CASE([$ac_cv_search_glutMainLoop],
         ["none required"],[],
 	 [no],[],
 	 [GLUT_LIBS="${ac_cv_search_glutMainLoop} ${GLU_LIBS}"])
  _AX_CHECK_GLUT_RESTORE_FLAGS()
  AC_LANG_PUSH([C])
])


dnl Check library manually: subroutine must set
dnl $ax_check_glut_lib_opengl={yes,no}
dnl for windows part see
dnl   - http://www.transmissionzero.co.uk/computing/using-glut-with-mingw/
dnl   - http://user.xmission.com/~nate/glut.html
AC_DEFUN([_AX_CHECK_GLUT_MANUAL_LIBS],
[AC_REQUIRE([AC_CANONICAL_HOST])
 GLUT_LIBS="${GLUT_LIBS} ${GLU_LIBS}"
 AS_CASE([${host}],
         # try first cygwin version
         [*-cygwin*],[_AX_CHECK_GLUT_MANUAL_LIBS_GENERIC([GLUT glut MesaGLUT freeglut freeglut32 glut32])],
         # try first native
	 [*-mingw*],[_AX_CHECK_GLUT_MANUAL_LIBS_GENERIC([glut32 GLUT glut MesaGLUT freeglut freeglut32])],
	 [_AX_CHECK_GLUT_MANUAL_LIBS_GENERIC([GLUT glut freeglut MesaGLUT])])

 AC_CACHE_CHECK([for OpenGL Utility Toolkit (GLUT) libraries],[ax_cv_check_glut_lib_opengl],
               	[ax_cv_check_glut_lib_opengl="${ax_check_glut_lib_opengl}"])
 ax_check_glut_lib_opengl="${ax_cv_check_glut_lib_opengl}"
])

# compile the example program
AC_DEFUN([_AX_CHECK_GLUT_COMPILE],
[dnl
 AC_LANG_PUSH([C])
 _AX_CHECK_GLUT_SAVE_FLAGS()
 CFLAGS="${GLUT_CFLAGS} ${CFLAGS}"
 AC_COMPILE_IFELSE([_AX_CHECK_GLUT_PROGRAM],
                   [ax_check_glut_compile_opengl="yes"],
                   [ax_check_glut_compile_opengl="no"])
 _AX_CHECK_GLUT_RESTORE_FLAGS()
 AC_LANG_POP([C])
])

# compile the example program (cache)
AC_DEFUN([_AX_CHECK_GLUT_COMPILE_CV],
[dnl
 AC_CACHE_CHECK([for compiling a minimal OpenGL Utility Toolkit (GLUT) program],[ax_cv_check_glut_compile_opengl],
                [_AX_CHECK_GLUT_COMPILE()
                 ax_cv_check_glut_compile_opengl="${ax_check_glut_compile_opengl}"])
 ax_check_glut_compile_opengl="${ax_cv_check_glut_compile_opengl}"
])

# link the example program
AC_DEFUN([_AX_CHECK_GLUT_LINK],
[dnl
 AC_LANG_PUSH([C])
 _AX_CHECK_GLUT_SAVE_FLAGS()
 CFLAGS="${GLUT_CFLAGS} ${CFLAGS}"
 LIBS="${GLUT_LIBS} ${LIBS}"
 LDFLAGS="${GLUT_LDFLAGS} ${LDFLAGS}"
 AC_LINK_IFELSE([_AX_CHECK_GLUT_PROGRAM],
                [ax_check_glut_link_opengl="yes"],
                [ax_check_glut_link_opengl="no"])
 _AX_CHECK_GLUT_RESTORE_FLAGS()
 AC_LANG_POP([C])
])

# link the example program (cache)
AC_DEFUN([_AX_CHECK_GLUT_LINK_CV],
[dnl
 AC_CACHE_CHECK([for linking a minimal OpenGL Utility Toolkit (GLUT) program],[ax_cv_check_glut_link_opengl],
                [_AX_CHECK_GLUT_LINK()
                 ax_cv_check_glut_link_opengl="${ax_check_glut_link_opengl}"])
 ax_check_glut_link_opengl="${ax_cv_check_glut_link_opengl}"
])


# manually check GLUT
AC_DEFUN([_AX_CHECK_GLUT_MANUAL],dnl
[
GLUT_CFLAGS="${GLUT_CFLAGS} ${GLU_CFLAGS}"
_AX_CHECK_GLUT_HEADERS

AS_IF([test "X$ax_check_glut_have_headers" = "Xyes"],
      [_AX_CHECK_GLUT_MANUAL_LIBS],
      [ax_check_glut_lib="no"])

AS_IF([test "X$ax_check_glut_lib_opengl" = "Xyes"],
      [_AX_CHECK_GLUT_COMPILE_CV()],
      [ax_cv_check_glut_compile_opengl="no"])

AS_IF([test "X$ax_cv_check_glut_compile_opengl" = "Xyes"],
      [_AX_CHECK_GLUT_LINK_CV()],
      [ax_cv_check_glut_link_opengl="no"])

AS_IF([test "X$ax_cv_check_glut_link_opengl" = "Xyes"],
       [no_glut="no"],
       [no_glut="yes"])
])


# main entry point
AC_DEFUN([AX_CHECK_GLUT],
[dnl
 AC_REQUIRE([AX_CHECK_GL])dnl
 AC_REQUIRE([AX_CHECK_GLU])dnl

 # set flags
 no_glut="yes"
 have_GLUT="no"
 have_glut="no"

 _AX_CHECK_GLUT_MANUAL

 AC_MSG_CHECKING([for a working OpenGL Utility Toolkit (GLUT) implementation])
 AS_IF([test "X$no_glut" = "Xno"],
       [have_GLUT="yes"
        have_glut="yes"
        AC_MSG_RESULT([yes])
        AC_MSG_CHECKING([for CFLAGS needed for OpenGL Utility Toolkit (GLUT)])
        AC_MSG_RESULT(["${GLUT_CFLAGS}"])
        AC_MSG_CHECKING([for LIBS needed for OpenGL Utility Toolkit (GLUT)])
        AC_MSG_RESULT(["${GLUT_LIBS}"])
        AC_MSG_CHECKING([for LDFLAGS needed for OpenGL Utility Toolkit (GLUT)])
        AC_MSG_RESULT(["${GLUT_LDFLAGS}"])],
       [AC_MSG_RESULT([no])
        GLUT_CFLAGS=""
        GLUT_LIBS=""
        GLUT_LDFLAGS=""])

 AC_SUBST([GLUT_CFLAGS])
 AC_SUBST([GLUT_LIBS])
 AC_SUBST([GLUT_LDFLAGS])
])

# ===========================================================================
#         http://www.gnu.org/software/autoconf-archive/ax_openmp.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_OPENMP([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
#
# DESCRIPTION
#
#   This macro tries to find out how to compile programs that use OpenMP a
#   standard API and set of compiler directives for parallel programming
#   (see http://www-unix.mcs/)
#
#   On success, it sets the OPENMP_CFLAGS/OPENMP_CXXFLAGS/OPENMP_F77FLAGS
#   output variable to the flag (e.g. -omp) used both to compile *and* link
#   OpenMP programs in the current language.
#
#   NOTE: You are assumed to not only compile your program with these flags,
#   but also link it with them as well.
#
#   If you want to compile everything with OpenMP, you should set:
#
#     CFLAGS="$CFLAGS $OPENMP_CFLAGS"
#     #OR#  CXXFLAGS="$CXXFLAGS $OPENMP_CXXFLAGS"
#     #OR#  FFLAGS="$FFLAGS $OPENMP_FFLAGS"
#
#   (depending on the selected language).
#
#   The user can override the default choice by setting the corresponding
#   environment variable (e.g. OPENMP_CFLAGS).
#
#   ACTION-IF-FOUND is a list of shell commands to run if an OpenMP flag is
#   found, and ACTION-IF-NOT-FOUND is a list of commands to run it if it is
#   not found. If ACTION-IF-FOUND is not specified, the default action will
#   define HAVE_OPENMP.
#
# LICENSE
#
#   Copyright (c) 2008 Steven G. Johnson <stevenj@alum.mit.edu>
#   Copyright (c) 2015 John W. Peterson <jwpeterson@gmail.com>
#
#   This program is free software: you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation, either version 3 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.

#serial 11

AC_DEFUN([AX_OPENMP], [
AC_PREREQ([2.69]) dnl for _AC_LANG_PREFIX

AC_CACHE_CHECK([for OpenMP flag of _AC_LANG compiler], ax_cv_[]_AC_LANG_ABBREV[]_openmp, [save[]_AC_LANG_PREFIX[]FLAGS=$[]_AC_LANG_PREFIX[]FLAGS
ax_cv_[]_AC_LANG_ABBREV[]_openmp=unknown
# Flags to try:  -fopenmp (gcc), -openmp (icc), -mp (SGI & PGI),
#                -xopenmp (Sun), -omp (Tru64), -qsmp=omp (AIX), none
ax_openmp_flags="-fopenmp -openmp -mp -xopenmp -omp -qsmp=omp none"
if test "x$OPENMP_[]_AC_LANG_PREFIX[]FLAGS" != x; then
  ax_openmp_flags="$OPENMP_[]_AC_LANG_PREFIX[]FLAGS $ax_openmp_flags"
fi
for ax_openmp_flag in $ax_openmp_flags; do
  case $ax_openmp_flag in
    none) []_AC_LANG_PREFIX[]FLAGS=$save[]_AC_LANG_PREFIX[] ;;
    *) []_AC_LANG_PREFIX[]FLAGS="$save[]_AC_LANG_PREFIX[]FLAGS $ax_openmp_flag" ;;
  esac
  AC_LINK_IFELSE([AC_LANG_SOURCE([[
@%:@include <omp.h>

static void
parallel_fill(int * data, int n)
{
  int i;
@%:@pragma omp parallel for
  for (i = 0; i < n; ++i)
    data[i] = i;
}

int
main()
{
  int arr[100000];
  omp_set_num_threads(2);
  parallel_fill(arr, 100000);
  return 0;
}
]])],[ax_cv_[]_AC_LANG_ABBREV[]_openmp=$ax_openmp_flag; break],[])
done
[]_AC_LANG_PREFIX[]FLAGS=$save[]_AC_LANG_PREFIX[]FLAGS
])
if test "x$ax_cv_[]_AC_LANG_ABBREV[]_openmp" = "xunknown"; then
  m4_default([$2],:)
else
  if test "x$ax_cv_[]_AC_LANG_ABBREV[]_openmp" != "xnone"; then
    OPENMP_[]_AC_LANG_PREFIX[]FLAGS=$ax_cv_[]_AC_LANG_ABBREV[]_openmp
  fi
  m4_default([$1], [AC_DEFINE(HAVE_OPENMP,1,[Define if OpenMP is enabled])])
fi
])dnl AX_OPENMP

dnl pkg.m4 - Macros to locate and utilise pkg-config.   -*- Autoconf -*-
dnl serial 11 (pkg-config-0.29.1)
dnl
dnl Copyright © 2004 Scott James Remnant <scott@netsplit.com>.
dnl Copyright © 2012-2015 Dan Nicholson <dbn.lists@gmail.com>
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
dnl General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
dnl 02111-1307, USA.
dnl
dnl As a special exception to the GNU General Public License, if you
dnl distribute this file as part of a program that contains a
dnl configuration script generated by Autoconf, you may include it under
dnl the same distribution terms that you use for the rest of that
dnl program.

dnl PKG_PREREQ(MIN-VERSION)
dnl -----------------------
dnl Since: 0.29
dnl
dnl Verify that the version of the pkg-config macros are at least
dnl MIN-VERSION. Unlike PKG_PROG_PKG_CONFIG, which checks the user's
dnl installed version of pkg-config, this checks the developer's version
dnl of pkg.m4 when generating configure.
dnl
dnl To ensure that this macro is defined, also add:
dnl m4_ifndef([PKG_PREREQ],
dnl     [m4_fatal([must install pkg-config 0.29 or later before running autoconf/autogen])])
dnl
dnl See the "Since" comment for each macro you use to see what version
dnl of the macros you require.
m4_defun([PKG_PREREQ],
[m4_define([PKG_MACROS_VERSION], [0.29.1])
m4_if(m4_version_compare(PKG_MACROS_VERSION, [$1]), -1,
    [m4_fatal([pkg.m4 version $1 or higher is required but ]PKG_MACROS_VERSION[ found])])
])dnl PKG_PREREQ

dnl PKG_PROG_PKG_CONFIG([MIN-VERSION])
dnl ----------------------------------
dnl Since: 0.16
dnl
dnl Search for the pkg-config tool and set the PKG_CONFIG variable to
dnl first found in the path. Checks that the version of pkg-config found
dnl is at least MIN-VERSION. If MIN-VERSION is not specified, 0.9.0 is
dnl used since that's the first version where most current features of
dnl pkg-config existed.
AC_DEFUN([PKG_PROG_PKG_CONFIG],
[m4_pattern_forbid([^_?PKG_[A-Z_]+$])
m4_pattern_allow([^PKG_CONFIG(_(PATH|LIBDIR|SYSROOT_DIR|ALLOW_SYSTEM_(CFLAGS|LIBS)))?$])
m4_pattern_allow([^PKG_CONFIG_(DISABLE_UNINSTALLED|TOP_BUILD_DIR|DEBUG_SPEW)$])
AC_ARG_VAR([PKG_CONFIG], [path to pkg-config utility])
AC_ARG_VAR([PKG_CONFIG_PATH], [directories to add to pkg-config's search path])
AC_ARG_VAR([PKG_CONFIG_LIBDIR], [path overriding pkg-config's built-in search path])

if test "x$ac_cv_env_PKG_CONFIG_set" != "xset"; then
	AC_PATH_TOOL([PKG_CONFIG], [pkg-config])
fi
if test -n "$PKG_CONFIG"; then
	_pkg_min_version=m4_default([$1], [0.9.0])
	AC_MSG_CHECKING([pkg-config is at least version $_pkg_min_version])
	if $PKG_CONFIG --atleast-pkgconfig-version $_pkg_min_version; then
		AC_MSG_RESULT([yes])
	else
		AC_MSG_RESULT([no])
		PKG_CONFIG=""
	fi
fi[]dnl
])dnl PKG_PROG_PKG_CONFIG

dnl PKG_CHECK_EXISTS(MODULES, [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl -------------------------------------------------------------------
dnl Since: 0.18
dnl
dnl Check to see whether a particular set of modules exists. Similar to
dnl PKG_CHECK_MODULES(), but does not set variables or print errors.
dnl
dnl Please remember that m4 expands AC_REQUIRE([PKG_PROG_PKG_CONFIG])
dnl only at the first occurence in configure.ac, so if the first place
dnl it's called might be skipped (such as if it is within an "if", you
dnl have to call PKG_CHECK_EXISTS manually
AC_DEFUN([PKG_CHECK_EXISTS],
[AC_REQUIRE([PKG_PROG_PKG_CONFIG])dnl
if test -n "$PKG_CONFIG" && \
    AC_RUN_LOG([$PKG_CONFIG --exists --print-errors "$1"]); then
  m4_default([$2], [:])
m4_ifvaln([$3], [else
  $3])dnl
fi])

dnl _PKG_CONFIG([VARIABLE], [COMMAND], [MODULES])
dnl ---------------------------------------------
dnl Internal wrapper calling pkg-config via PKG_CONFIG and setting
dnl pkg_failed based on the result.
m4_define([_PKG_CONFIG],
[if test -n "$$1"; then
    pkg_cv_[]$1="$$1"
 elif test -n "$PKG_CONFIG"; then
    PKG_CHECK_EXISTS([$3],
                     [pkg_cv_[]$1=`$PKG_CONFIG --[]$2 "$3" 2>/dev/null`
		      test "x$?" != "x0" && pkg_failed=yes ],
		     [pkg_failed=yes])
 else
    pkg_failed=untried
fi[]dnl
])dnl _PKG_CONFIG

dnl _PKG_SHORT_ERRORS_SUPPORTED
dnl ---------------------------
dnl Internal check to see if pkg-config supports short errors.
AC_DEFUN([_PKG_SHORT_ERRORS_SUPPORTED],
[AC_REQUIRE([PKG_PROG_PKG_CONFIG])
if $PKG_CONFIG --atleast-pkgconfig-version 0.20; then
        _pkg_short_errors_supported=yes
else
        _pkg_short_errors_supported=no
fi[]dnl
])dnl _PKG_SHORT_ERRORS_SUPPORTED


dnl PKG_CHECK_MODULES(VARIABLE-PREFIX, MODULES, [ACTION-IF-FOUND],
dnl   [ACTION-IF-NOT-FOUND])
dnl --------------------------------------------------------------
dnl Since: 0.4.0
dnl
dnl Note that if there is a possibility the first call to
dnl PKG_CHECK_MODULES might not happen, you should be sure to include an
dnl explicit call to PKG_PROG_PKG_CONFIG in your configure.ac
AC_DEFUN([PKG_CHECK_MODULES],
[AC_REQUIRE([PKG_PROG_PKG_CONFIG])dnl
AC_ARG_VAR([$1][_CFLAGS], [C compiler flags for $1, overriding pkg-config])dnl
AC_ARG_VAR([$1][_LIBS], [linker flags for $1, overriding pkg-config])dnl

pkg_failed=no
AC_MSG_CHECKING([for $1])

_PKG_CONFIG([$1][_CFLAGS], [cflags], [$2])
_PKG_CONFIG([$1][_LIBS], [libs], [$2])

m4_define([_PKG_TEXT], [Alternatively, you may set the environment variables $1[]_CFLAGS
and $1[]_LIBS to avoid the need to call pkg-config.
See the pkg-config man page for more details.])

if test $pkg_failed = yes; then
   	AC_MSG_RESULT([no])
        _PKG_SHORT_ERRORS_SUPPORTED
        if test $_pkg_short_errors_supported = yes; then
	        $1[]_PKG_ERRORS=`$PKG_CONFIG --short-errors --print-errors --cflags --libs "$2" 2>&1`
        else 
	        $1[]_PKG_ERRORS=`$PKG_CONFIG --print-errors --cflags --libs "$2" 2>&1`
        fi
	# Put the nasty error message in config.log where it belongs
	echo "$$1[]_PKG_ERRORS" >&AS_MESSAGE_LOG_FD

	m4_default([$4], [AC_MSG_ERROR(
[Package requirements ($2) were not met:

$$1_PKG_ERRORS

Consider adjusting the PKG_CONFIG_PATH environment variable if you
installed software in a non-standard prefix.

_PKG_TEXT])[]dnl
        ])
elif test $pkg_failed = untried; then
     	AC_MSG_RESULT([no])
	m4_default([$4], [AC_MSG_FAILURE(
[The pkg-config script could not be found or is too old.  Make sure it
is in your PATH or set the PKG_CONFIG environment variable to the full
path to pkg-config.

_PKG_TEXT

To get pkg-config, see <http://pkg-config.freedesktop.org/>.])[]dnl
        ])
else
	$1[]_CFLAGS=$pkg_cv_[]$1[]_CFLAGS
	$1[]_LIBS=$pkg_cv_[]$1[]_LIBS
        AC_MSG_RESULT([yes])
	$3
fi[]dnl
])dnl PKG_CHECK_MODULES


dnl PKG_CHECK_MODULES_STATIC(VARIABLE-PREFIX, MODULES, [ACTION-IF-FOUND],
dnl   [ACTION-IF-NOT-FOUND])
dnl ---------------------------------------------------------------------
dnl Since: 0.29
dnl
dnl Checks for existence of MODULES and gathers its build flags with
dnl static libraries enabled. Sets VARIABLE-PREFIX_CFLAGS from --cflags
dnl and VARIABLE-PREFIX_LIBS from --libs.
dnl
dnl Note that if there is a possibility the first call to
dnl PKG_CHECK_MODULES_STATIC might not happen, you should be sure to
dnl include an explicit call to PKG_PROG_PKG_CONFIG in your
dnl configure.ac.
AC_DEFUN([PKG_CHECK_MODULES_STATIC],
[AC_REQUIRE([PKG_PROG_PKG_CONFIG])dnl
_save_PKG_CONFIG=$PKG_CONFIG
PKG_CONFIG="$PKG_CONFIG --static"
PKG_CHECK_MODULES($@)
PKG_CONFIG=$_save_PKG_CONFIG[]dnl
])dnl PKG_CHECK_MODULES_STATIC


dnl PKG_INSTALLDIR([DIRECTORY])
dnl -------------------------
dnl Since: 0.27
dnl
dnl Substitutes the variable pkgconfigdir as the location where a module
dnl should install pkg-config .pc files. By default the directory is
dnl $libdir/pkgconfig, but the default can be changed by passing
dnl DIRECTORY. The user can override through the --with-pkgconfigdir
dnl parameter.
AC_DEFUN([PKG_INSTALLDIR],
[m4_pushdef([pkg_default], [m4_default([$1], ['${libdir}/pkgconfig'])])
m4_pushdef([pkg_description],
    [pkg-config installation directory @<:@]pkg_default[@:>@])
AC_ARG_WITH([pkgconfigdir],
    [AS_HELP_STRING([--with-pkgconfigdir], pkg_description)],,
    [with_pkgconfigdir=]pkg_default)
AC_SUBST([pkgconfigdir], [$with_pkgconfigdir])
m4_popdef([pkg_default])
m4_popdef([pkg_description])
])dnl PKG_INSTALLDIR


dnl PKG_NOARCH_INSTALLDIR([DIRECTORY])
dnl --------------------------------
dnl Since: 0.27
dnl
dnl Substitutes the variable noarch_pkgconfigdir as the location where a
dnl module should install arch-independent pkg-config .pc files. By
dnl default the directory is $datadir/pkgconfig, but the default can be
dnl changed by passing DIRECTORY. The user can override through the
dnl --with-noarch-pkgconfigdir parameter.
AC_DEFUN([PKG_NOARCH_INSTALLDIR],
[m4_pushdef([pkg_default], [m4_default([$1], ['${datadir}/pkgconfig'])])
m4_pushdef([pkg_description],
    [pkg-config arch-independent installation directory @<:@]pkg_default[@:>@])
AC_ARG_WITH([noarch-pkgconfigdir],
    [AS_HELP_STRING([--with-noarch-pkgconfigdir], pkg_description)],,
    [with_noarch_pkgconfigdir=]pkg_default)
AC_SUBST([noarch_pkgconfigdir], [$with_noarch_pkgconfigdir])
m4_popdef([pkg_default])
m4_popdef([pkg_description])
])dnl PKG_NOARCH_INSTALLDIR


dnl PKG_CHECK_VAR(VARIABLE, MODULE, CONFIG-VARIABLE,
dnl [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
dnl -------------------------------------------
dnl Since: 0.28
dnl
dnl Retrieves the value of the pkg-config variable for the given module.
AC_DEFUN([PKG_CHECK_VAR],
[AC_REQUIRE([PKG_PROG_PKG_CONFIG])dnl
AC_ARG_VAR([$1], [value of $3 for $2, overriding pkg-config])dnl

_PKG_CONFIG([$1], [variable="][$3]["], [$2])
AS_VAR_COPY([$1], [pkg_cv_][$1])

AS_VAR_IF([$1], [""], [$5], [$4])dnl
])dnl PKG_CHECK_VAR

# Copyright (C) 2002-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_AUTOMAKE_VERSION(VERSION)
# ----------------------------
# Automake X.Y traces this macro to ensure aclocal.m4 has been
# generated from the m4 files accompanying Automake X.Y.
# (This private macro should not be called outside this file.)
AC_DEFUN([AM_AUTOMAKE_VERSION],
[am__api_version='1.15'
dnl Some users find AM_AUTOMAKE_VERSION and mistake it for a way to
dnl require some minimum version.  Point them to the right macro.
m4_if([$1], [1.15], [],
      [AC_FATAL([Do not call $0, use AM_INIT_AUTOMAKE([$1]).])])dnl
])

# _AM_AUTOCONF_VERSION(VERSION)
# -----------------------------
# aclocal traces this macro to find the Autoconf version.
# This is a private macro too.  Using m4_define simplifies
# the logic in aclocal, which can simply ignore this definition.
m4_define([_AM_AUTOCONF_VERSION], [])

# AM_SET_CURRENT_AUTOMAKE_VERSION
# -------------------------------
# Call AM_AUTOMAKE_VERSION and AM_AUTOMAKE_VERSION so they can be traced.
# This function is AC_REQUIREd by AM_INIT_AUTOMAKE.
AC_DEFUN([AM_SET_CURRENT_AUTOMAKE_VERSION],
[AM_AUTOMAKE_VERSION([1.15])dnl
m4_ifndef([AC_AUTOCONF_VERSION],
  [m4_copy([m4_PACKAGE_VERSION], [AC_AUTOCONF_VERSION])])dnl
_AM_AUTOCONF_VERSION(m4_defn([AC_AUTOCONF_VERSION]))])

# AM_AUX_DIR_EXPAND                                         -*- Autoconf -*-

# Copyright (C) 2001-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# For projects using AC_CONFIG_AUX_DIR([foo]), Autoconf sets
# $ac_aux_dir to '$srcdir/foo'.  In other projects, it is set to
# '$srcdir', '$srcdir/..', or '$srcdir/../..'.
#
# Of course, Automake must honor this variable whenever it calls a
# tool from the auxiliary directory.  The problem is that $srcdir (and
# therefore $ac_aux_dir as well) can be either absolute or relative,
# depending on how configure is run.  This is pretty annoying, since
# it makes $ac_aux_dir quite unusable in subdirectories: in the top
# source directory, any form will work fine, but in subdirectories a
# relative path needs to be adjusted first.
#
# $ac_aux_dir/missing
#    fails when called from a subdirectory if $ac_aux_dir is relative
# $top_srcdir/$ac_aux_dir/missing
#    fails if $ac_aux_dir is absolute,
#    fails when called from a subdirectory in a VPATH build with
#          a relative $ac_aux_dir
#
# The reason of the latter failure is that $top_srcdir and $ac_aux_dir
# are both prefixed by $srcdir.  In an in-source build this is usually
# harmless because $srcdir is '.', but things will broke when you
# start a VPATH build or use an absolute $srcdir.
#
# So we could use something similar to $top_srcdir/$ac_aux_dir/missing,
# iff we strip the leading $srcdir from $ac_aux_dir.  That would be:
#   am_aux_dir='\$(top_srcdir)/'`expr "$ac_aux_dir" : "$srcdir//*\(.*\)"`
# and then we would define $MISSING as
#   MISSING="\${SHELL} $am_aux_dir/missing"
# This will work as long as MISSING is not called from configure, because
# unfortunately $(top_srcdir) has no meaning in configure.
# However there are other variables, like CC, which are often used in
# configure, and could therefore not use this "fixed" $ac_aux_dir.
#
# Another solution, used here, is to always expand $ac_aux_dir to an
# absolute PATH.  The drawback is that using absolute paths prevent a
# configured tree to be moved without reconfiguration.

AC_DEFUN([AM_AUX_DIR_EXPAND],
[AC_REQUIRE([AC_CONFIG_AUX_DIR_DEFAULT])dnl
# Expand $ac_aux_dir to an absolute path.
am_aux_dir=`cd "$ac_aux_dir" && pwd`
])

# AM_CONDITIONAL                                            -*- Autoconf -*-

# Copyright (C) 1997-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_CONDITIONAL(NAME, SHELL-CONDITION)
# -------------------------------------
# Define a conditional.
AC_DEFUN([AM_CONDITIONAL],
[AC_PREREQ([2.52])dnl
 m4_if([$1], [TRUE],  [AC_FATAL([$0: invalid condition: $1])],
       [$1], [FALSE], [AC_FATAL([$0: invalid condition: $1])])dnl
AC_SUBST([$1_TRUE])dnl
AC_SUBST([$1_FALSE])dnl
_AM_SUBST_NOTMAKE([$1_TRUE])dnl
_AM_SUBST_NOTMAKE([$1_FALSE])dnl
m4_define([_AM_COND_VALUE_$1], [$2])dnl
if $2; then
  $1_TRUE=
  $1_FALSE='#'
else
  $1_TRUE='#'
  $1_FALSE=
fi
AC_CONFIG_COMMANDS_PRE(
[if test -z "${$1_TRUE}" && test -z "${$1_FALSE}"; then
  AC_MSG_ERROR([[conditional "$1" was never defined.
Usually this means the macro was only invoked conditionally.]])
fi])])

# Copyright (C) 1999-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.


# There are a few dirty hacks below to avoid letting 'AC_PROG_CC' be
# written in clear, in which case automake, when reading aclocal.m4,
# will think it sees a *use*, and therefore will trigger all it's
# C support machinery.  Also note that it means that autoscan, seeing
# CC etc. in the Makefile, will ask for an AC_PROG_CC use...


# _AM_DEPENDENCIES(NAME)
# ----------------------
# See how the compiler implements dependency checking.
# NAME is "CC", "CXX", "OBJC", "OBJCXX", "UPC", or "GJC".
# We try a few techniques and use that to set a single cache variable.
#
# We don't AC_REQUIRE the corresponding AC_PROG_CC since the latter was
# modified to invoke _AM_DEPENDENCIES(CC); we would have a circular
# dependency, and given that the user is not expected to run this macro,
# just rely on AC_PROG_CC.
AC_DEFUN([_AM_DEPENDENCIES],
[AC_REQUIRE([AM_SET_DEPDIR])dnl
AC_REQUIRE([AM_OUTPUT_DEPENDENCY_COMMANDS])dnl
AC_REQUIRE([AM_MAKE_INCLUDE])dnl
AC_REQUIRE([AM_DEP_TRACK])dnl

m4_if([$1], [CC],   [depcc="$CC"   am_compiler_list=],
      [$1], [CXX],  [depcc="$CXX"  am_compiler_list=],
      [$1], [OBJC], [depcc="$OBJC" am_compiler_list='gcc3 gcc'],
      [$1], [OBJCXX], [depcc="$OBJCXX" am_compiler_list='gcc3 gcc'],
      [$1], [UPC],  [depcc="$UPC"  am_compiler_list=],
      [$1], [GCJ],  [depcc="$GCJ"  am_compiler_list='gcc3 gcc'],
                    [depcc="$$1"   am_compiler_list=])

AC_CACHE_CHECK([dependency style of $depcc],
               [am_cv_$1_dependencies_compiler_type],
[if test -z "$AMDEP_TRUE" && test -f "$am_depcomp"; then
  # We make a subdir and do the tests there.  Otherwise we can end up
  # making bogus files that we don't know about and never remove.  For
  # instance it was reported that on HP-UX the gcc test will end up
  # making a dummy file named 'D' -- because '-MD' means "put the output
  # in D".
  rm -rf conftest.dir
  mkdir conftest.dir
  # Copy depcomp to subdir because otherwise we won't find it if we're
  # using a relative directory.
  cp "$am_depcomp" conftest.dir
  cd conftest.dir
  # We will build objects and dependencies in a subdirectory because
  # it helps to detect inapplicable dependency modes.  For instance
  # both Tru64's cc and ICC support -MD to output dependencies as a
  # side effect of compilation, but ICC will put the dependencies in
  # the current directory while Tru64 will put them in the object
  # directory.
  mkdir sub

  am_cv_$1_dependencies_compiler_type=none
  if test "$am_compiler_list" = ""; then
     am_compiler_list=`sed -n ['s/^#*\([a-zA-Z0-9]*\))$/\1/p'] < ./depcomp`
  fi
  am__universal=false
  m4_case([$1], [CC],
    [case " $depcc " in #(
     *\ -arch\ *\ -arch\ *) am__universal=true ;;
     esac],
    [CXX],
    [case " $depcc " in #(
     *\ -arch\ *\ -arch\ *) am__universal=true ;;
     esac])

  for depmode in $am_compiler_list; do
    # Setup a source with many dependencies, because some compilers
    # like to wrap large dependency lists on column 80 (with \), and
    # we should not choose a depcomp mode which is confused by this.
    #
    # We need to recreate these files for each test, as the compiler may
    # overwrite some of them when testing with obscure command lines.
    # This happens at least with the AIX C compiler.
    : > sub/conftest.c
    for i in 1 2 3 4 5 6; do
      echo '#include "conftst'$i'.h"' >> sub/conftest.c
      # Using ": > sub/conftst$i.h" creates only sub/conftst1.h with
      # Solaris 10 /bin/sh.
      echo '/* dummy */' > sub/conftst$i.h
    done
    echo "${am__include} ${am__quote}sub/conftest.Po${am__quote}" > confmf

    # We check with '-c' and '-o' for the sake of the "dashmstdout"
    # mode.  It turns out that the SunPro C++ compiler does not properly
    # handle '-M -o', and we need to detect this.  Also, some Intel
    # versions had trouble with output in subdirs.
    am__obj=sub/conftest.${OBJEXT-o}
    am__minus_obj="-o $am__obj"
    case $depmode in
    gcc)
      # This depmode causes a compiler race in universal mode.
      test "$am__universal" = false || continue
      ;;
    nosideeffect)
      # After this tag, mechanisms are not by side-effect, so they'll
      # only be used when explicitly requested.
      if test "x$enable_dependency_tracking" = xyes; then
	continue
      else
	break
      fi
      ;;
    msvc7 | msvc7msys | msvisualcpp | msvcmsys)
      # This compiler won't grok '-c -o', but also, the minuso test has
      # not run yet.  These depmodes are late enough in the game, and
      # so weak that their functioning should not be impacted.
      am__obj=conftest.${OBJEXT-o}
      am__minus_obj=
      ;;
    none) break ;;
    esac
    if depmode=$depmode \
       source=sub/conftest.c object=$am__obj \
       depfile=sub/conftest.Po tmpdepfile=sub/conftest.TPo \
       $SHELL ./depcomp $depcc -c $am__minus_obj sub/conftest.c \
         >/dev/null 2>conftest.err &&
       grep sub/conftst1.h sub/conftest.Po > /dev/null 2>&1 &&
       grep sub/conftst6.h sub/conftest.Po > /dev/null 2>&1 &&
       grep $am__obj sub/conftest.Po > /dev/null 2>&1 &&
       ${MAKE-make} -s -f confmf > /dev/null 2>&1; then
      # icc doesn't choke on unknown options, it will just issue warnings
      # or remarks (even with -Werror).  So we grep stderr for any message
      # that says an option was ignored or not supported.
      # When given -MP, icc 7.0 and 7.1 complain thusly:
      #   icc: Command line warning: ignoring option '-M'; no argument required
      # The diagnosis changed in icc 8.0:
      #   icc: Command line remark: option '-MP' not supported
      if (grep 'ignoring option' conftest.err ||
          grep 'not supported' conftest.err) >/dev/null 2>&1; then :; else
        am_cv_$1_dependencies_compiler_type=$depmode
        break
      fi
    fi
  done

  cd ..
  rm -rf conftest.dir
else
  am_cv_$1_dependencies_compiler_type=none
fi
])
AC_SUBST([$1DEPMODE], [depmode=$am_cv_$1_dependencies_compiler_type])
AM_CONDITIONAL([am__fastdep$1], [
  test "x$enable_dependency_tracking" != xno \
  && test "$am_cv_$1_dependencies_compiler_type" = gcc3])
])


# AM_SET_DEPDIR
# -------------
# Choose a directory name for dependency files.
# This macro is AC_REQUIREd in _AM_DEPENDENCIES.
AC_DEFUN([AM_SET_DEPDIR],
[AC_REQUIRE([AM_SET_LEADING_DOT])dnl
AC_SUBST([DEPDIR], ["${am__leading_dot}deps"])dnl
])


# AM_DEP_TRACK
# ------------
AC_DEFUN([AM_DEP_TRACK],
[AC_ARG_ENABLE([dependency-tracking], [dnl
AS_HELP_STRING(
  [--enable-dependency-tracking],
  [do not reject slow dependency extractors])
AS_HELP_STRING(
  [--disable-dependency-tracking],
  [speeds up one-time build])])
if test "x$enable_dependency_tracking" != xno; then
  am_depcomp="$ac_aux_dir/depcomp"
  AMDEPBACKSLASH='\'
  am__nodep='_no'
fi
AM_CONDITIONAL([AMDEP], [test "x$enable_dependency_tracking" != xno])
AC_SUBST([AMDEPBACKSLASH])dnl
_AM_SUBST_NOTMAKE([AMDEPBACKSLASH])dnl
AC_SUBST([am__nodep])dnl
_AM_SUBST_NOTMAKE([am__nodep])dnl
])

# Generate code to set up dependency tracking.              -*- Autoconf -*-

# Copyright (C) 1999-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.


# _AM_OUTPUT_DEPENDENCY_COMMANDS
# ------------------------------
AC_DEFUN([_AM_OUTPUT_DEPENDENCY_COMMANDS],
[{
  # Older Autoconf quotes --file arguments for eval, but not when files
  # are listed without --file.  Let's play safe and only enable the eval
  # if we detect the quoting.
  case $CONFIG_FILES in
  *\'*) eval set x "$CONFIG_FILES" ;;
  *)   set x $CONFIG_FILES ;;
  esac
  shift
  for mf
  do
    # Strip MF so we end up with the name of the file.
    mf=`echo "$mf" | sed -e 's/:.*$//'`
    # Check whether this is an Automake generated Makefile or not.
    # We used to match only the files named 'Makefile.in', but
    # some people rename them; so instead we look at the file content.
    # Grep'ing the first line is not enough: some people post-process
    # each Makefile.in and add a new line on top of each file to say so.
    # Grep'ing the whole file is not good either: AIX grep has a line
    # limit of 2048, but all sed's we know have understand at least 4000.
    if sed -n 's,^#.*generated by automake.*,X,p' "$mf" | grep X >/dev/null 2>&1; then
      dirpart=`AS_DIRNAME("$mf")`
    else
      continue
    fi
    # Extract the definition of DEPDIR, am__include, and am__quote
    # from the Makefile without running 'make'.
    DEPDIR=`sed -n 's/^DEPDIR = //p' < "$mf"`
    test -z "$DEPDIR" && continue
    am__include=`sed -n 's/^am__include = //p' < "$mf"`
    test -z "$am__include" && continue
    am__quote=`sed -n 's/^am__quote = //p' < "$mf"`
    # Find all dependency output files, they are included files with
    # $(DEPDIR) in their names.  We invoke sed twice because it is the
    # simplest approach to changing $(DEPDIR) to its actual value in the
    # expansion.
    for file in `sed -n "
      s/^$am__include $am__quote\(.*(DEPDIR).*\)$am__quote"'$/\1/p' <"$mf" | \
	 sed -e 's/\$(DEPDIR)/'"$DEPDIR"'/g'`; do
      # Make sure the directory exists.
      test -f "$dirpart/$file" && continue
      fdir=`AS_DIRNAME(["$file"])`
      AS_MKDIR_P([$dirpart/$fdir])
      # echo "creating $dirpart/$file"
      echo '# dummy' > "$dirpart/$file"
    done
  done
}
])# _AM_OUTPUT_DEPENDENCY_COMMANDS


# AM_OUTPUT_DEPENDENCY_COMMANDS
# -----------------------------
# This macro should only be invoked once -- use via AC_REQUIRE.
#
# This code is only required when automatic dependency tracking
# is enabled.  FIXME.  This creates each '.P' file that we will
# need in order to bootstrap the dependency handling code.
AC_DEFUN([AM_OUTPUT_DEPENDENCY_COMMANDS],
[AC_CONFIG_COMMANDS([depfiles],
     [test x"$AMDEP_TRUE" != x"" || _AM_OUTPUT_DEPENDENCY_COMMANDS],
     [AMDEP_TRUE="$AMDEP_TRUE" ac_aux_dir="$ac_aux_dir"])
])

# Do all the work for Automake.                             -*- Autoconf -*-

# Copyright (C) 1996-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This macro actually does too much.  Some checks are only needed if
# your package does certain things.  But this isn't really a big deal.

dnl Redefine AC_PROG_CC to automatically invoke _AM_PROG_CC_C_O.
m4_define([AC_PROG_CC],
m4_defn([AC_PROG_CC])
[_AM_PROG_CC_C_O
])

# AM_INIT_AUTOMAKE(PACKAGE, VERSION, [NO-DEFINE])
# AM_INIT_AUTOMAKE([OPTIONS])
# -----------------------------------------------
# The call with PACKAGE and VERSION arguments is the old style
# call (pre autoconf-2.50), which is being phased out.  PACKAGE
# and VERSION should now be passed to AC_INIT and removed from
# the call to AM_INIT_AUTOMAKE.
# We support both call styles for the transition.  After
# the next Automake release, Autoconf can make the AC_INIT
# arguments mandatory, and then we can depend on a new Autoconf
# release and drop the old call support.
AC_DEFUN([AM_INIT_AUTOMAKE],
[AC_PREREQ([2.65])dnl
dnl Autoconf wants to disallow AM_ names.  We explicitly allow
dnl the ones we care about.
m4_pattern_allow([^AM_[A-Z]+FLAGS$])dnl
AC_REQUIRE([AM_SET_CURRENT_AUTOMAKE_VERSION])dnl
AC_REQUIRE([AC_PROG_INSTALL])dnl
if test "`cd $srcdir && pwd`" != "`pwd`"; then
  # Use -I$(srcdir) only when $(srcdir) != ., so that make's output
  # is not polluted with repeated "-I."
  AC_SUBST([am__isrc], [' -I$(srcdir)'])_AM_SUBST_NOTMAKE([am__isrc])dnl
  # test to see if srcdir already configured
  if test -f $srcdir/config.status; then
    AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
  fi
fi

# test whether we have cygpath
if test -z "$CYGPATH_W"; then
  if (cygpath --version) >/dev/null 2>/dev/null; then
    CYGPATH_W='cygpath -w'
  else
    CYGPATH_W=echo
  fi
fi
AC_SUBST([CYGPATH_W])

# Define the identity of the package.
dnl Distinguish between old-style and new-style calls.
m4_ifval([$2],
[AC_DIAGNOSE([obsolete],
             [$0: two- and three-arguments forms are deprecated.])
m4_ifval([$3], [_AM_SET_OPTION([no-define])])dnl
 AC_SUBST([PACKAGE], [$1])dnl
 AC_SUBST([VERSION], [$2])],
[_AM_SET_OPTIONS([$1])dnl
dnl Diagnose old-style AC_INIT with new-style AM_AUTOMAKE_INIT.
m4_if(
  m4_ifdef([AC_PACKAGE_NAME], [ok]):m4_ifdef([AC_PACKAGE_VERSION], [ok]),
  [ok:ok],,
  [m4_fatal([AC_INIT should be called with package and version arguments])])dnl
 AC_SUBST([PACKAGE], ['AC_PACKAGE_TARNAME'])dnl
 AC_SUBST([VERSION], ['AC_PACKAGE_VERSION'])])dnl

_AM_IF_OPTION([no-define],,
[AC_DEFINE_UNQUOTED([PACKAGE], ["$PACKAGE"], [Name of package])
 AC_DEFINE_UNQUOTED([VERSION], ["$VERSION"], [Version number of package])])dnl

# Some tools Automake needs.
AC_REQUIRE([AM_SANITY_CHECK])dnl
AC_REQUIRE([AC_ARG_PROGRAM])dnl
AM_MISSING_PROG([ACLOCAL], [aclocal-${am__api_version}])
AM_MISSING_PROG([AUTOCONF], [autoconf])
AM_MISSING_PROG([AUTOMAKE], [automake-${am__api_version}])
AM_MISSING_PROG([AUTOHEADER], [autoheader])
AM_MISSING_PROG([MAKEINFO], [makeinfo])
AC_REQUIRE([AM_PROG_INSTALL_SH])dnl
AC_REQUIRE([AM_PROG_INSTALL_STRIP])dnl
AC_REQUIRE([AC_PROG_MKDIR_P])dnl
# For better backward compatibility.  To be removed once Automake 1.9.x
# dies out for good.  For more background, see:
# <http://lists.gnu.org/archive/html/automake/2012-07/msg00001.html>
# <http://lists.gnu.org/archive/html/automake/2012-07/msg00014.html>
AC_SUBST([mkdir_p], ['$(MKDIR_P)'])
# We need awk for the "check" target (and possibly the TAP driver).  The
# system "awk" is bad on some platforms.
AC_REQUIRE([AC_PROG_AWK])dnl
AC_REQUIRE([AC_PROG_MAKE_SET])dnl
AC_REQUIRE([AM_SET_LEADING_DOT])dnl
_AM_IF_OPTION([tar-ustar], [_AM_PROG_TAR([ustar])],
	      [_AM_IF_OPTION([tar-pax], [_AM_PROG_TAR([pax])],
			     [_AM_PROG_TAR([v7])])])
_AM_IF_OPTION([no-dependencies],,
[AC_PROVIDE_IFELSE([AC_PROG_CC],
		  [_AM_DEPENDENCIES([CC])],
		  [m4_define([AC_PROG_CC],
			     m4_defn([AC_PROG_CC])[_AM_DEPENDENCIES([CC])])])dnl
AC_PROVIDE_IFELSE([AC_PROG_CXX],
		  [_AM_DEPENDENCIES([CXX])],
		  [m4_define([AC_PROG_CXX],
			     m4_defn([AC_PROG_CXX])[_AM_DEPENDENCIES([CXX])])])dnl
AC_PROVIDE_IFELSE([AC_PROG_OBJC],
		  [_AM_DEPENDENCIES([OBJC])],
		  [m4_define([AC_PROG_OBJC],
			     m4_defn([AC_PROG_OBJC])[_AM_DEPENDENCIES([OBJC])])])dnl
AC_PROVIDE_IFELSE([AC_PROG_OBJCXX],
		  [_AM_DEPENDENCIES([OBJCXX])],
		  [m4_define([AC_PROG_OBJCXX],
			     m4_defn([AC_PROG_OBJCXX])[_AM_DEPENDENCIES([OBJCXX])])])dnl
])
AC_REQUIRE([AM_SILENT_RULES])dnl
dnl The testsuite driver may need to know about EXEEXT, so add the
dnl 'am__EXEEXT' conditional if _AM_COMPILER_EXEEXT was seen.  This
dnl macro is hooked onto _AC_COMPILER_EXEEXT early, see below.
AC_CONFIG_COMMANDS_PRE(dnl
[m4_provide_if([_AM_COMPILER_EXEEXT],
  [AM_CONDITIONAL([am__EXEEXT], [test -n "$EXEEXT"])])])dnl

# POSIX will say in a future version that running "rm -f" with no argument
# is OK; and we want to be able to make that assumption in our Makefile
# recipes.  So use an aggressive probe to check that the usage we want is
# actually supported "in the wild" to an acceptable degree.
# See automake bug#10828.
# To make any issue more visible, cause the running configure to be aborted
# by default if the 'rm' program in use doesn't match our expectations; the
# user can still override this though.
if rm -f && rm -fr && rm -rf; then : OK; else
  cat >&2 <<'END'
Oops!

Your 'rm' program seems unable to run without file operands specified
on the command line, even when the '-f' option is present.  This is contrary
to the behaviour of most rm programs out there, and not conforming with
the upcoming POSIX standard: <http://austingroupbugs.net/view.php?id=542>

Please tell bug-automake@gnu.org about your system, including the value
of your $PATH and any error possibly output before this message.  This
can help us improve future automake versions.

END
  if test x"$ACCEPT_INFERIOR_RM_PROGRAM" = x"yes"; then
    echo 'Configuration will proceed anyway, since you have set the' >&2
    echo 'ACCEPT_INFERIOR_RM_PROGRAM variable to "yes"' >&2
    echo >&2
  else
    cat >&2 <<'END'
Aborting the configuration process, to ensure you take notice of the issue.

You can download and install GNU coreutils to get an 'rm' implementation
that behaves properly: <http://www.gnu.org/software/coreutils/>.

If you want to complete the configuration process using your problematic
'rm' anyway, export the environment variable ACCEPT_INFERIOR_RM_PROGRAM
to "yes", and re-run configure.

END
    AC_MSG_ERROR([Your 'rm' program is bad, sorry.])
  fi
fi
dnl The trailing newline in this macro's definition is deliberate, for
dnl backward compatibility and to allow trailing 'dnl'-style comments
dnl after the AM_INIT_AUTOMAKE invocation. See automake bug#16841.
])

dnl Hook into '_AC_COMPILER_EXEEXT' early to learn its expansion.  Do not
dnl add the conditional right here, as _AC_COMPILER_EXEEXT may be further
dnl mangled by Autoconf and run in a shell conditional statement.
m4_define([_AC_COMPILER_EXEEXT],
m4_defn([_AC_COMPILER_EXEEXT])[m4_provide([_AM_COMPILER_EXEEXT])])

# When config.status generates a header, we must update the stamp-h file.
# This file resides in the same directory as the config header
# that is generated.  The stamp files are numbered to have different names.

# Autoconf calls _AC_AM_CONFIG_HEADER_HOOK (when defined) in the
# loop where config.status creates the headers, so we can generate
# our stamp files there.
AC_DEFUN([_AC_AM_CONFIG_HEADER_HOOK],
[# Compute $1's index in $config_headers.
_am_arg=$1
_am_stamp_count=1
for _am_header in $config_headers :; do
  case $_am_header in
    $_am_arg | $_am_arg:* )
      break ;;
    * )
      _am_stamp_count=`expr $_am_stamp_count + 1` ;;
  esac
done
echo "timestamp for $_am_arg" >`AS_DIRNAME(["$_am_arg"])`/stamp-h[]$_am_stamp_count])

# Copyright (C) 2001-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_PROG_INSTALL_SH
# ------------------
# Define $install_sh.
AC_DEFUN([AM_PROG_INSTALL_SH],
[AC_REQUIRE([AM_AUX_DIR_EXPAND])dnl
if test x"${install_sh+set}" != xset; then
  case $am_aux_dir in
  *\ * | *\	*)
    install_sh="\${SHELL} '$am_aux_dir/install-sh'" ;;
  *)
    install_sh="\${SHELL} $am_aux_dir/install-sh"
  esac
fi
AC_SUBST([install_sh])])

# Copyright (C) 2003-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# Check whether the underlying file-system supports filenames
# with a leading dot.  For instance MS-DOS doesn't.
AC_DEFUN([AM_SET_LEADING_DOT],
[rm -rf .tst 2>/dev/null
mkdir .tst 2>/dev/null
if test -d .tst; then
  am__leading_dot=.
else
  am__leading_dot=_
fi
rmdir .tst 2>/dev/null
AC_SUBST([am__leading_dot])])

# Check to see how 'make' treats includes.	            -*- Autoconf -*-

# Copyright (C) 2001-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_MAKE_INCLUDE()
# -----------------
# Check to see how make treats includes.
AC_DEFUN([AM_MAKE_INCLUDE],
[am_make=${MAKE-make}
cat > confinc << 'END'
am__doit:
	@echo this is the am__doit target
.PHONY: am__doit
END
# If we don't find an include directive, just comment out the code.
AC_MSG_CHECKING([for style of include used by $am_make])
am__include="#"
am__quote=
_am_result=none
# First try GNU make style include.
echo "include confinc" > confmf
# Ignore all kinds of additional output from 'make'.
case `$am_make -s -f confmf 2> /dev/null` in #(
*the\ am__doit\ target*)
  am__include=include
  am__quote=
  _am_result=GNU
  ;;
esac
# Now try BSD make style include.
if test "$am__include" = "#"; then
   echo '.include "confinc"' > confmf
   case `$am_make -s -f confmf 2> /dev/null` in #(
   *the\ am__doit\ target*)
     am__include=.include
     am__quote="\""
     _am_result=BSD
     ;;
   esac
fi
AC_SUBST([am__include])
AC_SUBST([am__quote])
AC_MSG_RESULT([$_am_result])
rm -f confinc confmf
])

# Fake the existence of programs that GNU maintainers use.  -*- Autoconf -*-

# Copyright (C) 1997-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_MISSING_PROG(NAME, PROGRAM)
# ------------------------------
AC_DEFUN([AM_MISSING_PROG],
[AC_REQUIRE([AM_MISSING_HAS_RUN])
$1=${$1-"${am_missing_run}$2"}
AC_SUBST($1)])

# AM_MISSING_HAS_RUN
# ------------------
# Define MISSING if not defined so far and test if it is modern enough.
# If it is, set am_missing_run to use it, otherwise, to nothing.
AC_DEFUN([AM_MISSING_HAS_RUN],
[AC_REQUIRE([AM_AUX_DIR_EXPAND])dnl
AC_REQUIRE_AUX_FILE([missing])dnl
if test x"${MISSING+set}" != xset; then
  case $am_aux_dir in
  *\ * | *\	*)
    MISSING="\${SHELL} \"$am_aux_dir/missing\"" ;;
  *)
    MISSING="\${SHELL} $am_aux_dir/missing" ;;
  esac
fi
# Use eval to expand $SHELL
if eval "$MISSING --is-lightweight"; then
  am_missing_run="$MISSING "
else
  am_missing_run=
  AC_MSG_WARN(['missing' script is too old or missing])
fi
])

# Helper functions for option handling.                     -*- Autoconf -*-

# Copyright (C) 2001-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# _AM_MANGLE_OPTION(NAME)
# -----------------------
AC_DEFUN([_AM_MANGLE_OPTION],
[[_AM_OPTION_]m4_bpatsubst($1, [[^a-zA-Z0-9_]], [_])])

# _AM_SET_OPTION(NAME)
# --------------------
# Set option NAME.  Presently that only means defining a flag for this option.
AC_DEFUN([_AM_SET_OPTION],
[m4_define(_AM_MANGLE_OPTION([$1]), [1])])

# _AM_SET_OPTIONS(OPTIONS)
# ------------------------
# OPTIONS is a space-separated list of Automake options.
AC_DEFUN([_AM_SET_OPTIONS],
[m4_foreach_w([_AM_Option], [$1], [_AM_SET_OPTION(_AM_Option)])])

# _AM_IF_OPTION(OPTION, IF-SET, [IF-NOT-SET])
# -------------------------------------------
# Execute IF-SET if OPTION is set, IF-NOT-SET otherwise.
AC_DEFUN([_AM_IF_OPTION],
[m4_ifset(_AM_MANGLE_OPTION([$1]), [$2], [$3])])

# Copyright (C) 1999-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# _AM_PROG_CC_C_O
# ---------------
# Like AC_PROG_CC_C_O, but changed for automake.  We rewrite AC_PROG_CC
# to automatically call this.
AC_DEFUN([_AM_PROG_CC_C_O],
[AC_REQUIRE([AM_AUX_DIR_EXPAND])dnl
AC_REQUIRE_AUX_FILE([compile])dnl
AC_LANG_PUSH([C])dnl
AC_CACHE_CHECK(
  [whether $CC understands -c and -o together],
  [am_cv_prog_cc_c_o],
  [AC_LANG_CONFTEST([AC_LANG_PROGRAM([])])
  # Make sure it works both with $CC and with simple cc.
  # Following AC_PROG_CC_C_O, we do the test twice because some
  # compilers refuse to overwrite an existing .o file with -o,
  # though they will create one.
  am_cv_prog_cc_c_o=yes
  for am_i in 1 2; do
    if AM_RUN_LOG([$CC -c conftest.$ac_ext -o conftest2.$ac_objext]) \
         && test -f conftest2.$ac_objext; then
      : OK
    else
      am_cv_prog_cc_c_o=no
      break
    fi
  done
  rm -f core conftest*
  unset am_i])
if test "$am_cv_prog_cc_c_o" != yes; then
   # Losing compiler, so override with the script.
   # FIXME: It is wrong to rewrite CC.
   # But if we don't then we get into trouble of one sort or another.
   # A longer-term fix would be to have automake use am__CC in this case,
   # and then we could set am__CC="\$(top_srcdir)/compile \$(CC)"
   CC="$am_aux_dir/compile $CC"
fi
AC_LANG_POP([C])])

# For backward compatibility.
AC_DEFUN_ONCE([AM_PROG_CC_C_O], [AC_REQUIRE([AC_PROG_CC])])

# Copyright (C) 2001-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_RUN_LOG(COMMAND)
# -------------------
# Run COMMAND, save the exit status in ac_status, and log it.
# (This has been adapted from Autoconf's _AC_RUN_LOG macro.)
AC_DEFUN([AM_RUN_LOG],
[{ echo "$as_me:$LINENO: $1" >&AS_MESSAGE_LOG_FD
   ($1) >&AS_MESSAGE_LOG_FD 2>&AS_MESSAGE_LOG_FD
   ac_status=$?
   echo "$as_me:$LINENO: \$? = $ac_status" >&AS_MESSAGE_LOG_FD
   (exit $ac_status); }])

# Check to make sure that the build environment is sane.    -*- Autoconf -*-

# Copyright (C) 1996-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_SANITY_CHECK
# ---------------
AC_DEFUN([AM_SANITY_CHECK],
[AC_MSG_CHECKING([whether build environment is sane])
# Reject unsafe characters in $srcdir or the absolute working directory
# name.  Accept space and tab only in the latter.
am_lf='
'
case `pwd` in
  *[[\\\"\#\$\&\'\`$am_lf]]*)
    AC_MSG_ERROR([unsafe absolute working directory name]);;
esac
case $srcdir in
  *[[\\\"\#\$\&\'\`$am_lf\ \	]]*)
    AC_MSG_ERROR([unsafe srcdir value: '$srcdir']);;
esac

# Do 'set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   am_has_slept=no
   for am_try in 1 2; do
     echo "timestamp, slept: $am_has_slept" > conftest.file
     set X `ls -Lt "$srcdir/configure" conftest.file 2> /dev/null`
     if test "$[*]" = "X"; then
	# -L didn't work.
	set X `ls -t "$srcdir/configure" conftest.file`
     fi
     if test "$[*]" != "X $srcdir/configure conftest.file" \
	&& test "$[*]" != "X conftest.file $srcdir/configure"; then

	# If neither matched, then we have a broken ls.  This can happen
	# if, for instance, CONFIG_SHELL is bash and it inherits a
	# broken ls alias from the environment.  This has actually
	# happened.  Such a system could not be considered "sane".
	AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
  alias in your environment])
     fi
     if test "$[2]" = conftest.file || test $am_try -eq 2; then
       break
     fi
     # Just in case.
     sleep 1
     am_has_slept=yes
   done
   test "$[2]" = conftest.file
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
AC_MSG_RESULT([yes])
# If we didn't sleep, we still need to ensure time stamps of config.status and
# generated files are strictly newer.
am_sleep_pid=
if grep 'slept: no' conftest.file >/dev/null 2>&1; then
  ( sleep 1 ) &
  am_sleep_pid=$!
fi
AC_CONFIG_COMMANDS_PRE(
  [AC_MSG_CHECKING([that generated files are newer than configure])
   if test -n "$am_sleep_pid"; then
     # Hide warnings about reused PIDs.
     wait $am_sleep_pid 2>/dev/null
   fi
   AC_MSG_RESULT([done])])
rm -f conftest.file
])

# Copyright (C) 2009-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_SILENT_RULES([DEFAULT])
# --------------------------
# Enable less verbose build rules; with the default set to DEFAULT
# ("yes" being less verbose, "no" or empty being verbose).
AC_DEFUN([AM_SILENT_RULES],
[AC_ARG_ENABLE([silent-rules], [dnl
AS_HELP_STRING(
  [--enable-silent-rules],
  [less verbose build output (undo: "make V=1")])
AS_HELP_STRING(
  [--disable-silent-rules],
  [verbose build output (undo: "make V=0")])dnl
])
case $enable_silent_rules in @%:@ (((
  yes) AM_DEFAULT_VERBOSITY=0;;
   no) AM_DEFAULT_VERBOSITY=1;;
    *) AM_DEFAULT_VERBOSITY=m4_if([$1], [yes], [0], [1]);;
esac
dnl
dnl A few 'make' implementations (e.g., NonStop OS and NextStep)
dnl do not support nested variable expansions.
dnl See automake bug#9928 and bug#10237.
am_make=${MAKE-make}
AC_CACHE_CHECK([whether $am_make supports nested variables],
   [am_cv_make_support_nested_variables],
   [if AS_ECHO([['TRUE=$(BAR$(V))
BAR0=false
BAR1=true
V=1
am__doit:
	@$(TRUE)
.PHONY: am__doit']]) | $am_make -f - >/dev/null 2>&1; then
  am_cv_make_support_nested_variables=yes
else
  am_cv_make_support_nested_variables=no
fi])
if test $am_cv_make_support_nested_variables = yes; then
  dnl Using '$V' instead of '$(V)' breaks IRIX make.
  AM_V='$(V)'
  AM_DEFAULT_V='$(AM_DEFAULT_VERBOSITY)'
else
  AM_V=$AM_DEFAULT_VERBOSITY
  AM_DEFAULT_V=$AM_DEFAULT_VERBOSITY
fi
AC_SUBST([AM_V])dnl
AM_SUBST_NOTMAKE([AM_V])dnl
AC_SUBST([AM_DEFAULT_V])dnl
AM_SUBST_NOTMAKE([AM_DEFAULT_V])dnl
AC_SUBST([AM_DEFAULT_VERBOSITY])dnl
AM_BACKSLASH='\'
AC_SUBST([AM_BACKSLASH])dnl
_AM_SUBST_NOTMAKE([AM_BACKSLASH])dnl
])

# Copyright (C) 2001-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_PROG_INSTALL_STRIP
# ---------------------
# One issue with vendor 'install' (even GNU) is that you can't
# specify the program used to strip binaries.  This is especially
# annoying in cross-compiling environments, where the build's strip
# is unlikely to handle the host's binaries.
# Fortunately install-sh will honor a STRIPPROG variable, so we
# always use install-sh in "make install-strip", and initialize
# STRIPPROG with the value of the STRIP variable (set by the user).
AC_DEFUN([AM_PROG_INSTALL_STRIP],
[AC_REQUIRE([AM_PROG_INSTALL_SH])dnl
# Installed binaries are usually stripped using 'strip' when the user
# run "make install-strip".  However 'strip' might not be the right
# tool to use in cross-compilation environments, therefore Automake
# will honor the 'STRIP' environment variable to overrule this program.
dnl Don't test for $cross_compiling = yes, because it might be 'maybe'.
if test "$cross_compiling" != no; then
  AC_CHECK_TOOL([STRIP], [strip], :)
fi
INSTALL_STRIP_PROGRAM="\$(install_sh) -c -s"
AC_SUBST([INSTALL_STRIP_PROGRAM])])

# Copyright (C) 2006-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# _AM_SUBST_NOTMAKE(VARIABLE)
# ---------------------------
# Prevent Automake from outputting VARIABLE = @VARIABLE@ in Makefile.in.
# This macro is traced by Automake.
AC_DEFUN([_AM_SUBST_NOTMAKE])

# AM_SUBST_NOTMAKE(VARIABLE)
# --------------------------
# Public sister of _AM_SUBST_NOTMAKE.
AC_DEFUN([AM_SUBST_NOTMAKE], [_AM_SUBST_NOTMAKE($@)])

# Check how to create a tarball.                            -*- Autoconf -*-

# Copyright (C) 2004-2014 Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# _AM_PROG_TAR(FORMAT)
# --------------------
# Check how to create a tarball in format FORMAT.
# FORMAT should be one of 'v7', 'ustar', or 'pax'.
#
# Substitute a variable $(am__tar) that is a command
# writing to stdout a FORMAT-tarball containing the directory
# $tardir.
#     tardir=directory && $(am__tar) > result.tar
#
# Substitute a variable $(am__untar) that extract such
# a tarball read from stdin.
#     $(am__untar) < result.tar
#
AC_DEFUN([_AM_PROG_TAR],
[# Always define AMTAR for backward compatibility.  Yes, it's still used
# in the wild :-(  We should find a proper way to deprecate it ...
AC_SUBST([AMTAR], ['$${TAR-tar}'])

# We'll loop over all known methods to create a tar archive until one works.
_am_tools='gnutar m4_if([$1], [ustar], [plaintar]) pax cpio none'

m4_if([$1], [v7],
  [am__tar='$${TAR-tar} chof - "$$tardir"' am__untar='$${TAR-tar} xf -'],

  [m4_case([$1],
    [ustar],
     [# The POSIX 1988 'ustar' format is defined with fixed-size fields.
      # There is notably a 21 bits limit for the UID and the GID.  In fact,
      # the 'pax' utility can hang on bigger UID/GID (see automake bug#8343
      # and bug#13588).
      am_max_uid=2097151 # 2^21 - 1
      am_max_gid=$am_max_uid
      # The $UID and $GID variables are not portable, so we need to resort
      # to the POSIX-mandated id(1) utility.  Errors in the 'id' calls
      # below are definitely unexpected, so allow the users to see them
      # (that is, avoid stderr redirection).
      am_uid=`id -u || echo unknown`
      am_gid=`id -g || echo unknown`
      AC_MSG_CHECKING([whether UID '$am_uid' is supported by ustar format])
      if test $am_uid -le $am_max_uid; then
         AC_MSG_RESULT([yes])
      else
         AC_MSG_RESULT([no])
         _am_tools=none
      fi
      AC_MSG_CHECKING([whether GID '$am_gid' is supported by ustar format])
      if test $am_gid -le $am_max_gid; then
         AC_MSG_RESULT([yes])
      else
        AC_MSG_RESULT([no])
        _am_tools=none
      fi],

  [pax],
    [],

  [m4_fatal([Unknown tar format])])

  AC_MSG_CHECKING([how to create a $1 tar archive])

  # Go ahead even if we have the value already cached.  We do so because we
  # need to set the values for the 'am__tar' and 'am__untar' variables.
  _am_tools=${am_cv_prog_tar_$1-$_am_tools}

  for _am_tool in $_am_tools; do
    case $_am_tool in
    gnutar)
      for _am_tar in tar gnutar gtar; do
        AM_RUN_LOG([$_am_tar --version]) && break
      done
      am__tar="$_am_tar --format=m4_if([$1], [pax], [posix], [$1]) -chf - "'"$$tardir"'
      am__tar_="$_am_tar --format=m4_if([$1], [pax], [posix], [$1]) -chf - "'"$tardir"'
      am__untar="$_am_tar -xf -"
      ;;
    plaintar)
      # Must skip GNU tar: if it does not support --format= it doesn't create
      # ustar tarball either.
      (tar --version) >/dev/null 2>&1 && continue
      am__tar='tar chf - "$$tardir"'
      am__tar_='tar chf - "$tardir"'
      am__untar='tar xf -'
      ;;
    pax)
      am__tar='pax -L -x $1 -w "$$tardir"'
      am__tar_='pax -L -x $1 -w "$tardir"'
      am__untar='pax -r'
      ;;
    cpio)
      am__tar='find "$$tardir" -print | cpio -o -H $1 -L'
      am__tar_='find "$tardir" -print | cpio -o -H $1 -L'
      am__untar='cpio -i -H $1 -d'
      ;;
    none)
      am__tar=false
      am__tar_=false
      am__untar=false
      ;;
    esac

    # If the value was cached, stop now.  We just wanted to have am__tar
    # and am__untar set.
    test -n "${am_cv_prog_tar_$1}" && break

    # tar/untar a dummy directory, and stop if the command works.
    rm -rf conftest.dir
    mkdir conftest.dir
    echo GrepMe > conftest.dir/file
    AM_RUN_LOG([tardir=conftest.dir && eval $am__tar_ >conftest.tar])
    rm -rf conftest.dir
    if test -s conftest.tar; then
      AM_RUN_LOG([$am__untar <conftest.tar])
      AM_RUN_LOG([cat conftest.dir/file])
      grep GrepMe conftest.dir/file >/dev/null 2>&1 && break
    fi
  done
  rm -rf conftest.dir

  AC_CACHE_VAL([am_cv_prog_tar_$1], [am_cv_prog_tar_$1=$_am_tool])
  AC_MSG_RESULT([$am_cv_prog_tar_$1])])

AC_SUBST([am__tar])
AC_SUBST([am__untar])
]) # _AM_PROG_TAR

