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

#include <cutil/BitHack.h>

using namespace cutil ;


//-------------------------------------------------------------------------------//
// Initialize static Member Data
// These arrays are used during the calculations of log to the base 2
const unsigned int BitHack::BIT32_LOG2_B[] = {0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0, 0xFF00FF00, 0xFFFF0000} ;
const unsigned int BitHack::BIT32_LOG2_S[] = {1, 2, 4, 8, 16} ;


//-------------------------------------------------------------------------------//
// Constructor / Desctructor

/**
 * Construct a new BitHack (Private Constructor)
 *
 */
BitHack::BitHack()
{}


/**
 * Destructor
 *
 */
BitHack::~BitHack()
{}

//-------------------------------------------------------------------------------//


/**
 * Calculate the log to the base 2 of the specified value.
 *
 * @param value the input value
 * @return the log to the base 2 of the input value
 */
unsigned int BitHack::log2(unsigned int value)
{
	int i ;

	register unsigned int c = 0 ;
	for(i = 4; i >= 0; i--) // unroll for speed...
	{
		if(value & BitHack::BIT32_LOG2_B[i])
		{
			value >>= BitHack::BIT32_LOG2_S[i];
			c |= BitHack::BIT32_LOG2_S[i];
		}
	}

	return(c) ;
}


/**
 * Calculate the log to the base 2 of the specified value.
 * This method must only be used if the input value is gauranteed to be
 * a power of 2. For speed purposes, no error/value checking is performed.
 *
 * @param value the input value
 * @return the log to the base 2 of the input value
 */
unsigned int BitHack::log2Unchecked(unsigned int value)
{
	register unsigned int c = ((value & BitHack::BIT32_LOG2_B[0]) != 0) ;
	int i ;
	for(i = 4; i >= 1; i--) // unroll for speed...
	{
		c |= ((value & BitHack::BIT32_LOG2_B[i]) != 0) << i;
	}

	return(c) ;
}


/**
 * Calculates the next value that is a power of 2, greater then, or equal to the input value.
 *
 * @vparam alue the input value
 * @return the next power of 2 greater or equal to the input value
 */
unsigned int BitHack::nextPow2(unsigned int value)
{
	value-- ;
	value |= value >> 1 ;
	value |= value >> 2 ;
	value |= value >> 4 ;
	value |= value >> 8 ;
	value |= value >> 16 ;
	value++ ;

	return(value) ;
}
