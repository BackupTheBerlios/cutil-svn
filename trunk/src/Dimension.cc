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

#include <cutil/Dimension.h>

using cutil::Dimension ;

/**
 * Construct a new Dimension with both width and height of zero
 *
 */
Dimension::Dimension() : theWidth(0), theHeight(0)
{}

/**
 * Construct a copy of the specified Dimension
 *
 * @param d the Dimension to copy
 */
Dimension::Dimension(const Dimension& d) : theWidth(d.getWidth()), theHeight(d.getHeight())
{}

/**
 * Construct a new Dimension with the specified width and height values
 *
 * @param width the width of this Dimension
 * @param height the height of this Dimension
 */
Dimension::Dimension(int width, int height) : theWidth(width), theHeight(height)
{}

/**
 * Destructor
 */
Dimension::~Dimension()
{}

/**
 * Returns the width of this Dimension
 *
 * @return the width of this Dimension
 */
int
Dimension::getWidth() const
{
	return(theWidth) ;
}

/**
 * Returns the height of this Dimension
 *
 * @return the height of this Dimension
 */
int
Dimension::getHeight() const
{
	return(theHeight) ;
}

/**
 * Sets the width of this Dimension
 *
 * @param width the new width of this Dimension
 */
void
Dimension::setWidth(int width)
{
	theWidth = width ;
}

/**
 * Sets the height of this Dimension
 *
 * @param height the new height of this Dimension
 */
void
Dimension::setHeight(int height)
{
	theHeight = height ;
}

/**
 * Setsthe size of this Dimensin to match the specified Dimension
 *
 * @param d the Dimension to set this Dimension size
 */
void
Dimension::setSize(const Dimension& d)
{
	theWidth = d.getWidth() ;
	theHeight = d.getHeight() ;
}

