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

#ifndef _CUTIL_ABSTRACT_SHAPE_H_
#define _CUTIL_ABSTRACT_SHAPE_H_

namespace cutil
{
	class Dimension ;
	class Point ;
	class Rectangle ;

	/**
	 * A Rectangle specifiying an area in coordinate space, enclised by this Rectangles top left point (x,y) extending width and height
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class AbstractShape
	{
		public:
			/**
			 * Destructor
			 */
			virtual ~AbstractShape() {} ;

			/**
			 * Determine whether this AbstractShape contains the specified point at the location (x,y)
			 *
			 * @param x the x component of the point
			 * @param y the y component of the point
			 * @return true if this AbstractShape contains the specified point
			 */
			virtual bool contains(double x, double y) const = 0 ;

			/**
			 * Determine whether this AbstractShape completely encloses the specified Rectangle
			 *
			 * @param x the x coordinate component of the Rectangle location
			 * @param y the y coordinate component of the Rectangle location
			 * @param width the width of the Rectangle
			 * @param height the height of the Rectangle
			 * @return true if this AbstractShape completely encloses the specified Rectangle
			 */
			virtual bool contains(double x, double y, double width, double height) const = 0 ;

			/**
			 * Determine whether this AbstractShape contains the specifie point
			 *
			 * @param point the point to test
			 * @return true if this AbstractShape contains the specified point
			 */
			virtual bool contains(const Point& point) const = 0 ;

			/**
			 * Determine whether this AbstractShape completely encloses the specified Rectangle
			 *
			 * @param rect the Rectangle to test
			 * @return true if this AbstractShape completely encloses the specified Rectangle
			 */
			virtual bool contains(const Rectangle& rect) const = 0 ;

			/**
			 * Returns the bounding Rectangle of this AbstractShape within the bounds paramater
			 *
			 * @param bounds set to the bounding Rectangle of this AbstractShape
			 * @return returns bounding Rectangle of this AbstractShape
			 */
			virtual Rectangle& getBounds(Rectangle& bounds) const = 0 ;

			/**
			 * Returns true if the specified Rectangle intersects with this AbstractShape
			 *
			 * @param x the x coordinate component of the Rectangle to intersect
			 * @param y the y coordinate component of the Rectangle to intersect
			 * @param width the width of the Rectangle to intersect
			 * @param height the height of the Rectangle to intersect
			 * @return true if the sepcified Rectangle intersects with this AbstractShape,
			 *         false otherwise
			 */
			virtual bool intersects(double x, double y, double width, double height) const = 0 ;

			/**
			 * Returns true if the specified Rectangle intersects with this AbstractShape
			 *
			 * @param rect the Rectangle with which to determine intersection
			 * @return true if the sepcified Rectangle intersects with this AbstractShape,
			 *         false otherwise
			 */
			virtual bool intersects(const Rectangle& rect) const = 0 ;

		protected:
			/**
			 * Protected Constructor - disallow instances of Abstract class
			 */
			AbstractShape() {} ;

		private:

	} ;
}

#endif /* _CUTIL_ABSTRACT_SHAPE_H_ */

