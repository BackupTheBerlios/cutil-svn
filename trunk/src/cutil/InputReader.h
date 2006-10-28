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

#ifndef _CUTIL_INPUTREADER_
#define _CUTIL_INPUTREADER_

#include <string>

namespace cutil
{
	class AbstractInputStream ;

	/**
	 * InputReader provides a convenient way of reading data from a wrapped AbstractInputStream.
	 *
	 */
	class InputReader
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * constructs a new InputReader wrapping the specified AbstractInputStream
			 *
			 * @param input an AbstractInputStream from which to read data
			 */
			InputReader(AbstractInputStream& input) ;
			virtual ~InputReader() ;

			//-------------------------------------------------------------------------------//
			// InputReader Operations

			/**
			 * Reads a size_t from the underlying AbstractInputStream.
			 * size_t's are expected to be encoded to a string representation as written by an
			 * OutputWritter (BufferedOutputWriter). The string representation is read from
			 * the underlying AbstractInputStream, converted back to a size_t and returned.
			 *
			 * @return size_t read.
			 */
			size_t readSize() const ;

			/**
			 * Reads a string from the underlying AbstractInputStream.
			 * This method expects the string to be prefixed with the size of the string
			 * written as a size_t. The length of the string is first read, then a 
			 * string of this length is read and returned.
			 *
			 * @return the read string
			 * @see readString(size_t)
			 */
			std::string readString() const ;

			/**
			 * Reads a string of length from the underlying AbstractInputStream.
			 *
			 * @param length length of the string to be read.
			 * @return the read string
			 * @see readString()
			 */
			std::string readString(size_t length) const ;

			//-------------------------------------------------------------------------------//

		protected:

			//-------------------------------------------------------------------------------//

		private:
			/**
			 * Dis-allow Copy constructor
			 *
			 */
			InputReader(const InputReader& ir) : m_input(ir.m_input) {} ;

			/** wrapped AbstractInputStream from which data is read from */
			AbstractInputStream& m_input ;

	} ; /* class InputReader */

} /* namespace cutil */

#endif /* _CUTIL_INPUTREADER_ */

