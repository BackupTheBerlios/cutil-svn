/*
 * Copyright (C) 2007  Colin Law
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
 *
 * $Id$
 */

#ifndef _CUTIL_ASSERT_H_
#define _CUTIL_ASSERT_H_

#include <cutil/Exception.h>

#include <string>
#include <sstream>

namespace cutil
{
	/**
	 * Assert provides simple checks which may be used within unit tests. If assert fails, an
	 * exception is thrown.
	 *
	 */
	class Assert
	{
		public:
			/**
			 * Determines if expected and actual are equal.
			 * Equality is determined via the equality operator.
			 *
			 * @param expected the expected value
			 * @param actual the value to be checked
			 * @param message string to log describing test failure.
			 */
			template <class T>
			static void areEqual(T expected, T actual, std::string message = std::string("")) ;

		private:
			/**
			 * private constructor to avoid instances of this static class
			 */
			Assert() {}

	} ; /* class Assert */

	//---------------------------------------------------------------------------------------//
	// Implementation

	template <class T>
	void Assert::areEqual(T expected, T actual, const std::string message)
	{
		bool equal = (expected == actual) ;
		if(!equal)
		{
			std::ostringstream buf ;
			buf << "Assertation Failed: " ;
			buf << message ;
			buf << " [expected=" << expected << ",actual=" << actual << "]" ;

			throw(Exception(buf.str())) ;
		}
	}

} /* namespace cutil */

#endif /* _CUTIL_ASSERT_H_ */
