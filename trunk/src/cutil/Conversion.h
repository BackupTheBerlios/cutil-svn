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
 * @author Colin Law [claw@mail.berlios.de]
 *
 * $Id$
 */

#ifndef _CUTIL_CONVERSION_H_
#define _CUTIL_CONVERSION_H_

#include <cutil/Exception.h>

#include <iomanip>
#include <string>
#include <sstream>

namespace cutil
{
	/**
	 * Various static utility methods for conversion of types.
	 *
	 */
	class Conversion
	{
		public:
			/**
			 * Converts string s to type T returning via reference
			 * <code>
			 *	float f = 0.0f ;
			 *	std::string s = "12.3" ;
			 *	fromString(s, f) ;
			 * </code>
			 *
			 * @param s string to convert
			 * @param val set to the result of the conversion.
			 * @throw Exception if the conversion failed.
			 */
			template <class T> static void fromString(const std::string& s, T& val) throw(cutil::Exception) ;

			/**
			 * Converts string s to type T
			 * <code>
			 *	std::string s = "12.3" ;
			 *	float f = fromString<float>(s) ;
			 * </code>
			 *
			 * @param s string to convert
			 * @return the result of the conversion.
			 * @throw Exception if the conversion failed.
			 */
			template <class T> static T fromString(const std::string& s) throw(cutil::Exception) ;

			/**
			 * Converts type val to a string
			 *
			 * @param val type to convert to string
			 * @return string representation of val
			 * @throw Exception if the conversion failed.
			 */
			template <class T> static std::string toString(const T& val) throw(cutil::Exception) ;

			/**
			 * Converts type val to a string
			 * This variation provides a padding and width parameter to control formating of
			 * the conversion. This would typically be used to convert from a number to string.
			 *
			 * @param val type to convert to string
			 * @return string representation of val
			 * @throw Exception if the conversion failed.
			 */
			template <class T> static std::string toString(const T& val, char pad, size_t width) throw(cutil::Exception) ;

			//---------------------------------------------------------------------------------------//

		private:
			Conversion() {} ;
			Conversion(const Conversion&) {} ;

			//---------------------------------------------------------------------------------------//

	} ; /* class Conversion */

	template <class T>
	void Conversion::fromString(const std::string& s, T& val) throw(cutil::Exception)
	{
		std::istringstream buf(s) ;
		buf >> val ;
		if(buf.fail())
		{
			std::ostringstream ebuf ;
			ebuf << __PRETTY_FUNCTION__ << " : Conversion failed" ;
			throw(cutil::Exception(ebuf.str())) ;
		}
	}

	template <class T>
	T Conversion::fromString(const std::string& s) throw(cutil::Exception)
	{
		T value ;
		fromString(s, value) ;
		return(value) ;
	}

	template <class T>
	std::string Conversion::toString(const T& val) throw(cutil::Exception)
	{
		std::ostringstream buf ;
		buf << val;
		if(buf.fail())
		{
			std::ostringstream ebuf ;
			ebuf << __PRETTY_FUNCTION__ << " : Conversion failed" ;
			throw(cutil::Exception(ebuf.str())) ;
		}
		return(buf.str()) ;
	}

	template <class T>
	std::string Conversion::toString(const T& val, char pad, size_t width) throw(cutil::Exception)
	{
		std::ostringstream buf ;
		buf << std::setfill(pad) ;
		buf << std::setw(width) << val ;
		if(buf.fail())
		{
			std::ostringstream ebuf ;
			ebuf << __PRETTY_FUNCTION__ << " : Conversion failed" ;
			throw(cutil::Exception(ebuf.str())) ;
		}

		const std::string s = buf.str() ;
		if(s.size() > width)
		{
			std::ostringstream ebuf ;
			ebuf << __PRETTY_FUNCTION__ << ": string length exceeds maximum length[" << s.size() << "," << width << "]" ;
			throw(cutil::Exception(ebuf.str())) ;
		}

		return(buf.str()) ;
	}

} /* namespace cutil */

#endif /* _CUTIL_CONVERSION_H_ */

