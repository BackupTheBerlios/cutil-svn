dnl $Id$
dnl
dnl ==============================================================
dnl Process this file with autoconf to produce a configure script.
dnl ==============================================================

AC_INIT(src/cutil/Enum.h)

dnl ======================
dnl Package/Library Naming
dnl ======================
LIBRARY_NAME=cutil
PACKAGE=$LIBRARY_NAME
AC_SUBST(LIBRARY_NAME)
AC_SUBST(PACKAGE)


dnl =========================================================
dnl Package Version Info
dnl This also forms the package release version number scheme
dnl =========================================================
dnl
dnl  ? : ? : +1  == implementation only, bug fixes, minor addition, set to zero on major or minor incremented
dnl  ? :+1 : 0   == public or internal interface alterations, set zero if major is incremented
dnl +1 : 0 : 0   == increment on major code release, large-scale refactor etc
dnl MAJOR : MINOR : MICRO

MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=0

dnl ===================================================
dnl Libtool Version Numbering Scheme
dnl Note: this is independant of Package Version scheme
dnl ===================================================
dnl
dnl +1 : ? : +1  == new interface that does not break old one
dnl +1 : ? : 0   == new interface that breaks old one
dnl  ? : ? : 0   == no new interfaces, but breaks apps
dnl  ? :+1 : ?   == just some internal changes, nothing breaks but might work
dnl                 better
dnl CURRENT : REVISION : AGE

LIBTOOL_LIB_CURRENT=0
LIBTOOL_LIB_REVISION=0
LIBTOOL_LIB_AGE=0



LIBRARY_RELEASE=${MAJOR_VERSION}.${MINOR_VERSION}.${MICRO_VERSION}
LIBRARY_VERSION=${MAJOR_VERSION}.${MINOR_VERSION}.${MICRO_VERSION}
LIBTOOL_LIBRARY_VERSION=${LIBTOOL_LIB_CURRENT}:${LIBTOOL_LIB_REVISION}:${LIBTOOL_LIB_AGE}

AC_SUBST(LIBRARY_VERSION)
AC_SUBST(LIBTOOL_LIBRARY_VERSION)
AC_SUBST(LIBRARY_RELEASE)

AM_INIT_AUTOMAKE(cutil, $LIBRARY_RELEASE)
AM_SANITY_CHECK

dnl ====================
dnl Checks for programs.
dnl ====================

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_INSTALL

AC_LANG_CPLUSPLUS

dnl =====================
dnl Checks for libraries.
dnl =====================

PKG_CHECK_MODULES(XMLPP, libxml++-2.6)

AC_SUBST(XMLPP_CFLAGS)
AC_SUBST(XMLPP_LIBS)


dnl =======================
dnl Checks for C++ Features
dnl =======================

dnl need to add some c++ checks here, templates, typename etc.

dnl ============
dnl Output Files
dnl ============

AC_OUTPUT(Makefile \
          cutil.pc \
          src/Makefile \
          src/cutil/Makefile \
          tests/Makefile
)
