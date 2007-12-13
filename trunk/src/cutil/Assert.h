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
			 * @param message string added to the exception message
			 */
			template <class T>
			static void areEqual(T expected, T actual, std::string message = std::string("")) ;

			/**
			 * Determines if not_expected and actual are not equal
			 * Inequality is determined via the equality operator.
			 *
			 * @param notExpected the value actual is not expected to be equal to.
			 * @param actual the value to be checked
			 * @param message string added to the exception message
			 */
			template <class T>
			static void areNotEqual(T not_expected, T actual, std::string message = std::string("")) ;

			/**
			 * Determines if actual is true, throwing a cutil::Exception if actual is not true
			 *
			 * @param actual the value to test
			 * @param message string added to the exception message
			 */
			static void isTrue(bool actual, std::string message = std::string(""))
			{
				if(actual != true)
				{
					std::ostringstream buf ;
					buf << "Assertation Failed (isTrue): " ;
					buf << message ;
					buf << " [actual=" << actual << "]" ;

					throw(Exception(buf.str())) ;
				}
			}

			/**
			 * Determines if actual is false, throwing a cutil::Exception if actual is not false
			 *
			 * @param actual the value to test
			 * @param message string added to the exception message
			 */
			static void isFalse(bool actual, std::string message = std::string(""))
			{
				if(actual != false)
				{
					std::ostringstream buf ;
					buf << "Assertation Failed (isFalse): " ;
					buf << message ;
					buf << " [actual=" << actual << "]" ;

					throw(Exception(buf.str())) ;
				}
			}

			/**
			 * Determines if the specified pointer is a null pointer, i.e. 0.
			 *
			 * @param ptr pointer to test
			 * @param message string added to the exception message
			 */
			static void isNull(void* ptr, std::string message = std::string(""))
			{
				if(ptr != 0)
				{
					throw(Exception(std::string("Assertation Failed (isNull): ") + message)) ;
				}
			}

			/**
			 * Determines if the specified pointer is not a null pointer, i.e. not 0.
			 *
			 * @param ptr pointer to test
			 * @param message string added to the exception message
			 */
			static void isNotNull(void* ptr, std::string message = std::string(""))
			{
				if(ptr == 0)
				{
					throw(Exception(std::string("Assertation Failed (isNotNull): ") + message)) ;
				}
			}

			/**
			 *  throws cutil::Exception with the specified message string
			 *
			 * @param message string added to the exception message
			 */
			static void fail(std::string message = std::string(""))
			{
				throw(Exception(message)) ;
			}

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

	template <class T>
	void Assert::areNotEqual(T not_expected, T actual, const std::string message)
	{
		bool equal = (not_expected == actual) ;
		if(equal)
		{
			std::ostringstream buf ;
			buf << "Assertation Failed: " ;
			buf << message ;
			buf << " [not_expected=" << not_expected << ",actual=" << actual << "]" ;

			throw(Exception(buf.str())) ;
		}
	}

} /* namespace cutil */

#endif /* _CUTIL_ASSERT_H_ */
