# $Id$

SUBDIRS = cutil

INCLUDES = -I. ${XMLPP_CFLAGS}
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic-errors

lib_LTLIBRARIES = libcutil.la

if XMLPP_SUPPORT
  XML_STATE_HANDLER=XMLStateHandler.cc XMLStateNode.cc
endif

libcutil_la_SOURCES = \
	BitHack.cc \
	Dimension.cc \
	Exception.cc \
	InetAddress.cc \
	InetException.cc \
	NamedPipe.cc \
	NamedPipeException.cc \
	Point.cc \
	Rectangle.cc \
	ServerSocket.cc \
	Socket.cc \
	SocketException.cc \
	StateHandler.cc \
	StateNode.cc \
	${XML_STATE_HANDLER}

libcutil_la_LIBADD = ${XMLPP_LIBS}

libcutil_la_LDFLAGS = -version-info ${LIBTOOL_LIBRARY_VERSION}
