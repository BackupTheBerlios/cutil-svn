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

#ifndef _CUTIL_SERVERSOCKET_
#define _CUTIL_SERVERSOCKET_

#include <cutil/SocketException.h>

#include <memory>

namespace cutil
{
	class InetAddress ;
	class Socket ;

	/**
	 * A Server Socket.
	 * A Server Socket is responsible for waiting on connection requests from Client Sockets over a network
	 * and then doing something useful with the request.
	 *
	 * The ServerSocket typically starts in an UNBOUND_ENUM, or BOUND_ENUM state. The user must bind the
	 * ServerSocket to a specified port (if not constructed already bound). listen() must then be called
	 * to indicate the willingness of this ServerSocket to accept connections. The ServerSocket can then
	 * be placed in a wait state to wait for incomming connections by calling accept()
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class ServerSocket
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * Constructs an unbound Server Socket
			 *
			 */
			ServerSocket() ;

			/**
			 * Creates a Server socket that is bound to the specified port.
			 * After construction, the server socket is bound and in the listening state
			 *
			 * @param port the port number to bind to
			 * @throws SocketException if there is an error binding the socket
			 */
			ServerSocket(int port) throw(SocketException) ;

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
			ServerSocket(int port, int backlog) throw(SocketException) ;

			/**
			 * Server Socket Destructor
			 * If this Server Socket is still connected, the Socket will be shutdown
			 * and closed
			 *
			 */
			~ServerSocket() ;

			//-------------------------------------------------------------------------------//
			// ServerSocket Operations

			/**
			 * Listens for connection being made on this socket and accepts them.
			 * This method will block until a connection is accepted
			 *
			 * @return the newly accepted socket, or 0 on failure
			 * @throw SocketException if there is an error during the accept
			 */
			std::auto_ptr<Socket> accept() throw(SocketException) ;

			/**
			 * Binds this socket to a local address
			 *
			 * @param host the host IP Address
			 * @param port the port number
			 * @throws SocketException if there is an error during binding
			 */
			void bind(const InetAddress* host, int port) throw(SocketException) ;

			/**
			 * Close this socket
			 *
			 * @throw SocketException if an error occurs during the closing of this ServerSocket
			 */
			void close() throw(SocketException) ;

			/**
			 * Returns the local InetAddress of this ServerSocket
			 * If this ServerSocket is not yet bound, this method will return 0
			 *
			 * @return the local InetAddress of this ServerSocket
			 */
			std::auto_ptr<InetAddress> getInetAddress() const ;

			/**
			 * Returns the local port to which this ServerSocket is bound
			 * If this ServerSocket is not yet bound, -1 will be returned
			 *
			 * @return the local port number to which this socket is bound, or -1 if this ServerSocket is not yet bound
			 */
			int getPort() const ;

			/** ServerSocket states */
			enum ServerSocketStateEnum { UNBOUND_ENUM, BOUND_ENUM, LISTENING_ENUM, CLOSED_ENUM } ;

			/**
			 * Returns the current state of this ServerSocket
			 *
			 * @return the current state of this ServerSocket
			 */
			ServerSocketStateEnum getState() const ;

			/**
			 * Sets the willingness of this socket to accept connections
			 * This ServerSocket will have its backlog value set to DEFAULT_BACKLOG
			 * @throw SocketException if there is an error on the listen procedure
			 */
			void listen() throw(SocketException) ;

			/**
			 * Sets the willingness of this socket to accept connections
			 *
			 * @param backlog the maximum incomming connection queue length
			 * @throw SocketException if there is an error on the listen procedure
			 */
			void listen(int backlog) throw(SocketException) ;

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
			void setReuseAddress(bool yn) throw(SocketException) ;

			/**
			 * Returns whether SO_REUSEADDR is enabled upon this ServerSocket
			 *
			 * @return true if SO_REUSEADDR is enabled upon this ServerSocket, false otherwise
			 */
			bool getReuseAddress() const ;


			//-------------------------------------------------------------------------------//

			/** Default backlog value */
			static const int DEFAULT_BACKLOG = 5 ;

			//-------------------------------------------------------------------------------//

		protected:

			//-------------------------------------------------------------------------------//

		private:
			/**
			 * Dis-allow Copy constructor
			 */
			ServerSocket(const ServerSocket& ss) {} ;

			/**
			 * Creates the ServerSocket file descriptor
			 *
			 * @return the file descriptor for the newly created socket
			 * @throw SocketException if there is an error creating the socket
			 */
			int createServerSocketFd() throw(SocketException) ;

			/** The socket descriptor */
			int theSocketDescriptor ;

			/** indicate sthe current state of this ServerSocket */
			ServerSocketStateEnum theState ;

	} ; /* class ServerSocket */

} /* namespace cutil */

#endif /* _CUTIL_SERVERSOCKET_ */
