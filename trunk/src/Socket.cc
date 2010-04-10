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
#include <cstring>
#include <string>
#include <sstream>

using namespace cutil ;

const int Socket::DEFAULT_MAXRECV = 500 ;

/**
 * Create an initially unconnected socket.
 *
 */
Socket::Socket()
{
	m_socket_descriptor = -1 ;
	m_connected = false ;
	m_closed = false ;
	m_input_shutdown = false ;
	m_output_shutdown = false;
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
	m_socket_descriptor = createSocketFd() ;
	connect(host, port) ;

	// if no exception were thrown, then set our status flags
	m_connected = true ;
	m_closed = false ;
	m_input_shutdown = false ;
	m_output_shutdown = false;
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
	m_socket_descriptor = createSocketFd() ;

	// create an InetAddress fromthe specified host string
	// may throw InetException
	InetAddress address(host) ;
	connect(address, port) ;

	// if no exception were thrown, then set our status flags
	m_connected = true ;
	m_closed = false ;
	m_input_shutdown = false ;
	m_output_shutdown = false;
}

/**
 * Creates a Socket from the specified file descriptor.
 * This method acts as a wrapper around a constructed socket descriptor.
 * This method is intended as a convenient constructor for the creation of a Socket to 'wrap'
 * the file descriptor returned from a successful call to accept of a ServerSocket
 *
 * @param fd the file descriptor used for socket communications
 * @throws SocketException if fd is not a valid socket file descriptor
 */
Socket::Socket(int fd) throw(SocketException)
{
	// varify that the specified descriptor is a socket
	struct stat statbuf ;
	if(::fstat(fd, &statbuf) == 0)
	{
		// exists, but is it a socket
		if(S_ISSOCK(statbuf.st_mode))
		{
			m_socket_descriptor = fd ;

			// @note[todo] need to check that the socket is really connected etc.
			m_connected = true ;
		}
		else
		{
			m_socket_descriptor = -1 ;
			m_connected = false ;
			throw(SocketException("The specified descriptor is not a socket descriptor")) ;
		}
	}
	else
	{
		throw(SocketException(std::string("Exception in Socket constructor [fstat]: ").append(::strerror(errno)))) ;
	}


	m_closed = false ;
	m_input_shutdown = false ;
	m_output_shutdown = false;
}

/**
 * Destructor.
 *
 */
Socket::~Socket()
{
	if(m_socket_descriptor != -1)
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
	if(m_socket_descriptor == -1)
	{
		throw(SocketException("No valid socket Descriptor")) ;
	}

	struct sockaddr_in sockAddr ;
	sockAddr.sin_family = AF_INET ;
	sockAddr.sin_port = htons(port) ;
	sockAddr.sin_addr = host.getAddress() ;

	int retcode = ::connect(m_socket_descriptor, (sockaddr*)&sockAddr, sizeof(sockAddr)) ;

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
	if(::close(m_socket_descriptor) == 0)
	{
		m_socket_descriptor = -1 ;
		m_closed = true ;
		m_connected = false ;
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
		if(::getpeername(m_socket_descriptor, reinterpret_cast<struct sockaddr*>(&sockAddr), &addrLength) == -1)
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

		if(::getpeername(m_socket_descriptor, reinterpret_cast<struct sockaddr*>(&sockAddr), &addrLength) == -1)
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
		if(::getsockname(m_socket_descriptor, reinterpret_cast<struct sockaddr*>(&sockAddr), &addrLength) == -1)
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

		if(::getsockname(m_socket_descriptor, reinterpret_cast<struct sockaddr*>(&sockAddr), &addrLength) == -1)
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
	return(m_socket_descriptor) ;
}

/**
 * Returns whether this Socket is connected.
 *
 * @return true if this Socket is connected, false otherwise
 */
bool
Socket::isConnected() const
{
	return(m_connected && !m_closed) ;
}

/**
 * Retuns whether this Socket is closed
 *
 * @return true if this Socket has been closed, false otherwise
 */
bool
Socket::isClosed() const
{
	return(m_closed) ;
}

/**
 * Returns whether the Input or read part of this Socket has been shutdown
 *
 * @return true if the read part of this Socket connection has been shutdown
 */
bool
Socket::isInputShutdown() const
{
	return(m_input_shutdown) ;
}

/**
 * returns whether the output or write part of this Socket has been shutdown
 *
 * @return true if the write part of this Socket connec5tion has been shutdown
 */
bool
Socket::isOutputShutdown() const
{
	return(m_output_shutdown) ;
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
		int retcode = ::shutdown(m_socket_descriptor, SHUT_RD) ;

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
		int retcode = ::shutdown(m_socket_descriptor, SHUT_WR) ;

		if(retcode == -1)
		{
			throw(SocketException(std::string("Exception in shutdownOutput [shutdown]: ").append(::strerror(errno)))) ;
		}
	}
}

/*
void
Socket::setNonBlocking() throw(SocketException)
{
	int flags = 0 ;
	flags = fcntl(m_socket_descriptor, F_GETFL, 0) ;
	flags = flags | O_NONBLOCK ;

	int retcode = fcntl(m_socket_descriptor, F_SETFL, flags) ;

	if(retcode == -1)
	{
		throw(SocketException(std::string("Exception in setNonBlocking [fcntl]: ").append(::strerror(errno)))) ;
	}
}
*/

//-------------------------------------------------------------------------------//
// AbstractInputStream

bool
Socket::isDataAvailable(long usec) const throw(Exception)
{
	// read file descriptor set
	fd_set rfds ;
	FD_ZERO(&rfds) ;

	FD_SET(m_socket_descriptor, &rfds) ;

	struct timeval t ;
	t.tv_sec = 0 ;
	t.tv_usec = usec ;

	int nfds = m_socket_descriptor + 1 ;

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
		throw(Exception(std::string("Exception in isDataAvailable [select]: ").append(::strerror(errno)))) ;
	}
}

ssize_t
Socket::read(void* buf, size_t length) const throw(Exception)
{
	memset(buf, 0, length) ;

	ssize_t retcode = ::recv(m_socket_descriptor, buf, length, 0) ;

	if(retcode == -1)
	{
		throw(Exception(std::string("Exception in read [recv]: ").append(::strerror(errno)))) ;
	}
	else if(retcode == 0)
	{
		// End-of-File
		//throw(EndOfFileException)
		return(0) ;
	}
	else
	{
		return(retcode) ;
	}
}

ssize_t
Socket::read(void* buf, size_t length, int& err_code) const throw()
{
	ssize_t retcode = ::recv(m_socket_descriptor, buf, length, 0) ;

	if(retcode < 0)
	{
		err_code = errno ;
	}

	return(retcode) ;
}

ssize_t
Socket::read(char& read_byte) throw(Exception)
{
	return(read(&read_byte, 1)) ;
}

ssize_t
Socket::read(char& read_byte, int& err_code) throw()
{
	return(read(&read_byte, 1, err_code)) ;
}

//-------------------------------------------------------------------------------//
// AbstractOutputStream

ssize_t
Socket::write(const void* data, size_t size) throw(Exception)
{
	ssize_t retcode = ::send(m_socket_descriptor, data, size, MSG_NOSIGNAL) ;
	if(retcode == -1)
	{
		throw(Exception(std::string("Exception in write [send]: ").append(::strerror(errno)))) ;
	}
	else
	{
		return(retcode) ;
	}
}

ssize_t
Socket::write(const void* data, size_t size, int& err_code) throw()
{
	ssize_t retcode = ::send(m_socket_descriptor, data, size, MSG_NOSIGNAL) ;

	if(retcode < 0)
	{
		err_code = errno ;
	}

	return(retcode) ;
}

ssize_t
Socket::write(const char& write_byte) throw(Exception)
{
	return(write(&write_byte, 1)) ;
}

ssize_t
Socket::write(const char& write_byte, int& err_code) throw()
{
	return(write(&write_byte, 1, err_code)) ;
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

