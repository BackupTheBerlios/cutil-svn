/*
 * Copyright (C) 2004-2005  Colin Law
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * @author Colin Law [claw@mail.berlios.de]
 * $Id$
 */

#include <cutil/Socket.h>

#include <cutil/InetAddress.h>
#include <cutil/InetException.h>
#include <cutil/SocketException.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <string>
#include <sstream>

using namespace cutil ;

/**
 * Create an initially unconnected socket.
 *
 */
Socket::Socket()
{
	theSocketDescriptor = -1 ;
	theConnectedFlag = false ;
	theClosedFlag = false ;
	theInputShutdownFlag = false ;
	theOutputShutdownFlag = false;
}

/**
 * Creates a Socket and connects to the specifed port and host.
 *
 * @param host the host to connect to.
 * @param port the remote port number
 */
Socket::Socket(const InetAddress& host, int port) throw(SocketException)
{
	// may throw SocketException
	theSocketDescriptor = createSocketFd() ;
	connect(host, port) ;

	// if no exception were thrown, then set our status flags
	theConnectedFlag = true ;
	theClosedFlag = false ;
	theInputShutdownFlag = false ;
	theOutputShutdownFlag = false;
}

/**
 * Creates a Socket and connects to the specifed port and host.
 *
 * @param host the host to connect to.
 * @param port the remote port number
 */
Socket::Socket(const std::string& host, int port) throw(InetException, SocketException)
{
	// may throw SocketException
	theSocketDescriptor = createSocketFd() ;

	// create an InetAddress fromthe specified host string
	// may throw InetException
	InetAddress address(host) ;
	connect(address, port) ;

	// if no exception were thrown, then set our status flags
	theConnectedFlag = true ;
	theClosedFlag = false ;
	theInputShutdownFlag = false ;
	theOutputShutdownFlag = false;
}

/**
 * Creates a Socket from the specified file descriptor connected to the specified host and port.
 * This method acts as a wrapper around a constructed socket and file descriptor.
 * This method is intended as a convenient constructor for the creation of a Socket to 'wrap'
 * the file descriptor returned from a successful call to accept of a ServerSocket
 *
 * @param host the remote host this Socket is connected to
 * @param port the remote port
 * @param fd the file descriptor used for socket communications
 * @throws SocketException if fd is not a valid socket file descriptor
 */
Socket::Socket(const InetAddress& host, int port, int fd) throw(SocketException)
{
	// varify that the specified descriptor is a socket
	struct stat statbuf ;
	if(::fstat(fd, &statbuf) == 0)
	{
		// exists, but is it a socket
		if(S_ISSOCK(statbuf.st_mode))
		{
			theSocketDescriptor = fd ;

			// @note[todo] need to check that the socket is really connected etc.
			theConnectedFlag = true ;
		}
		else
		{
			theSocketDescriptor = -1 ;
			theConnectedFlag = false ;
			throw(SocketException("The specified descriptor is not a socket descriptor")) ;
		}
	}
	else
	{
		throw(SocketException(std::string("Exception in Socket constructor [fstat]: ").append(::strerror(errno)))) ;
	}


	theClosedFlag = false ;
	theInputShutdownFlag = false ;
	theOutputShutdownFlag = false;
}

/**
 * Destructor.
 *
 */
Socket::~Socket()
{
	if(theSocketDescriptor != -1)
	{
		close() ;
	}
}

//-------------------------------------------------------------------------------//
// Socket Operations

/**
 * Connect this socket to the server.
 *
 * @param host the host to connect to
 * @param port the port number to connect to.
 */
void
Socket::connect(const InetAddress& host, int port) throw(SocketException)
{
	if(theSocketDescriptor == -1)
	{
		throw(SocketException("No valid socket Descriptor")) ;
	}

	struct sockaddr_in sockAddr ;
	sockAddr.sin_family = AF_INET ;
	sockAddr.sin_port = htons(port) ;
	sockAddr.sin_addr = host.getAddress() ;

	int retcode = ::connect(theSocketDescriptor, (sockaddr*)&sockAddr, sizeof(sockAddr)) ;

	if (retcode == -1)
	{
		throw(SocketException(std::string("Exception in connect [connect]:").append(::strerror(errno)))) ;
	}
}

/**
 * Close this socket
 *
 * @throw SocketException if an error occurs during the closing of this Socket
 */
void
Socket::close() throw(SocketException)
{
	if(::close(theSocketDescriptor) == 0)
	{
		theSocketDescriptor = -1 ;
		theClosedFlag = true ;
	}
	else
	{
		throw(SocketException(std::string("Exception in close [close]:").append(::strerror(errno)))) ;
	}
}

/**
 * Returns the remote host to which this Socket is conencted
 * If this Socket is not yet connected, 0 will be returned
 *
 * @return the host to which this Socket is connected, or 0 if this socket is not yet connected
 */
std::auto_ptr<InetAddress>
Socket::getInetAddress() const
{
	std::auto_ptr<InetAddress> ret ;

	if(isConnected())
	{
		struct sockaddr_in sockAddr ;
		socklen_t addrLength = static_cast<socklen_t>(sizeof(sockAddr)) ;
		if(::getpeername(theSocketDescriptor, reinterpret_cast<struct sockaddr*>(&sockAddr), &addrLength) == -1)
		{
			std::string err(::strerror(errno)) ;
			throw(InetException(std::string("Exception in getLocalAddress [getsockname]:").append(err))) ;
		}
		else
		{
			ret.reset(new InetAddress(sockAddr.sin_addr)) ;
		}
	}

	return(ret) ;
}

/**
 * Returns the remote port number to which this Socket is connected.
 * If this Socket is not yet connected, -1 will be returned
 *
 * @return the remote port number to which this Socket is connected, or -1 if this Socket is not yet connected
 */
int
Socket::getPort() const
{
	int ret = -1 ;

	if(isConnected())
	{
		struct sockaddr_in sockAddr ;
		socklen_t addrLength = static_cast<socklen_t>(sizeof(sockAddr)) ;

		if(::getpeername(theSocketDescriptor, reinterpret_cast<struct sockaddr*>(&sockAddr), &addrLength) == -1)
		{
			std::string err(::strerror(errno)) ;
			throw(InetException(std::string("Exception in getPort [getsockname]:").append(err))) ;
		}
		else
		{
			ret = ntohs(sockAddr.sin_port) ;
		}
	}

	return(ret) ;
}

/**
 * Returns the local address to which this Socket is bound.
 * This may also known as the address of the peer socket
 * If this Socket is not yet connected, 0 will be returned
 *
 * @return the local address to which this Socket is connected, or 0 if this socket is not yet bound
 */
std::auto_ptr<InetAddress>
Socket::getLocalAddress() const
{
	std::auto_ptr<InetAddress> ret ;

	if(isConnected())
	{
		struct sockaddr_in sockAddr ;
		socklen_t addrLength = static_cast<socklen_t>(sizeof(sockAddr)) ;
		if(::getsockname(theSocketDescriptor, reinterpret_cast<struct sockaddr*>(&sockAddr), &addrLength) == -1)
		{
			std::string err(::strerror(errno)) ;
			throw(InetException(std::string("Exception in getLocalAddress [getsockname]:").append(err))) ;
		}
		else
		{
			ret.reset(new InetAddress(sockAddr.sin_addr)) ;
		}
	}

	return(ret) ;
}

/**
 * Returns the local port number to which this Socket is bound
 * This may also known as the port number of the peer socket
 * If this Socket is not yet connected, -1 will be returned
 *
 * @return the local port number to which this Socket is bound, or -1 if this Socket is not yet bound
 */
int
Socket::getLocalPort() const
{
	int ret = -1 ;

	if(isConnected())
	{
		struct sockaddr_in sockAddr ;
		socklen_t addrLength = static_cast<socklen_t>(sizeof(sockAddr)) ;

		if(::getsockname(theSocketDescriptor, reinterpret_cast<struct sockaddr*>(&sockAddr), &addrLength) == -1)
		{
			throw(InetException(std::string("Exception in getPort [getsockname]:").append(::strerror(errno)))) ;
		}
		else
		{
			ret = ntohs(sockAddr.sin_port) ;
		}
	}

	return(ret) ;
}

/**
 * Returns the Socket Descriptor of this Socket
 *
 * @return the socket descriptor of this socket
 */
int
Socket::getSocketDescriptor() const
{
	return(theSocketDescriptor) ;
}

/**
 * Checks to see if data is availble for reading on this Socket.
 * This method will block for usec micro-seconds, waiting for data
 * to become available for reading on this socket, ie a call to read would
 * not block.
 *
 * @param usec the timeout value
 * @return true if data is availble for reading upon this socket
 *         false otherwise
 */
bool
Socket::isAvailable(long usec) const throw(SocketException)
{
	// read file descriptor set
	fd_set rfds ;
	FD_ZERO(&rfds) ;

	FD_SET(theSocketDescriptor, &rfds) ;

	struct timeval t ;
	t.tv_sec = 0 ;
	t.tv_usec = usec ;

	int nfds = theSocketDescriptor + 1 ;

	int ret = ::select(nfds, &rfds, reinterpret_cast<fd_set*>(0), reinterpret_cast<fd_set*>(0), &t) ;
	if(ret == 1)
	{
		// we explicitly set only one descriptor so we expect a return of 1
		return(true) ;
	}
	else if(ret == 0)
	{
		return(false) ;
	}
	else
	{
		throw(SocketException(std::string("Exception in isAvailable [select]: ").append(::strerror(errno)))) ;
	}
}

/**
 * Returns whether this Socket is connected.
 *
 * @return true if this Socket is connected, false otherwise
 */
bool
Socket::isConnected() const
{
	return(theConnectedFlag) ;
}

/**
 * Retuns whether this Socket is closed
 *
 * @return true if this Socket has been closed, false otherwise
 */
bool
Socket::isClosed() const
{
	return(theClosedFlag) ;
}

/**
 * Returns whether the Input or read part of this Socket has been shutdown
 *
 * @return true if the read part of this Socket connection has been shutdown
 */
bool
Socket::isInputShutdown() const
{
	return(theInputShutdownFlag) ;
}

/**
 * returns whether the output or write part of this Socket has been shutdown
 *
 * @return true if the write part of this Socket connec5tion has been shutdown
 */
bool
Socket::isOutputShutdown() const
{
	return(theOutputShutdownFlag) ;
}

/**
 * Reads from the socket into the specified std::string, returning the number of bytes read.
 * if the number of bytes read is 0, the End-of-File has been reached.
 * 
 * @param data the string to be written to
 * @return the number of bytes read.
 */
int
Socket::read(std::string& data) const throw(SocketException)
{
	char buf[Socket::DEFAULT_MAXRECV + 1] ;
	data = "" ;
	memset(buf, 0, (Socket::DEFAULT_MAXRECV + 1)) ;

	int retcode = ::recv(theSocketDescriptor, buf, Socket::DEFAULT_MAXRECV, 0) ;

	if(retcode == -1)
	{
		throw(SocketException(std::string("Exception in read [recv]: ").append(::strerror(errno)))) ;
	}
	else if(retcode == 0)
	{
		// End-of-File
		//throw(EndOfFileException)
		return(0) ;
	}
	else
	{
		data = std::string(buf) ;
		return(retcode) ;
	}
}

/**
 * Shutdown the Input Stream of this socket.
 *
 */
void
Socket::shutdownInput() throw(SocketException)
{
	if(isConnected())
	{
		int retcode = ::shutdown(theSocketDescriptor, SHUT_RD) ;

		if(retcode == -1)
		{
			throw(SocketException(std::string("Exception in shutdownInput [shutdown]: ").append(::strerror(errno)))) ;
		}
	}
}

/**
 * Shutdown the Output Stream of this socket.
 *
 */
void
Socket::shutdownOutput() throw(SocketException)
{
	if(isConnected())
	{
		int retcode = ::shutdown(theSocketDescriptor, SHUT_WR) ;

		if(retcode == -1)
		{
			throw(SocketException(std::string("Exception in shutdownOutput [shutdown]: ").append(::strerror(errno)))) ;
		}
	}
}

/**
 * Write the specified string out on the socket
 *
 * @param data the data to be written to the socket
 * @return the number of bytes written
 * @throw SocketException if there is an error writing to the socket
 */
int
Socket::write(const std::string data) const throw(SocketException)
{
	int retcode = ::send(theSocketDescriptor, data.c_str(), data.size(), MSG_NOSIGNAL) ;
	if (retcode == -1)
	{
		throw(SocketException(std::string("Exception in write [send]: ").append(::strerror(errno)))) ;
	}
	else
	{
		return(retcode) ;
	}
}

/**
 * Writes the specified number of bytes of the specified data out on the socket
 *
 * @param data the data to be written to the socket
 * @param size the size of the data
 * @return the number of bytes actually written to the socket
 * @throw SocketExceotion if there is an error writing to the socket
 */
int
Socket::writeData(const void* data, int size) const throw(SocketException)
{
	int retcode = ::send(theSocketDescriptor, data, size, MSG_NOSIGNAL) ;
	if (retcode == -1)
	{
		throw(SocketException(std::string("Exception in writeData [send]: ").append(::strerror(errno)))) ;
	}
	else
	{
		return(retcode) ;
	}
}

//-------------------------------------------------------------------------------//

/**
 * Creates the Socket file descriptor
 *
 * @return the file descriptor for the newly created socket
 * @throw SocketException if there is an error creating the socket
 */
int
Socket::createSocketFd() throw(SocketException)
{
	int retcode = ::socket(AF_INET, SOCK_STREAM, 0) ;

	if(retcode == -1)
	{
		throw(SocketException(std::string("Exception in createSocketFd [socket]: ").append(::strerror(errno)))) ;
	}

	return(retcode) ;
}

