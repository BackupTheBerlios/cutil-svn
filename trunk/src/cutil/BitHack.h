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


#ifndef _CUTIL_BITHACK_H_
#define _CUTIL_BITHACK_H_

/**
 * claws collection of useful utilities, A collection of useful bits and pieces.
 */
namespace cutil
{
	/**
	 * Various useful bit manipulation functions.
	 * This class is a collection of static methods that provide fast operations through
	 * manipulation of bits, rather than standard numerical calculations.
	 * These come from http://graphics.stanford.edu/~seander/bithacks.html
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class BitHack
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor
			
			/**
			 * Destructor
			 * We dont actually allow instances of this class to be created, so this will never be called
			 */
			virtual ~BitHack() ;

			//-------------------------------------------------------------------------------//

			/**
			 * Calculate the log to the base 2 of the specified value.
			 *
			 * @param value the input value
			 * @return the log to the base 2 of the input value
			 */
			static unsigned int log2(unsigned int value) ;

			/**
			 * Calculate the log to the base 2 of the specified value.
			 * This method must only be used if the input value is gauranteed to be
			 * a power of 2. For speed purposes, no error/value checking is performed.
			 *
			 * @param value the input value
			 * @return the log to the base 2 of the input value
			 */
			static unsigned int log2Unchecked(unsigned int value) ;

			/**
			 * Calculates the next value that is a power of 2, greater then, or equal to the input value.
			 *
			 * @param value the input value
			 * @return the next power of 2 greater or equal to the input value
			 */
			static unsigned int nextPow2(unsigned int value) ;


			//-------------------------------------------------------------------------------//
			// Static member data

			// These arrays are used when calculating the log to the base 2.
			// @todo document how these are used
			static const unsigned int BIT32_LOG2_B[] ;
			static const unsigned int BIT32_LOG2_S[] ;

		protected:

			//-------------------------------------------------------------------------------//

		private:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * Dis-allow the Construction of a BitHack instance
			 *
			 */
			BitHack() ;

			//-------------------------------------------------------------------------------//

	} ; /* class BitHack */

	//-------------------------------------------------------------------------------//
} /* namespace cutil */

#endif /* _CUTIL_BITHACK_H_ */
