/*
 * Copyright (C) 2006  Colin Law
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
 * $Id$
 */

#include <cutil/InputReader.h>

#include <cutil/AbstractInputStream.h>
#include <cutil/Conversion.h>

#include <cstring>

using cutil::InputReader ;

//-------------------------------------------------------------------------------//
// Constructor / Desctructor

/**
 * constructs a new InputReader wrapping the specified AbstractInputStream
 *
 * @param input an AbstractInputStream from which to read data
 */
InputReader::InputReader(AbstractInputStream& input) : m_input(input)
{
}

InputReader::~InputReader()
{
}

/**
 * Reads a size_t from the underlying AbstractInputStream.
 * size_t's are expected to be encoded to a string representation as written by an
 * OutputWritter (BufferedOutputWriter). The string representation is read from
 * the underlying AbstractInputStream, converted back to a size_t and returned.
 *
 * @return size_t read.
 */
size_t
InputReader::readSize() const
{
	char buf[32] ;
	// using 20 characters as the maximum number of characters a 64-bit interger could hold
	m_input.read(buf, 20) ;

	return(cutil::Conversion::fromString<size_t>(std::string(buf))) ;
}

/**
 * Reads a string from the underlying AbstractInputStream.
 * This method expects the string to be prefixed with the size of the string
 * written as a size_t. The length of the string is first read, then a 
 * string of this length is read and returned.
 *
 * @return the read string
 * @see readString(size_t)
 */
std::string
InputReader::readString() const
{
	const size_t length = readSize() ;
	return(readString(length)) ;
}

/**
 * Reads a string of length from the underlying AbstractInputStream.
 *
 * @param length length of the string to be read.
 * @return the read string
 * @see readString()
 */
std::string
InputReader::readString(size_t length) const
{
	const size_t num_chars = length + 1 ;
	char* buf = new char[num_chars] ;
	memset(buf, '\0', num_chars) ;

	try
	{
		m_input.read(buf, length) ;
	}
	catch(Exception& e)
	{
		// cleanup and rethrow
		delete[] buf ;
		throw ;
	}

	std::string s(buf) ;
	delete[] buf ;

	return(s) ;
}
