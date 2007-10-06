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


#ifndef _CUTIL_ABSTRACTINPUTSTREAM_
#define _CUTIL_ABSTRACTINPUTSTREAM_

#include <cutil/Exception.h>

namespace cutil
{
	/**
	 * AbstractInputStream provides an abstract interface for reading data.
	 * Typically, an AbstractInputStream will be wrapped by an InputWriter
	 * to provide a convenient method of reading data.
	 *
	 */
	class AbstractInputStream
	{
		public:
			virtual ~AbstractInputStream() {}

			/**
			 * Checks to see if data is availble for reading on this AbstractInputStream.
			 * This method will block for usec, waiting for data to become available for reading
			 * ie a call to read would not block.
			 *
			 * @param usec the timeout value
			 * @return true if data is availble for reading upon this socket
			 *         false otherwise
			 */
			virtual bool isDataAvailable(long usec) const throw(Exception) = 0 ;

			/**
			 * Reads the specified number of bytes from this AbstractInputStream into buf.
			 * If the number of bytes read is 0, the End-of-File has been reached.
			 * The number of bytes read is returned.
			 * 
			 * @param buf buffer into which the data should be read into
			 * @param length number of bytes to be read
			 * @return the number of bytes read.
			 * @throw Exception if there is an error reading data.
			 */
			virtual ssize_t read(void* buf, size_t length) const throw(Exception) = 0 ;

			/**
			 * Reads the specified number of bytes from this AbstractInputStream into into buf.
			 * This method does not throw any Exception and is intended as a real-time call in
			 * a non-blocking state (although can be used anywhere)
			 * The return value indicates the return satus of the underlying call, on sucess this
			 * will be the number of bytes actually written, on failure err_code will be set to the error
			 * code of the underlying call, see read(2) for definitions of the return value and error codes.
			 *
			 * @param buf buffer into which the data should be read into
			 * @param length number of bytes to be read
			 * @param err_code set to the errno of the underlying call
			 * @return the return status of the underlying call
			 */
			virtual ssize_t read(void* buf, size_t length, int& err_code) const throw() = 0 ;

			/**
			 * Attempts to read a byte from this AbstractInputStream.
			 * when no data is available false will be returned.
			 *
			 * @param read_byte set to the byte read
			 * @return true if a byte was read, false otherwise
			 * @throw Exception on read error
			 */
			virtual ssize_t read(char& read_byte) throw(Exception) = 0 ;

			/**
			 * Attempts to read a byte from this AbstractInputStream.
			 * This method does not throw any Exception and is intended as a real-time call in
			 * a non-blocking state (although can be used anywhere)
			 * The return value indicates the return satus of the underlying call, on failure
			 * err_code will be set to the error code of the underlying call, see read(2) for
			 * definitions of the return value and error codes.
			 *
			 * @param read_byte set to the byte read
			 * @param err_code set to the errno of the underlying call
			 * @return the return status of the underlying call
			 */
			virtual ssize_t read(char& read_byte, int& err_code) throw() = 0 ;

			//-------------------------------------------------------------------------------//

		protected:

			/**
			 * protected constructor
			 *
			 */
			AbstractInputStream() {}

			//-------------------------------------------------------------------------------//

		private:

			/**
			 * Dis-allow Copy constructor
			 *
			 */
			AbstractInputStream(const AbstractInputStream&) {} ;

	} ; /* class AbstractInputStream */

} /* namespace cutil */


#endif /* _CUTIL_ABSTRACTINPUTSTREAM_ */

