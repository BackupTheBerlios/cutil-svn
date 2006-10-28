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


#ifndef _CUTIL_ABSTRACTOUTPUTSTREAM_
#define _CUTIL_ABSTRACTOUTPUTSTREAM_

#include <cutil/Exception.h>

namespace cutil
{
	/**
	 * AbstractOutputStream provides an abstract interface for writing data.
	 * Typically, an AbstractOutputStream will be wrapped by an OutputWriter
	 * to provide a convenient method of writing data.
	 *
	 */
	class AbstractOutputStream
	{
		public:

			virtual ~AbstractOutputStream() {}

			/**
			 * Writes the specified number of bytes to this AbstractOutputStream
			 *
			 * @param data the data to be written to the socket
			 * @param size the size of the data
			 * @return the number of bytes actually written to the AbstractOutputStream
			 * @throw Exception if there is an error writing to the AbstractOutputStream
			 */
			virtual ssize_t write(const void* data, size_t size) throw(Exception) = 0 ;

			/**
			 * Writes the specified number of bytes to this AbstractOutputStream.
			 * This method does not throw any exception and is intended as a real-time call in a non-blocking state.
			 * The return value indicates the status of the underlying write call. on sucess this
			 * will be the number of bytes actually written, on failure err_code will be set to the error
			 * code of the underlying call. see write(2) for definitions of the return value and error codes
			 *
			 * @param data the data to be written to the socket
			 * @param size the size of the data to be written
			 * @param err_code set to errno of the underlying call on error
			 * @return the return status of the underlying call
			 */
			virtual ssize_t write(const void* data, size_t size, int& err_code) throw() = 0 ;

			/**
			 * Writes the specified char to this AbstractOutputStream
			 *
			 * @param write_byte the byte to be written
			 * @throw Exception on error
			 */
			virtual ssize_t write(const char& write_byte) throw(Exception) = 0 ;

			/**
			 * Writes the specified char to this AbstractOutputStream.
			 * This method does not throw any exception and is intended as a real-time call in a non-blocking state.
			 * The return value indicates the status of the underlying write call, on failure
			 * err_code will be set to the error code of the underlying call. see write(2) for
			 * definitions of the return value and error codes
			 *
			 * @param write_byte the byte to be written
			 * @param err_code set to errno of the underlying call on error
			 * @return the return status of the underlying call
			 */
			virtual ssize_t write(const char& write_byte, int& err_code) throw() = 0 ;

			//-------------------------------------------------------------------------------//

		protected:

			/**
			 * protected constructor
			 *
			 */
			AbstractOutputStream() {}

			//-------------------------------------------------------------------------------//

		private:

			/**
			 * Dis-allow Copy constructor
			 *
			 */
			AbstractOutputStream(const AbstractOutputStream& s) {} ;

	} ; /* class AbstractOutputStream */

} /* namespace cutil */


#endif /* _CUTIL_ABSTRACTOUTPUTSTREAM_ */

