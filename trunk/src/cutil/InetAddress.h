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


#ifndef _CUTIL_INETADDRESS_
#define _CUTIL_INETADDRESS_

#include <cutil/InetException.h>

#include <netinet/in.h>

#include <string>

namespace cutil
{
	/**
	 * InetAddress represents an Internet Protocol Version 4 (IPv4) address.
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class InetAddress
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * Constructs a new InetAddress initialized to the local host
			 *
			 */
			InetAddress() throw(InetException) ;

			/**
			 * Constructs a new InetAddress for the specified host.
			 * The host address may be specified either as a textual representation of an IP address or as
			 * the machine, or network name e.g. www.somehost.co.uk.
			 * If a host name is specified, an attempt is made to lookup the IP address of the hostm, and will
			 * result in an InetException if the specified host cannot be found. If an IP address is specified,
			 * it is checked for correct formatting only.
			 *
			 * @param host the hostname or ip address
			 * @throw InetException if no IP address could be found for a specified host name, or an IP address
			 *                      is specified in a invalid format.
			 */
			InetAddress(const std::string& host) throw(InetException) ;

			/**
			 * Constructs a new InetAddress from a low level network address structure
			 *
			 * @param addr the low level network address struct
			 */
			InetAddress(const struct in_addr& addr) ;

			/**
			 * Destructor.
			 *
			 */
			virtual ~InetAddress() ;

			//-------------------------------------------------------------------------------//
			//

			/**
			 * Returns the IP address of this InetAddress in string form
			 *
			 * @return the IP address of this InetAdress as a string
			 */
			std::string getHostAddress() const throw(InetException) ;

			/**
			 * Returns the host name of this InetAddress
			 * If the host name of this InetAddress cannot be found, a textual representation of its
			 * IP address wil be returned instead
			 *
			 * @return the host name of this InetAddress
			 */
			std::string getHostName() const ;

			/**
			 * Returns the low level network address of this InetAddress
			 *
			 * @return the low level network address of this InetAddress
			 */
			struct in_addr getAddress() const ;


			//-------------------------------------------------------------------------------//
			// static methods

			/**
			 * Determines if the specified ipAddress is a valid IPv4 IP Address
			 * A valid IPv4 Address is in dot decimal notation "ddd.ddd.ddd.ddd" where
			 * 0 <= ddd <= 255.
			 *
			 * @return true if the specified address is a valid IP address, false otherwise
			 */
			static bool isIpAddress(const std::string& ipAddress) ;


			//-------------------------------------------------------------------------------//

		protected:

			/**
			 * Initializes this InetAddress from an IpAddress
			 *
			 * @param ipAddress the address to initialize this InetAddress
			 * @throw InetException if an invalid address is specified.
			 */
			void initializeFromIpAddress(const std::string& ipAddress) throw(InetException) ;

			/**
			 * Initializes this InetAddress from a host name
			 *
			 * @param host the hostname to initialize this InetAddress
			 * @throw InetException if the specified hostname cannot be resolved
			 */
			void initializeFromHostName(const std::string& host) throw(InetException) ;


			/** low level struct containing the ip address of this InetAddress */
			struct in_addr theInetAddress ;

			/** stores the hostname of this InetAddress if created with by name */
			std::string theHostName ;

			//-------------------------------------------------------------------------------//

		private:

	} ; /* class Socket */

} /* namespace cutil */


#endif /* _CUTIL_INETADDRESS_ */

