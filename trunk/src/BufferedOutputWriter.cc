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

#include <cutil/BufferedOutputWriter.h>

#include <cutil/AbstractOutputStream.h>
#include <cutil/Conversion.h>

using cutil::BufferedOutputWriter ;

//-------------------------------------------------------------------------------//
// Constructor / Desctructor

/**
 * constructs a new BufferedOutputWriter wrapping the specified AbstractOutputStream
 *
 * @param output an AbstractOutputStream to which data is written
 */
BufferedOutputWriter::BufferedOutputWriter(AbstractOutputStream& output)
		: m_output(output)
{
}

BufferedOutputWriter::~BufferedOutputWriter()
{
}

//-------------------------------------------------------------------------------//
// OutputWriter Operations

/**
 * Clears the current contents of the internal output buffer
 *
 */
void
BufferedOutputWriter::clear()
{
	m_output_buffer.clear() ;
}

/**
 * returns the size of the currently composed output data.
 * The size of currently composed output data is the total of all data written to this
 * OutputWriter since it was last cleared.
 *
 * @return the size of the currently composed output data.
 */
size_t
BufferedOutputWriter::getSize() const
{
	return(m_output_buffer.size()) ;
}

/**
 * writes the specfied size value into this OutputWriter
 * size_t data types are first converted to a string representation
 * before being written to this OutputWriter.
 *
 * @param val the size value to write to this OutputWriter
 */
void
BufferedOutputWriter::write(const size_t& val)
{
	// using 20 characters as the maximum number of characters a 64-bit interger could hold
	writeStringToBuffer(cutil::Conversion::toString(val, '0', 20)) ;
}

/**
 * Writes s into this OutputWriter.
 * The size of s is first written so that an InputReader can determine the
 * number of characters within s.
 *
 * @param s the string to be written to this OutputWriter
 * @see writeRaw(const std::string&)
 */
void
BufferedOutputWriter::write(const std::string& s)
{
	write(s.size()) ;
	writeStringToBuffer(s) ;
}

/**
 * Writes s into this OutputWriter.
 * This method writes s to this OutputWriter without prepending the size of the string.
 *
 * @param s the string to be written to this OutputWriter
 */
void
BufferedOutputWriter::writeRaw(const std::string& s)
{
	writeStringToBuffer(s) ;
}

/**
 * writes the contents of this BufferedOutputWriters internal buffer to the OutputStream
 * Note, after writing, the contents of this BufferedOutputWriters internal buffer is
 * not cleared.
 *
 * @return the number of bytes written
 */
ssize_t
BufferedOutputWriter::writeToStream() const
{
	char* buf = new char[m_output_buffer.size()] ;
	int pos = 0;
	for(std::vector<char>::const_iterator citer = m_output_buffer.begin(); citer != m_output_buffer.end(); ++citer)
	{
		buf[pos++] = *citer ;
	}

	ssize_t written = 0 ;
	try
	{
		written = m_output.write(buf, m_output_buffer.size()) ;
		delete[] buf ;
	}
	catch(Exception& e)
	{
		// cleanup and rethrow
		delete[] buf ;
		throw ;
	}

	return(written) ;
}

/**
 * writes the contents of this BufferedOutputWriter to the OutputStream
 * After writing, the contents of this BufferedOutputWriter are cleared.
 *
 * @return the number of bytes written
 */
ssize_t
BufferedOutputWriter::flushToStream()
{
	ssize_t written = writeToStream() ;
	clear() ;
	return(written) ;
}

/**
 * Wtites the specified string into this BufferedOutputWriters internal buffer
 *
 * @param s the string ti be written
 */
void
BufferedOutputWriter::writeStringToBuffer(const std::string& s)
{
	for(std::string::const_iterator citer = s.begin(); citer != s.end(); ++citer)
	{
		m_output_buffer.push_back(*citer) ;
	}
}
