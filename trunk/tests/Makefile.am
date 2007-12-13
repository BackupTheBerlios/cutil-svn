noinst_PROGRAMS = UnitTests

AM_CXXFLAGS = -I${top_srcdir}/src

UnitTests_SOURCES = \
	EnumTest.cc \
	MapIteratorTest.cc \
	NullableTest.cc \
	RefCountPtrTest.cc \
	UnitTests.cc

noinst_HEADERS = \
	EnumTest.h \
	MapIteratorTest.h \
	NullableTest.h \
	RefCountPtrTest.h

UnitTests_LDADD = ../src/libcutil.la
