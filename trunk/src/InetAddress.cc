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

#include <cutil/InetAddress.h>
#include <cutil/InetException.h>

#include <netdb.h>

#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <cerrno>
#include <string>

using cutil::InetAddress ;
using cutil::InetException ;

//-------------------------------------------------------------------------------//
// Constructor / Desctructor

/**
 * Constructs a new InetAddress initialized to the local host
 *
 */
InetAddress::InetAddress() throw(InetException)
{
	initializeFromIpAddress("127.0.0.1") ;
}

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
InetAddress::InetAddress(const std::string& host) throw(InetException)
{
	if(isIpAddress(host))
	{
		initializeFromIpAddress(host) ;
	}
	else
	{
		initializeFromHostName(host) ;
	}
}

/**
 * Constructs a new InetAddress from a low level network address structure
 *
 * @param addr the low level network address struct
 */
InetAddress::InetAddress(const struct in_addr& addr)
{
	theHostName = "" ;
	theInetAddress = addr ;
}

/**
 * Destructor.
 *
 */
InetAddress::~InetAddress()
{

}

//-------------------------------------------------------------------------------//
//

/**
 * Returns the IP address of this InetAddress in string form
 *
 * @return the IP address of this InetAdress as a string
 */
std::string
InetAddress::getHostAddress() const throw(InetException)
{
	std::string ret = "" ;

	char host[INET_ADDRSTRLEN] ;
	if(::inet_ntop(AF_INET, &theInetAddress, host, INET_ADDRSTRLEN) == 0)
	{
		switch(errno)
		{
			case EAFNOSUPPORT:
			{
				throw(InetException("Invalid address family was specified")) ;
				break ;
			}
			case ENOSPC:
			{
				throw(InetException("The converted address string would exceed the specified destination space")) ;
				break ;
			}
			default:
			{
				throw(InetException("An unspecified error occured")) ;
			}
		}
	}
	else
	{
		ret = std::string(host) ;
	}

	return(ret) ;
}

/**
 * Returns the host name of this InetAddress
 * If the host name of this InetAddress cannot be found, a textual representation of its
 * IP address wil be returned instead
 *
 * @return the host name of this InetAddress
 */
std::string
InetAddress::getHostName() const
{
	std::string host = "" ;

	if(!theHostName.empty())
	{
		host = theHostName ;
	}
	else
	{
		struct sockaddr_in sa ;
		sa.sin_family = AF_INET ;
		sa.sin_port = 0 ;
		sa.sin_addr = theInetAddress ;

		char hostc[NI_MAXHOST] ;
		if(::getnameinfo(reinterpret_cast<struct sockaddr*>(&sa), sizeof(sa), hostc, sizeof(hostc), 0, 0, NI_NAMEREQD) != 0)
		{
			//cannot get the hostname, so we return the textual ip address instead
			host = getHostAddress() ;
		}
		else
		{
			host = std::string(hostc) ;
		}
	}

	return(host) ;
}

/**
 * Returns the low level network address of this InetAddress
 *
 * @return the low level network address of this InetAddress
 */
struct in_addr
InetAddress::getAddress() const
{
	return(theInetAddress) ;
}


//-------------------------------------------------------------------------------//
// static methods

/**
 * Determines if the specified ipAddress is a valid IPv4 IP Address
 * A valid IPv4 Address is in dot decimal notation "ddd.ddd.ddd.ddd" where
 * 0 <= ddd <= 255.
 *
 * @return true if the specified address is a valid IP address, false otherwise
 */
bool
InetAddress::isIpAddress(const std::string& ipAddress)
{
	bool ret = false ;

	// break the addres into exactly 4 segments
	// tokenize upon the '.'
	std::string segment[4] ;
	std::string::size_type marker = 0 ;

	// we loop for the first three segments, then the remaining chars are the 4th segment
	unsigned int dotCount = 0 ;
	for(int i = 0; i < 3; i++)
	{
		std::string::size_type pos = ipAddress.find('.', marker) ;

		if(pos != std::string::npos)
		{
			dotCount++ ;
			segment[i] = ipAddress.substr(marker, pos - marker) ;

			marker += (pos - marker) ;
			marker++ ; // skip over the '.'
		}
		else
		{
			// not enough dots, doesn't sound like a valid IPv4 address
			break ;
		}
	}

	if(dotCount == 3)
	{
		// populate the 4th segment with the remaining text
		segment[3] = ipAddress.substr(marker, ipAddress.length() - marker) ;

		// now check each segment is 1,2 or 3 digit characters only, and that when the chars are combined
		// they represent a number between 0 and 255 inclusive
		unsigned int counter = 0 ;
		bool valid = true ;

		while((valid) && (counter < 4))
		{
			if(segment[counter].length() <= 3)
			{
				std::string s = segment[counter] ;

				for(std::string::size_type j = 0; j < s.size(); j++)
				{
					if(!isdigit(s[j]))
					{
						// not a digit
						valid = false ;

						// break from the for loop
						break ;
					}
				}

				// it was all digits, right?
				if(valid)
				{
					// get the int value of the chars
					int val = atoi(s.c_str()) ;
					if((val < 0) || (val > 255))
					{
						valid = false ;
					}
				}
			}
			else
			{
				// too many characters in the segment
				valid = false ;
			}

			// next segment
			counter++ ;
		}

		ret = valid ;
	}
	else
	{
		// not enough segments
		ret = false ;
	}

	return(ret) ;
}



//-------------------------------------------------------------------------------//
// protected methods

/**
 * Initializes this InetAddress from an IpAddress
 *
 * @param ipAddress the address to initialize this InetAddress
 * @throw InetException if an invalid address is specified.
 */
void
InetAddress::initializeFromIpAddress(const std::string& ipAddress) throw(InetException)
{
	theHostName = "" ;
	int ret = ::inet_pton(AF_INET, ipAddress.c_str(), &theInetAddress) ;

	if(ret < 0)
	{
		int errcpy = errno ;

		// clear the in_addr struct tl leave us in a sensible state
		memset(&theInetAddress, 0, sizeof(theInetAddress)) ;

		switch(errcpy)
		{
			case EAFNOSUPPORT:
			{
				throw(InetException("An invalid address family was specified in the call to inet_pton")) ;
				break ;
			}
			default:
			{
				throw(InetException("An unspecified error occured during the call to inet_pton")) ;
			}
		}
	}
	else if(ret == 0)
	{
		throw(InetException("An invalid network address was specified")) ;
	}
}

/**
 * Initializes this InetAddress from a host name
 *
 * @param host the hostname to initialize this InetAddress
 * @throw InetException if the specified hostname cannot be resolved
 */
void
InetAddress::initializeFromHostName(const std::string& host) throw(InetException)
{
	struct addrinfo hints, *res ;

	memset(&hints, 0, sizeof(hints)) ;
	hints.ai_family = PF_INET ;
	hints.ai_socktype = SOCK_STREAM ;

	if(::getaddrinfo(host.c_str(), 0, &hints, &res) != 0)
	{
		theHostName = "" ;
		memset(&theInetAddress, 0, sizeof(theInetAddress)) ;

		throw(InetException("Unknown Host Name")) ;
	}
	else
	{
		// @note[explanation] getaddrinfo may return more then one addrinfo
		//      we assume here that only the first returned is required

		// extract the addr_in struct from the returned info as the addr_in for this InetAddres
		theInetAddress = (reinterpret_cast<struct sockaddr_in*>(res->ai_addr))->sin_addr ;

		// @note[explanation] keep a copy of the specified host name so that a call to getHostName
		//      returns the name this InetAdress was created with,
		theHostName = host ;
	}

	freeaddrinfo(res) ;
}

