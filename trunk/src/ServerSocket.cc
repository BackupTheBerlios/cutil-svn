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

#include <cutil/ServerSocket.h>

#include <cutil/InetAddress.h>
#include <cutil/Socket.h>
#include <cutil/SocketException.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <memory>
#include <string>
#include <sstream>

using namespace cutil ;

//-------------------------------------------------------------------------------//
// Constructor / Desctructor

/**
 * Constructs an unbound Server Socket
 *
 */
ServerSocket::ServerSocket()
{
	theSocketDescriptor = createServerSocketFd() ;
	theState = UNBOUND_ENUM ;
}

/**
 * Creates a Server socket that is bound to the specified port.
 * After construction, the server socket is bound and in the listening state
 *
 * @param port the port number to bind to
 * @throws SocketException if there is an error binding the socket
 */
ServerSocket::ServerSocket(int port) throw(SocketException)
{
	// set the state after each stage, so that we if we get an exception a call to getState still
	// returns correct information
	theState = UNBOUND_ENUM ;
	theSocketDescriptor = createServerSocketFd() ;

	// use SO_REUSEADDR by default
	setReuseAddress(true) ;

	bind(0, port) ;
	theState = BOUND_ENUM ;

	// indicate that we are willing to accept connections
	listen() ;

	theState = LISTENING_ENUM ;
}

/**
 * Creates a Server socket that is bound to the specified port with the specified backlog number.
 * The backlog parameter defines the maximum queue length for incomming connections. If a connection request arrives
 * and the queue is full, the connection will be refused.
 * After construction, the server socket is bound and in the listening state
 *
 * @param port the port number to bind to
 * @param backlog the maximum incomming connection queue length
 * @throws SocketException if there is an error binding the socket
 */
ServerSocket::ServerSocket(int port, int backlog) throw(SocketException)
{
	theState = UNBOUND_ENUM ;
	theSocketDescriptor = createServerSocketFd() ;

	// use SO_REUSEADDR by default
	setReuseAddress(true) ;

	bind(0, port) ;
	theState = BOUND_ENUM ;

	// indicate that we a rewilling to accept connections
	listen(backlog) ;

	theState = LISTENING_ENUM ;
}

/**
 * Server Socket Destructor
 * If this Server Socket is still connected, the Socket will be shutdown
 * and closed
 *
 */
ServerSocket::~ServerSocket()
{
	if(theSocketDescriptor != -1)
	{
		close() ;
	}
}


//-------------------------------------------------------------------------------//
// ServerSocket Operations

/**
 * Listens for connection being made on this socket and accepts them.
 * This method will block until a connection is accepted
 *
 * @return the newly accepted socket, or 0 on failure
 * @throw SocketException if there is an error during the accept
 */
std::auto_ptr<Socket>
ServerSocket::accept() throw(SocketException)
{
	std::auto_ptr<Socket> newConnection ;

	if(theState == LISTENING_ENUM)
	{
		// structure to popu;late with the new connectrion details, remote address address of the connection etc
		struct sockaddr_in sockAddr ;
		int addrLength = sizeof(sockAddr) ;

		int retcode = ::accept(theSocketDescriptor, reinterpret_cast<sockaddr*>(&sockAddr), reinterpret_cast<socklen_t*>(&addrLength)) ;

		if(retcode == -1)
		{
			throw(SocketException(std::string("Exception in accept [accept]:").append(::strerror(errno)))) ;
		}
		else
		{
			// if not an error, then the socket call returns the Socket descriptor

			// we use the sockaddr_in struct above which is populated with the details of the remote connection
			// to populate our new Socket object with valid host and port values.
			// we also specifiy the returned socket descriptor to the constructor to 'wrap' our socket connection
			// in a cutil::Socket object.

			char host[INET_ADDRSTRLEN] ;
			const char* ret = ::inet_ntop(AF_INET, &sockAddr.sin_addr, host,  INET_ADDRSTRLEN) ;

			if(ret == 0)
			{
				throw(SocketException(std::string("Exception in accept [inet_ntop]:").append(::strerror(errno)))) ;
			}

			int port = ntohs(sockAddr.sin_port) ;

			newConnection.reset(new Socket(std::string(host), port, retcode)) ;
		}
	}
	else
	{
		throw(SocketException("Accept may only be called upon a ServerSocket that has been bound and is in a listening state")) ;
	}

	return(newConnection) ;
}

/**
 * Binds this socket to a local address
 *
 * @param host the host IP Address
 * @param port the port number
 * @throws SocketException if there is an error during binding
  */
void
ServerSocket::bind(const InetAddress* host, int port) throw(SocketException)
{
	if(theState == UNBOUND_ENUM)
	{
		struct sockaddr_in sockAddr ;
		sockAddr.sin_family = AF_INET ;
		sockAddr.sin_port = htons(port) ;

		// if the specified host is empty we assume INADDR_ANY
		if(!host)
		{
			sockAddr.sin_addr.s_addr = INADDR_ANY ;
		}
		else
		{
			sockAddr.sin_addr = host->getAddress() ;
		}

		int retcode = ::bind(theSocketDescriptor, reinterpret_cast<struct sockaddr*>(&sockAddr), sizeof(sockAddr));

		if(retcode == -1)
		{
			throw(SocketException(std::string("Exception in bind [bind]:").append(::strerror(errno)))) ;
		}
		else
		{
			theState = BOUND_ENUM ;
		}
	}
	else
	{
		throw(SocketException("Bind cannot be called upon a socket that is already bound or is closed closed")) ;
	}
}

/**
 * Close this socket
 *
 * @throw SocketException if an error occurs during the closing of this ServerSocket
 */
void
ServerSocket::close() throw(SocketException)
{
	if(::close(theSocketDescriptor) == 0)
	{
		theSocketDescriptor = -1 ;
		theState = CLOSED_ENUM ;
	}
	else
	{
		throw(SocketException(std::string("Exception in close [close]:").append(::strerror(errno)))) ;
	}
}

/**
 * Returns the local InetAddress of this ServerSocket
 * If this ServerSocket is not yet bound, this method will return 0
 *
 * @return the local InetAddress of this ServerSocket
 */
std::auto_ptr<InetAddress>
ServerSocket::getInetAddress() const
{
	std::auto_ptr<InetAddress> ret ;

	if(theState == BOUND_ENUM)
	{
		struct sockaddr_in sockAddr ;
		socklen_t addrLength = static_cast<socklen_t>(sizeof(sockAddr)) ;
		if(::getsockname(theSocketDescriptor, reinterpret_cast<struct sockaddr*>(&sockAddr), &addrLength) == -1)
		{
			throw(InetException(std::string("Exception in getLocalAddress [getsockname]:").append(::strerror(errno)))) ;
		}
		else
		{
			ret.reset(new InetAddress(sockAddr.sin_addr)) ;
		}
	}

	return(ret) ;
}

/**
 * Returns the local port to which this ServerSocket is bound
 * If this ServerSocket is not yet bound, -1 will be returned
 *
 * @return the local port number to which this socket is bound, or -1 if this ServerSocket is not yet bound
 */
int
ServerSocket::getPort() const
{
	int ret = -1 ;

	if(theState == BOUND_ENUM)
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
 * Returns the current state of this ServerSocket
 *
 * @return the current state of this ServerSocket
 */
ServerSocket::ServerSocketStateEnum
ServerSocket::getState() const
{
	return(theState) ;
}

/**
 * Sets the willingness of this socket to accept connections
 * This ServerSocket will have its backlog value set to DEFAULT_BACKLOG
 * @throw SocketException if there is an error on the listen procedure
 */
void
ServerSocket::listen() throw(SocketException)
{
	listen(ServerSocket::DEFAULT_BACKLOG) ;
}

/**
 * Sets the willingness of this socket to accept connections
 *
 * @param backlog the maximum incomming connection queue length
 * @throw SocketException if there is an error on the listen procedure
 */
void
ServerSocket::listen(int backlog) throw(SocketException)
{
	if(theState == BOUND_ENUM)
	{
		int retcode = ::listen(theSocketDescriptor, backlog) ;

		if(retcode == -1)
		{
			throw(InetException(std::string("Exception in listen [listen]:").append(::strerror(errno)))) ;
		}
		else
		{
			theState = LISTENING_ENUM ;
		}
	}
	else
	{
		throw(SocketException("Cannot call listen on an unbound or closed ServerSocket")) ;
	}
}

/**
 * Sets whether a call to bind may reuse a local address that is in a TIME_WAIT state.
 * Typically, when a connection is closed, the connection may remain in a timeout state for a period after the 
 * connection is closed. If a specific local address is required for use, and a previous connection is in the
 * timeout state, a call to setReuseAddess(true) will allow the local address to be reused.
 *
 * setReuseAddress must be called prior to calling bind upon this ServerSocket. If called on a bound
 * ServerSocket, a SocketException will be raised.
 *
 * @param yn set true to enable the SO_REUSEADDR on this ServerSocket, false to disable
 * @throw SocketException if an error occurs enabling setting the ServerSocket option, or is called upon a bound, or closed ServerSocket.
 */
void
ServerSocket::setReuseAddress(bool yn) throw(SocketException)
{
	if(theState == UNBOUND_ENUM)
	{
		int optval = 1 ;
		socklen_t optlen = static_cast<socklen_t>(sizeof(optval)) ;
		if(::setsockopt(theSocketDescriptor, SOL_SOCKET, SO_REUSEADDR, &optval, optlen) == -1)
		{
			throw(InetException(std::string("Exception in setReuseAddress [setsockopt]:").append(::strerror(errno)))) ;
		}
	}
	else
	{
		throw(SocketException("Cannot call setReuseAddress on a bound ServerSocket")) ;
	}
}

/**
 * Returns whether SO_REUSEADDR is enabled upon this ServerSocket
 *
 * @return true if SO_REUSEADDR is enabled upon this ServerSocket, false otherwise
 */
bool
ServerSocket::getReuseAddress() const
{
	int optval ;
	socklen_t optlen = static_cast<socklen_t>(sizeof(optval)) ;
	if(::getsockopt(theSocketDescriptor, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen) == -1)
	{
		throw(InetException(std::string("Exception in getReuseAddress [getsockopt]:").append(::strerror(errno)))) ;
	}

	if(optval == 0)
	{
		return(false) ;
	}
	else
	{
		return(true) ;
	}
}


/**
 * Creates the ServerSocket file descriptor
 *
 * @return the file descriptor for the newly created socket
 * @throw SocketException if there is an error creating the socket
 */
int
ServerSocket::createServerSocketFd() throw(SocketException)
{
	int retcode = ::socket(AF_INET, SOCK_STREAM, 0) ;

	if(retcode == -1)
	{
		throw(InetException(std::string("Exception in createServerSocketFd [socket]:").append(::strerror(errno)))) ;
	}

	return(retcode) ;
}
