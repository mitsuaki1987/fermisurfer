AC_PREREQ([2.69])
AC_INIT([fermisurfer], [1.10.0], [mkawamura@issp.u-tokyo.ac.jp])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_HEADERS([config.h])

echo ""
echo "  Check compiler"
echo ""
AC_PROG_CC
echo ""
echo "  Check OpenMP"
echo ""
echo ""
echo "  Check OpenMP"
echo ""
AC_ARG_WITH(openmp, [AS_HELP_STRING([--with-openmp],
                                 [compile with OpenMP (parallelization) support.
                                  If none is found, OpenMP is not used. Default: yes])],,
            [with_openmp=yes])
if test x"$with_openmp" != xno; then
  AX_OPENMP([FCFLAGS="$FCFLAGS $OPENMP_CFLAGS";CFLAGS="$CFLAGS $OPENMP_CFLAGS"],[
      AC_MSG_WARN([OpenMP requested, but couldn't use OpenMP.])
  ])
fi
echo ""
echo "  Check OpenGL and GLUT"
echo ""
AX_CHECK_GLUT
CFLAGS="$CFLAGS $GLUT_CFLAGS"
LIBS="$LIBS $GLUT_LIBS -lm"
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
echo "        CC = ${CC}"
echo "    CFLAGS = ${CFLAGS}"
echo "      LIBS = ${LIBS}"
echo ""
echo "  Configure done."
echo ""
