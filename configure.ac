AC_PREREQ([2.69])
AC_INIT([fermisurfer], [2.4.0], [mkawamura@issp.u-tokyo.ac.jp])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_HEADERS([config.h])

echo ""
echo "  Check compiler"
echo ""
AC_PROG_CXX
echo ""
echo "  Check OpenMP"
echo ""
AC_ARG_WITH(openmp, [AS_HELP_STRING([--with-openmp],
                                 [compile with OpenMP (parallelization) support.
                                  If none is found, OpenMP is not used. Default: yes])],,
            [with_openmp=yes])
if test x"$with_openmp" != xno; then
  AX_OPENMP([CXXFLAGS="$CXXFLAGS $OPENMP_CFLAGS"],[
      AC_MSG_WARN([OpenMP requested, but couldn't use OpenMP.])
  ])
fi
echo ""
echo "  Check OpenGL"
echo ""
AX_CHECK_GL
AX_CHECK_GLU
CXXFLAGS="$CXXFLAGS $GL_CFLAGS $GLU_CFLAGS"
LIBS="$LIBS $GLU_LIBS $GL_LIBS -lm"
echo ""
echo "  Check wxWidgets"
echo ""
AM_OPTIONS_WXCONFIG
reqwx=2.4.0
AM_PATH_WXCONFIG($reqwx, wxWin=1,,[std,gl])
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.

		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
fi

CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS"
echo ""
echo "  Output files"
echo ""
AC_CONFIG_FILES([
Makefile
src/Makefile
])

AC_OUTPUT

echo ""
echo "  SUMMARY"
echo ""
echo "       CXX = ${CXX}"
echo "  CXXFLAGS = ${CXXFLAGS}"
echo "      LIBS = ${LIBS}"
echo ""
echo "  Configure done."
echo ""
