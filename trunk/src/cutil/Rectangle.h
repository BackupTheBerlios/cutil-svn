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

#ifndef _CUTIL_RECTANGLE_H_
#define _CUTIL_RECTANGLE_H_

#include <cutil/AbstractShape.h>

namespace cutil
{
	class Dimension ;
	class Point ;

	/**
	 * A Rectangle specifiying an area in coordinate space, enclised by this Rectangles top left point (x,y) extending width and height
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class Rectangle : public AbstractShape
	{
		public:
			/**
			 * Construct a new Rectangle whose top-left corner is (0,0) and with a width and height of zero
			 *
			 */
			Rectangle() ;

			/**
			 * Construct a copy of the specified Rectangle
			 *
			 * @param r the Rectangle to copy
			 */
			Rectangle(const Rectangle& r) ;

			/**
			 * Construct a new Rectangle whose top-left corner is (0,0) and whose width and height are obtained from the specified Dimension
			 *
			 * @param dimension the width and height of this Rectangle as a Dimension
			 */
			Rectangle(const cutil::Dimension& dimension) ;

			/**
			 * Construct a new Rectangle whose top-left corner is (0,0) and whose width and height are the specified values
			 *
			 * @param width the width of this Rectangle
			 * @param height the height of this Rectangle
			 */
			Rectangle(double width, double height) ;

			/**
			 * Construct a new Rectangle whose top-left corner is specified by (x, y) and whose width and height are the specified values
			 *
			 * @param x the x coordinate of the top left corner of this Rectangle
			 * @param y the y coordinate of the top left corner of this Rectangle
			 * @param width the width of this Rectangle
			 * @param height the height of this Rectangle
			 */
			Rectangle(double x, double y, double width, double height) ;

			/**
			 * Destructor
			 */
			virtual ~Rectangle() ;


			/**
			 * Returns the X coordinate of the top-left corner of this Rectangle
			 *
			 * @return the X coordinate of this Rectangle
			 */
			double getX() const ;

			/**
			 * Returns the Y coordinate of the top-left corner of this Rectangle
			 *
			 * @return the y coordinate of this Rectangle
			 */
			double getY() const ;

			/**
			 * Returns the width of this Rectangle
			 *
			 * @return the width of this Rectangle
			 */
			double getWidth() const ;

			/**
			 * Returns the height of this Rectangle
			 *
			 * @return the height of this Rectangle
			 */
			double getHeight() const ;


			/**
			 * Sets the X coordinate the top-left corner of this Rectangle
			 *
			 * @param x the X coordinate of this Rectangle
			 */
			void setX(double x) ;

			/**
			 * Sets the Y coordinate of the top-left corner of this Rectangle
			 *
			 * @param y the Y coordinate of this Rectangle
			 */
			void setY(double y) ;

			/**
			 * Sets the width of this Rectangle
			 *
			 * @param width the new width of this Rectangle
			 */
			void setWidth(double width) ;

			/**
			 * Sets the height of this Rectangle
			 *
			 * @param height the new height of this Rectangle
			 */
			void setHeight(double height) ;


			/**
			 * Sets the size of this Rectangle to the specified width and height
			 *
			 * @param width the new width of this Rectangle
			 * @param height the new height of this Rectangle
			 */
			void setSize(double width, double height) ;

			/**
			 * Sets the location of this Rectangle within coordiate space to the specified X and Y coordinates
			 *
			 * @param x the X coordinate of the top-left corner of this Rectangle
			 * @param y the Y coordinate of the top-left corner of this Rectangle
			 */
			void setLocation(double x, double y) ;

			/**
			 * Sets the location and size of this Rectangle to the specified values.
			 *
			 * @param x the x coordinate of the top left corner of this Rectangle
			 * @param y the y coordinate of the top left corner of this Rectangle
			 * @param width the width of this Rectangle
			 * @param height the height of this Rectangle
			 */
			void setRect(double x, double y, double width, double height) ;

			/**
			 * Sets this Rectangle to the specifie Rectangle
			 *
			 * @param rect the Rectangle to set this Rectangle to
			 */
			void setRect(const Rectangle& rect) ;

			/**
			 * Determine whether or not this Rectangle is empty
			 * A Rectangle is considered empty if it has a zero width and a zero height
			 *
			 * @return true if this Rectangle is empty
			 */
			bool isEmpty() const ;


			/**
			 * Determine whether this Rectangle contains the specifie point at the location (x,y)
			 *
			 * @param x the x component of the point
			 * @param y the y component of the point
			 * @return true if this Rectangle contains the specified point
			 */
			virtual bool contains(double x, double y) const ;

			/**
			 * Determine whether this Rectangle completely encloses the specified Rectangle
			 *
			 * @param x the x coordinate component of the Rectangle location
			 * @param y the y coordinate component of the Rectangle location
			 * @param width the width of the Rectangle
			 * @param height the height of the Rectangle
			 * @return true if this Rectangle completely encloses the specified Rectangle
			 */
			virtual bool contains(double x, double y, double width, double height) const ;

			/**
			 * Determine whether this Rectangle contains the specified point
			 *
			 * @param point the point to test
			 * @return true if this Rectangle contains the specified point
			 */
			virtual bool contains(const Point& point) const ;

			/**
			 * Determine whether this Rectangle completely encloses the specified Rectangle
			 *
			 * @param rect the Rectangle to test
			 * @return true if this Rectangle completely encloses the specified Rectangle
			 */
			virtual bool contains(const Rectangle& rect) const ;

			/**
			 * Returns the bounding Rectangle of this AbstractShape within the bounds paramater
			 *
			 * @param bounds set to the bounding Rectangle of this AbstractShape
			 * @return returns bounding Rectangle of this AbstractShape
			 */
			virtual Rectangle& getBounds(Rectangle& bounds) const ;

			/**
			 * Returns true if the specified Rectangle intersects with this Rectangle
			 *
			 * @param rect the Rectangle with which to determine intersection
			 * @return true if the sepcified Rectangle intersects with this Rectangle,
			 *         false otherwise
			 */
			virtual bool intersects(const Rectangle& rect) const ;

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
			virtual bool intersects(double x, double y, double width, double height) const ;

			/**
			 * Intersects the specified Rectangle, src1 and src2 and returns the results via the dst parameter
			 *
			 * @param src1 the first Rectangle of the intersection
			 * @param src2 the second Rectangle of the intersection
			 * @param dst Rectangle object used to return the result of the intersection
			 * @return dts parameter set to the intersection of src1 and src2
			 */
			static Rectangle& intersect(const Rectangle& src1, const Rectangle& src2, Rectangle& dst) ;

			/**
			 * Adds the specified Rectangle to this Rectangle
			 *
			 * @param rect the Rectangle to add
			 */
			void add(const Rectangle& rect) ;

		protected:

		private:
			double theX ;
			double theY ;
			double theWidth ;
			double theHeight ;
	} ;
}

#endif /* _CUTIL_RECTANGLE_H_ */

