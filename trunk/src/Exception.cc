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

#include <cutil/Exception.h>

using namespace cutil ;

//-------------------------------------------------------------------------------//
// Top Level Exception Class.

/**
 * Construct a new Exception woth the specified error message.
 *
 * @param message the error message
 */
Exception::Exception(std::string message) : theMessage(message)
{}

/**
 * Destructor
 */
Exception::~Exception() throw()
{}

/**
 * Returns the error message as a char*
 *
 * @return the error message as a char*
 */
const char* Exception::what() const throw()
{
	return(theMessage.c_str()) ;
}
/**
 * Returns the message of this Exception as a std::string
 *
 * @return the error message as std::string
 */
std::string Exception::toString()
{
	return(theMessage) ;
}

