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

#ifndef _CUTIL_DIMENSION_H_
#define _CUTIL_DIMENSION_H_

namespace cutil
{
	/**
	 * A Dimension is a utility convenience to contain a width and a height value
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class Dimension
	{
		public:
			/**
			 * Construct a new Dimension with both width and height of zero
			 *
			 */
			Dimension() ;

			/**
			 * Construct a copy of the specified Dimension
			 *
			 * @param d the Dimension to copy
			 */
			Dimension(const Dimension& d) ;

			/**
			 * Construct a new Dimension with the specified width and height values
			 *
			 * @param width the width of this Dimension
			 * @param height the height of this Dimension
			 */
			Dimension(int width, int height) ;

			/**
			 * Destructor
			 */
			~Dimension() ;

			/**
			 * Returns the width of this Dimension
			 *
			 * @return the width of this Dimension
			 */
			int getWidth() const ;

			/**
			 * Returns the height of this Dimension
			 *
			 * @return the height of this Dimension
			 */
			int getHeight() const ;

			/**
			 * Sets the width of this Dimension
			 *
			 * @param width the new width of this Dimension
			 */
			void setWidth(int width) ;

			/**
			 * Sets the height of this Dimension
			 *
			 * @param height the new height of this Dimension
			 */
			void setHeight(int height) ;

			/**
			 * Setsthe size of this Dimensin to match the specified Dimension
			 *
			 * @param d the Dimension to set this Dimension size
			 */
			void setSize(const Dimension& d) ;

		protected:

		private:
			int theWidth ;
			int theHeight ;
	} ;
}

#endif /* _CUTIL_DIMENSION_H_ */

