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

#include <cutil/Point.h>

#include <sstream>
#include <string>

using cutil::Point ;

//-------------------------------------------------------------------------------//
// Constructor / Destructor

/**
 * Constructs a new Point intialized to the origin (0, 0)
 *
 */
Point::Point() : theX(0), theY(0)
{}

/**
 *  Constructs a new Point initialized to the specified coordinates (x, y)
 *
 * @param x the x coordinate
 * @param y the y coordinate
 */
Point::Point(double x, double y) : theX(x), theY(y)
{}

/**
 * Constructs a new Point which is a copy of the specified Point
 *
 * @param p the Point to copy
 */
Point::Point(const Point& p)
{
	theX = p.getX() ;
	theY = p.getY() ;
}

/**
 * Destructor
 */
Point::~Point()
{
	// nothing to do
}


//-------------------------------------------------------------------------------//
// Point Accessors / Mutators

/**
 * Returns the X coordinate of this Point
 *
 * @return the X coordinate of this Point
 */
double
Point::getX() const
{
	return(theX) ;
}

/**
 * Returns the Y coordinate of this Point
 *
 * @return the y coordinate of this Point
 */
double
Point::getY() const
{
	return(theY) ;
}

/**
 * Sets the location of this Point to the specified coordinates
 *
 * @param x the x coordinate
 * @param y the y coordinate
 */
void
Point::setLocation(double x, double y)
{
	theX = x ;
	theY = y ;
}

/**
 * Sets the location of this Point to the Point
 *
 * @param p the Point containing the new coordinates of this Point
 */
void
Point::setLocation(const Point& p)
{
	theX = p.getX() ;
	theY = p.getY() ;
}

/**
 * Returns a string representation of this Point.
 * The returned string indicates the class type in this case, Point, and the location
 * of this Point
 *
 * @return a string representation of this Point
 */
std::string
Point::toString() const
{
	std::ostringstream buf ;
	buf << "Point[x=" << theX << ", y=" << theY << "]" ;
	return(buf.str()) ;
}

/**
 * Translates this points location from it original location (x, y) to the new location (x + dx, y + dy)
 *
 * @param x the value to translate along the x-axis
 * @param y the value to translate along the y-axis
 */
void
Point::translate(double x, double y)
{
	theX += x ;
	theY += y ;
}
