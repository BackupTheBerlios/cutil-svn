# $Id$

SUBDIRS = cutil

INCLUDES = -I. ${XMLPP_CFLAGS}
AM_CXXFLAGS = -W -Wall -Werror -ansi

lib_LTLIBRARIES = libcutil.la

if XMLPP_SUPPORT
  XML_STATE_HANDLER=XMLStateHandler.cc XMLStateNode.cc
endif

libcutil_la_SOURCES = \
	AbstractTestCase.cc \
	AbstractUnitTest.cc \
	BitHack.cc \
	BufferedOutputWriter.cc \
	ConsoleReporter.cc \
	DefaultTestCase.cc \
	Dimension.cc \
	Exception.cc \
	FilePath.cc \
	InetAddress.cc \
	InetException.cc \
	InputReader.cc \
	NamedPipe.cc \
	NamedPipeException.cc \
	PluginInfo.cc \
	PluginManager.cc \
	PluginManagerException.cc \
	Point.cc \
	Rectangle.cc \
	ServerSocket.cc \
	SharedLibrary.cc \
	SharedLibraryException.cc \
	Socket.cc \
	SocketException.cc \
	StateHandler.cc \
	StateNode.cc \
	StringUtilities.cc \
	TestDriver.cc \
	TestLog.cc \
	TestManager.cc \
	TestResult.cc \
	${XML_STATE_HANDLER}

libcutil_la_LIBADD = ${XMLPP_LIBS}

libcutil_la_LDFLAGS = -version-info ${LIBTOOL_LIBRARY_VERSION}
