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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USAGNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * @author Colin Law [claw@mail.berlios.de]
 * $Id$
 */

#include <cutil/Rectangle.h>

#include <cutil/Dimension.h>
#include <cutil/Point.h>

using cutil::Dimension ;
using cutil::Point ;
using cutil::Rectangle ;

/**
 * Construct a new Rectangle whose top-left corner is (0,0) and with a width and height of zero
 *
 */
Rectangle::Rectangle()
{
	theX = 0 ;
	theY = 0 ;
	theWidth = 0 ;
	theHeight = 0 ;
}

/**
 * Construct a copy of the specified Rectangle
 *
 * @param r the Rectangle to copy
 */
Rectangle::Rectangle(const Rectangle& r)
 : AbstractShape()
{
	theX = r.getX() ;
	theY = r.getY() ;
	theWidth = r.getWidth() ;
	theHeight = r.getHeight() ;
}

/**
 * Construct a new Rectangle whose top-left corner is (0,0) and whose width and height are obtained from the specified Dimension
 *
 * @param dimension the width and height of this Rectangle as a Dimension
 */
Rectangle::Rectangle(const cutil::Dimension& dimension)
{
	theX = 0 ;
	theY = 0 ;
	theWidth = dimension.getWidth() ;
	theHeight = dimension.getHeight() ;
}

/**
 * Construct a new Rectangle whose top-left corner is (0,0) and whose width and height are the specified values
 *
 * @param width the width of this Rectangle
 * @param height the height of this Rectangle
 */
Rectangle::Rectangle(double width, double height)
{
	theX = 0 ;
	theY = 0 ;
	theWidth = width ;
	theHeight = height ;
}

/**
 * Construct a new Rectangle whose top-left corner is specified by (x, y) and whose width and height are the specified values
 *
 * @param x the x coordinate of the top left corner of this Rectangle
 * @param y the y coordinate of the top left corner of this Rectangle
 * @param width the width of this Rectangle
 * @param height the height of this Rectangle
 */
Rectangle::Rectangle(double x, double y, double width, double height)
{
	theX = x ;
	theY = y ;
	theWidth = width ;
	theHeight = height ;
}

/**
 * Destructor
 */
Rectangle::~Rectangle()
{
	// nothing much to do
}


/**
 * Returns the X coordinate of the top-left corner of this Rectangle
 *
 * @return the X coordinate of this Rectangle
 */
double
Rectangle::getX() const
{
	return(theX) ;
}

/**
 * Returns the Y coordinate of the top-left corner of this Rectangle
 *
 * @return the y coordinate of this Rectangle
 */
double
Rectangle::getY() const
{
	return(theY) ;
}

/**
 * Returns the width of this Rectangle
 *
 * @return the width of this Rectangle
 */
double
Rectangle::getWidth() const
{
	return(theWidth) ;
}

/**
 * Returns the height of this Rectangle
 *
 * @return the height of this Rectangle
 */
double
Rectangle::getHeight() const
{
	return(theHeight) ;
}


/**
 * Sets the X coordinate the top-left corner of this Rectangle
 *
 * @param x the X coordinate of this Rectangle
 */
void
Rectangle::setX(double x)
{
	theX = x ;
}

/**
 * Sets the Y coordinate of the top-left corner of this Rectangle
 *
 * @param y the Y coordinate of this Rectangle
 */
void
Rectangle::setY(double y)
{
	theY = y ;
}

/**
 * Sets the width of this Rectangle
 *
 * @param width the new width of this Rectangle
 */
void
Rectangle::setWidth(double width)
{
	theWidth = width ;
}

/**
 * Sets the height of this Rectangle
 *
 * @param height the new height of this Rectangle
 */
void
Rectangle::setHeight(double height)
{
	theHeight = height ;
}


/**
 * Sets the size of this Rectangle to the specified width and height
 *
 * @param width the new width of this Rectangle
 * @param height the new height of this Rectangle
 */
void
Rectangle::setSize(double width, double height)
{
	theWidth = width ;
	theHeight = height ;
}

/**
 * Sets the location of this Rectangle within coordiate space to the specified X and Y coordinates
 *
 * @param x the X coordinate of the top-left corner of this Rectangle
 * @param y the Y coordinate of the top-left corner of this Rectangle
 */
void
Rectangle::setLocation(double x, double y)
{
	theX = x ;
	theY = y ;
}

/**
 * Sets the location and size of this Rectangle to the specified values.
 *
 * @param x the x coordinate of the top left corner of this Rectangle
 * @param y the y coordinate of the top left corner of this Rectangle
 * @param width the width of this Rectangle
 * @param height the height of this Rectangle
 */
void
Rectangle::setRect(double x, double y, double width, double height)
{
	theX = x ;
	theY = y ;
	theWidth = width ;
	theHeight = height ;
}

/**
 * Sets this Rectangle to the specifie Rectangle
 *
 * @param rect the Rectangle to set this Rectangle to
 */
void
Rectangle::setRect(const Rectangle& rect)
{
	this->setRect(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight()) ;
}

/**
 * Determine whether or not this Rectangle is empty
 * A Rectangle is considered empty if it has a zero width and a zero height
 *
 * @return true if this Rectangle is empty
 */
bool
Rectangle::isEmpty() const
{
	return((theWidth <= 0) && (theHeight <= 0)) ;
}

/**
 * Determine whether this Rectangle contains the specified point at the location (x,y)
 *
 * @param x the x component of the point
 * @param y the y component of the point
 * @return true if this Rectangle contains the specified point
 */
bool
Rectangle::contains(double x, double y) const
{
	return((theX <= x) && (x <= (theX + theWidth)) && (theY <= y) && (y <= (theY + theHeight))) ;
}

/**
 * Determine whether this Rectangle completely encloses the specified Rectangle
 *
 * @param x the x coordinate component of the Rectangle location
 * @param y the y coordinate component of the Rectangle location
 * @param width the width of the Rectangle
 * @param height the height of the Rectangle
 * @return true if this Rectangle completely encloses the specified Rectangle
 */
bool
Rectangle::contains(double x, double y, double width, double height) const
{
	return((theX <= x) && (x <= (theX + theWidth))
	       && (theX <= (x + width)) && ((x + width) <= (theX + theWidth))
	       && (theY <= y) && (y <= (theY + theHeight))
	       && (theY <= (y + height)) && ((y + height) <= (theY + theHeight))) ;
}

/**
 * Determine whether this Rectangle contains the specifie point
 *
 * @param point the point to test
 * @return true if this Rectangle contains the specified point
 */
bool
Rectangle::contains(const Point& point) const
{
	return(contains(point.getX(), point.getY())) ;
}

/**
 * Determine whether this Rectangle completely encloses the specified Rectangle
 *
 * @param rect the Rectangle to test
 * @return true if this Rectangle completely encloses the specified Rectangle
 */
bool
Rectangle::contains(const Rectangle& rect) const
{
	return(contains(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight())) ;
}

/**
 * Returns the bounding Rectangle of this AbstractShape within the bounds paramater
 *
 * @param bounds set to the bounding Rectangle of this AbstractShape
 * @return returns bounding Rectangle of this AbstractShape
 */
Rectangle& Rectangle::getBounds(Rectangle& bounds) const
{
	bounds.setRect(*this) ;
	return(bounds) ;
}

/**
 * Returns true if the specified Rectangle intersects with this Rectangle
 *
 * @param rect the Rectangle with which to determine intersection
 * @return true if the sepcified Rectangle intersects with this Rectangle,
 *         false otherwise
 */
bool
Rectangle::intersects(const Rectangle& rect) const
{
	return(this->intersects(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight())) ;
}

/**
 * Returns true if the specified Rectangle intersects with this Rectangle
 *
 * @param x the x coordinate component of the Rectangle to intersect
 * @param y the y coordinate component of the Rectangle to intersect
 * @param width the width of the Rectangle to intersect
 * @param height the height of the Rectangle to intersect
 * @return true if the sepcified Rectangle intersects with this Rectangle,
 *         false otherwise
 */
bool
Rectangle::intersects(double x, double y, double width, double height) const
{
	double intersect_l = std::max(this->getX(), x) ;
	double intersect_y = std::max(this->getY(), y) ;
	double intersect_w = std::min(this->getX() + this->getWidth(), x + width) ;
	double intersect_h = std::min(this->getY() + this->getHeight(), y + height) ;

	if((intersect_w < intersect_l) && (intersect_h < intersect_y))
	{
		return(false) ;
	}
	else
	{
		return(true) ;
	}
}

/**
 * Intersects the specified Rectangle, src1 and src2 and returns the results via the dst parameter
 *
 * @param src1 the first Rectangle of the intersection
 * @param src2 the second Rectangle of the intersection
 * @param dst Rectangle object used to return the result of the intersection
 * @return dts parameter set to the intersection of src1 and src2
 */
Rectangle&
Rectangle::intersect(const Rectangle& src1, const Rectangle& src2, Rectangle& dst)
{
	dst.setX(std::max(src1.getX(), src2.getX())) ;
	dst.setY(std::max(src1.getY(), src2.getY())) ;
	dst.setWidth(std::min(src1.getX() + src1.getWidth(), src2.getX() + src2.getWidth())) ;
	dst.setHeight(std::min(src1.getY() + src1.getHeight(), src2.getY() + src2.getHeight())) ;

	return(dst) ;
}

/**
 * Adds the specified Rectangle to this Rectangle
 *
 * @param rect the Rectangle to add
 */ 
void
Rectangle::add(const Rectangle& rect)
{
	double x = this->getX() <= 0 ? rect.getX() : std::min(this->getX(), rect.getX()) ;
	double y = this->getY() <= 0 ? rect.getY() : std::min(this->getY(), rect.getY()) ;
	double w = this->getWidth() <= 0 ? rect.getWidth() : std::max(this->getX() + this->getWidth(), rect.getX() + rect.getWidth()) - x ;
	double h = this->getHeight() <= 0 ? rect.getHeight() : std::max(this->getY() + this->getHeight(), rect.getY() + rect.getHeight()) - y ;

	this->setRect(x, y, w, h) ;
}
