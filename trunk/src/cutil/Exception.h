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


#ifndef _CUTIL_EXCEPTION_H_
#define _CUTIL_EXCEPTION_H_

#include <exception>
#include <string>

namespace cutil
{
	/**
	 * Exception Defines the Top-Level Exception class.
	 *
	 * All Exception classes are derived from the top level cutil::Exception class
	 * which is itself derived from std::exception.
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class Exception : public std::exception
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor
			
			/**
			 * Construct a new Exception with the specified error message.
			 *
			 * @param message the error message
			 */
			Exception(std::string message) ;

			/**
			 * Destructor
			 */
			virtual ~Exception() throw() ;
			
			
			//-------------------------------------------------------------------------------//
			// Accesors

			/**
			 * Returns the message of this Exception as a std::string
			 *
			 * @return the error message as std::string
			 */
			virtual std::string toString() ;

			/**
			 * Returns the error message as a char*
			 *
			 * @return the error message as a char*
			 */
			virtual const char* what() const throw() ;
			
			//-------------------------------------------------------------------------------//

		protected:
		
			//-------------------------------------------------------------------------------//

		private:

			/** the error message */
			std::string theMessage ;

	} ; /* class Exception */

} /* namespace cutil */

#endif /* _CUTIL_EXCEPTION_H_ */
