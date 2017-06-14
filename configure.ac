AC_PREREQ([2.69])
AC_INIT([fermisurfer], [1.9], [mkawamura@issp.u-tokyo.ac.jp])
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
AX_OPENMP([CFLAGS="$CFLAGS $OPENMP_CFLAGS"])
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
