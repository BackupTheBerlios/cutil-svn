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

#ifndef _CUTIL_POINT_H_
#define _CUTIL_POINT_H_

#include <string>

namespace cutil
{
	/**
	 * A Point representing a location in coordinate space.
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class Point
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Destructor

			/**
			 * Constructs a new Point intialized to the origin (0, 0)
			 *
			 */
			Point() ;

			/**
			 *  Constructs a new Point initialized to the specified coordinates (x, y)
			 *
			 * @param x the x coordinate
			 * @param y the y coordinate
			 */
			Point(double x, double y) ;

			/**
			 * Constructs a new Point which is a copy of the specified Point
			 *
			 * @param p the Point to copy
			 */
			Point(const Point& p) ;

			/**
			 * Destructor
			 */
			virtual ~Point() ;


			//-------------------------------------------------------------------------------//
			// Point Accessors / Mutators

			/**
			 * Returns the X coordinate of this Point
			 *
			 * @return the X coordinate of this Point
			 */
			double getX() const ;

			/**
			 * Returns the Y coordinate of this Point
			 *
			 * @return the y coordinate of this Point
			 */
			double getY() const ;

			/**
			 * Sets the location of this Point to the specified coordinates
			 *
			 * @param x the x coordinate
			 * @param y the y coordinate
			 */
			void setLocation(double x, double y) ;

			/**
			 * Sets the location of this Point to the Point
			 *
			 * @param p the Point containing the new coordinates of this Point
			 */
			void setLocation(const Point& p) ;

			/**
			 * Returns a string representation of this Point.
			 * The returned string indicates the class type in this case, Point, and the location
			 * of this Point
			 *
			 * @return a string representation of this Point
			 */
			virtual std::string toString() const ;

			/**
			 * Translates this points location from it original location (x, y) to the new location (x + dx, y + dy)
			 *
			 * @param x the value to translate along the x-axis
			 * @param y the value to translate along the y-axis
			 */
			void translate(double x, double y) ;

			//-------------------------------------------------------------------------------//

		protected:

			/** the x coordianate */
			double theX ;

			/** the y coordianate */
			double theY ;

			//-------------------------------------------------------------------------------//

		private:

	} ;
}

#endif /* _CUTIL_POINT_H_ */

