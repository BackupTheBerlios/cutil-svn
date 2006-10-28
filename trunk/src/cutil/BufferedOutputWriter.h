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


#ifndef _CUTIL_BUFFEREDOUTPUTWRITER_
#define _CUTIL_BUFFEREDOUTPUTWRITER_

#include <string>
#include <vector>

namespace cutil
{
	class AbstractOutputStream ;

	/**
	 * BufferedOutputWriter provides a convenient interface for writing to an AbstractOutputStream.
	 * Data is written into an internal buffer within the BufferedOutputWriter. Once a block of
	 * data is prepared by the client, if can be written to the wrapped AbstractOutputStream.
	 *
	 */
	class BufferedOutputWriter
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * constructs a new BufferedOutputWriter wrapping the specified AbstractOutputStream
			 *
			 * @param output an AbstractOutputStream to which data is written
			 */
			BufferedOutputWriter(AbstractOutputStream& output) ;
			virtual ~BufferedOutputWriter() ;

			//-------------------------------------------------------------------------------//
			// BufferedOutputWriter Operations

			/**
			 * Clears the current contents of the internal output buffer
			 *
			 */
			void clear() ;

			/**
			 * returns the size of the currently composed output data.
			 * The size of currently composed output data is the total of all data written to this
			 * OutputWriter since it was last cleared.
			 *
			 * @return the size of the currently composed output data.
			 */
			size_t getSize() const ;

			/**
			 * writes the specfied size value into this OutputWriter
			 * size_t data types are first converted to a string representation
			 * before being written to this OutputWriter.
			 *
			 * @param val the size value to write to this OutputWriter
			 */
			void write(const size_t& val) ;

			/**
			 * Writes s into this OutputWriter.
			 * The size of s is first written so that an InputReader can determine the
			 * number of characters within s.
			 *
			 * @param s the string to be written to this OutputWriter
			 * @see writeRaw(const std::string&)
			 */
			void write(const std::string& s) ;

			/**
			 * Writes s into this OutputWriter.
			 * This method writes s to this OutputWriter without prepending the size of the string.
			 *
			 * @param s the string to be written to this OutputWriter
			 */
			void writeRaw(const std::string& s) ;

			/**
			 * writes the contents of this BufferedOutputWriters internal buffer to the OutputStream
			 * Note, after writing, the contents of this BufferedOutputWriters internal buffer is
			 * not cleared.
			 *
			 * @return the number of bytes written
			 */
			ssize_t writeToStream() const ;

			/**
			 * writes the contents of this BufferedOutputWriter to the OutputStream
			 * After writing, the contents of this BufferedOutputWriter are cleared.
			 *
			 * @return the number of bytes written
			 */
			ssize_t flushToStream() ;

			//-------------------------------------------------------------------------------//

		protected:

			//-------------------------------------------------------------------------------//

		private:
			/**
			 * Dis-allow Copy constructor
			 *
			 */
			BufferedOutputWriter(const BufferedOutputWriter& b) : m_output(b.m_output) {} ;

			/**
			 * Wtites the specified string into this BufferedOutputWriters internal buffer
			 *
			 * @param s the string ti be written
			 */
			void writeStringToBuffer(const std::string& s) ;

			/** wrapped AbstractOutputStream to which data will be written */
			AbstractOutputStream& m_output ;

			/** internal data buffer into which data is written before sent to the AbstractOutputStream */
			std::vector<char> m_output_buffer ;

	} ; /* class BufferedOutputWriter */

} /* namespace cutil */


#endif /* _CUTIL_BUFFEREDOUTPUTWRITER_ */

