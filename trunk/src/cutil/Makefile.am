# $Id$

library_includedir=${includedir}/${LIBRARY_NAME}

#we could use the following line instead to allow for numerous version to be installed
#but its a bit overkill at the moment
#library_includedir=$(includedir)/$(LIBRARY_NAME)-$(LIBRARY_RELEASE)/$(LIBRARY_NAME)

if XMLPP_SUPPORT
  XML_STATE_HANDLER=XMLStateHandler.h XMLStateNode.h
endif

library_include_HEADERS = \
	AbstractInputStream.h \
	AbstractOutputStream.h \
	AbstractShape.h \
	BitHack.h \
	BufferedOutputWriter.h \
	Closure.h \
	Conversion.h \
	Dimension.h \
	Enum.h \
	Exception.h \
	FilePath.h \
	InetAddress.h \
	InetException.h \
	InputReader.h \
	MapIterator.h \
	NamedPipe.h \
	NamedPipeException.h \
	Plugin.h \
	PluginFactory.h \
	PluginInfo.h \
	PluginManager.h \
	Point.h \
	Rectangle.h \
	RefCountPtr.h \
	ServerSocket.h \
	SharedLibrary.h \
	SharedLibraryException.h \
	Socket.h \
	SocketException.h \
	Stateable.h \
	StateHandler.h \
	StateNode.h \
	StringUtilities.h \
	${XML_STATE_HANDLER}




