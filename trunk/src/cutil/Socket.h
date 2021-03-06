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


#ifndef _CUTIL_SOCKET_
#define _CUTIL_SOCKET_

#include <cutil/AbstractInputStream.h>
#include <cutil/AbstractOutputStream.h>

#include <cutil/InetException.h>
#include <cutil/SocketException.h>

#include <memory>

namespace cutil
{
	class InetAddress ;

	/**
	 * A Client Socket, or simply, A Socket. Sockets provide an endpoint for communication between two machines or processes.
	 *
	 * This implementation currently handles PF_INET, IPv4 Internel Protocol communication domain.
	 *
	 */
	class Socket : public AbstractInputStream, public AbstractOutputStream
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * Create an initially unconnected socket.
			 *
			 */
			Socket() ;

			/**
			 * Creates a Socket and connects to the specifed port and host.
			 *
			 * @param host the host to connect to.
			 * @param port the remote port number
			 */
			Socket(const InetAddress& host, int port) throw(SocketException) ;

			/**
			 * Creates a Socket and connects to the specifed port and host.
			 *
			 * @param host the host to connect to.
			 * @param port the remote port number
			 */
			Socket(const std::string& host, int port) throw(InetException, SocketException) ;

			/**
			 * Creates a Socket from the specified file descriptor.
			 * This method acts as a wrapper around a constructed socket descriptor.
			 * This method is intended as a convenient constructor for the creation of a Socket to 'wrap'
			 * the file descriptor returned from a successful call to accept of a ServerSocket
			 *
			 * @param fd the file descriptor used for socket communications
			 * @throws SocketException if fd is not a valid socket file descriptor
			 */
			Socket(int fd) throw(SocketException) ;

			/**
			 * Destructor.
			 *
			 */
			virtual ~Socket() ;

			//-------------------------------------------------------------------------------//
			// Socket Operations

			/**
			 * Connect this socket to the server.
			 *
			 * @param host the host to connect to
			 * @param port the port number to connect to.
			 */
			void connect(const InetAddress& host, int port) throw(SocketException) ;

			/**
			 * Close this socket
			 *
			 * @throw SocketException if an error occurs during the closing of this Socket
			 */
			void close() throw(SocketException) ;

			/**
			 * Returns the remote host to which this Socket is conencted
			 * If this Socket is not yet connected, 0 will be returned
			 *
			 * @return the host to which this Socket is connected, or 0 if this socket is not yet connected
			 */
			std::auto_ptr<InetAddress> getInetAddress() const ;

			/**
			 * Returns the remote port number to which this Socket is connected.
			 * If this Socket is not yet connected, -1 will be returned
			 *
			 * @return the remote port number to which this Socket is connected, or -1 if this Socket is not yet connected
			 */
			int getPort() const ;

			/**
			 * Returns the local address to which this Socket is bound.
			 * This may also known as the address of the peer socket
			 * If this Socket is not yet connected, 0 will be returned
			 *
			 * @return the local address to which this Socket is connected, or 0 if this socket is not yet bound
			 */
			std::auto_ptr<InetAddress> getLocalAddress() const ;

			/**
			 * Returns the local port number to which this Socket is bound
			 * This may also known as the port number of the peer socket
			 * If this Socket is not yet connected, -1 will be returned
			 *
			 * @return the local port number to which this Socket is bound, or -1 if this Socket is not yet bound
			 */
			int getLocalPort() const ;

			/**
			 * Returns the Socket Descriptor of this Socket
			 *
			 * @return the socket descriptor of this socket
			 */
			int getSocketDescriptor() const ;

			/**
			 * Returns whether this Socket is connected.
			 *
			 * @return true if this Socket is connected, false otherwise
			 */
			bool isConnected() const ;

			/**
			 * Retuns whether this Socket is closed
			 *
			 * @return true if this Socket has been closed, false otherwise
			 */
			bool isClosed() const ;

			/**
			 * Returns whether the Input or read part of this Socket has been shutdown
			 *
			 * @return true if the read part of this Socket connection has been shutdown
			 */
			bool isInputShutdown() const ;

			/**
			 * returns whether the output or write part of this Socket has been shutdown
			 *
			 * @return true if the write part of this Socket connec5tion has been shutdown
			 */
			bool isOutputShutdown() const ;

			/**
			 * Shutdown the Input Stream of this socket.
			 *
			 */
			void shutdownInput() throw(SocketException) ;

			/**
			 * Shutdown the Output Stream of this socket.
			 *
			 */
			void shutdownOutput() throw(SocketException) ;

			//-------------------------------------------------------------------------------//
			// AbstractInputStream

			virtual bool isDataAvailable(long usec) const throw(Exception) ;

			virtual ssize_t read(void* buf, size_t length) const throw(Exception) ;

			virtual ssize_t read(void* buf, size_t length, int& err_code) const throw() ;

			virtual ssize_t read(char& read_byte) throw(Exception) ;

			virtual ssize_t read(char& read_byte, int& err_code) throw() ;

			//-------------------------------------------------------------------------------//
			// AbstractOutputStream

			virtual ssize_t write(const void* data, size_t size) throw(Exception) ;

			virtual ssize_t write(const void* data, size_t size, int& err_code) throw() ;

			virtual ssize_t write(const char& write_byte) throw(Exception) ;

			virtual ssize_t write(const char& write_byte, int& err_code) throw() ;

			//-------------------------------------------------------------------------------//

			/** Default max size for read buffer during read from socket */
			static const int DEFAULT_MAXRECV ;

			//-------------------------------------------------------------------------------//

		protected:

			//-------------------------------------------------------------------------------//

		private:

			/**
			 * Dis-allow Copy constructor
			 *
			 */
			Socket(const Socket&) : AbstractInputStream(), AbstractOutputStream() {} 

			/**
			 * Creates the Socket file descriptor
			 *
			 * @return the file descriptor for the newly created socket
			 * @throw SocketException if there is an error creating the socket
			 */
			int createSocketFd() throw(SocketException) ;

			/** The socket descriptor */
			int m_socket_descriptor ;

			/** indicates if this Socket is connected to a remote host */
			bool m_connected ;

			/** indicates that this socket has been closed */
			bool m_closed ;

			/** indicates that the Input, or read end of this Socket has been shutdown */
			bool m_input_shutdown ;

			/** indicates that the Output, or write end of this Socket has been shutdown */
			bool m_output_shutdown ;


	} ; /* class Socket */

} /* namespace cutil */


#endif /* _CUTIL_SOCKET_ */

