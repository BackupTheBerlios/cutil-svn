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

#ifndef _CUTIL_STATEABLE_H_
#define _CUTIL_STATEABLE_H_

#include <cutil/RefCountPtr.h>

namespace cutil
{
	// Forward Declaration
	class StateNode ;


	/**
	 * This Purely Abstract Class defines an Object that may have its state persistantly saved and restored.
	 * This interface, or purely abstract class, provides a mechanism for an external 'controlling' class to
	 * initiate the state initialization and saving of this this Stateable by calling saveState and initState.
	 * An alternative approach is to pass a StateNode reference to an object which may then update handle its
	 * state as required.
	 * Objects which can save and restore there state should implement this Abstract Class.
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class Stateable
	{
		public:
			//---------------------------------------------------------------------------------------//
			// Destructor

			/**
			 * Destructor
			 */
			virtual ~Stateable() {} ;


			//---------------------------------------------------------------------------------------//
			// State Handling

			/**
			 * Saves the state of this Stateable.
			 * The stateNode parameter represents the parent StateNode within the state tree.
			 * Generally state nodes should be grouped by creating nodes below thier parent,
			 * although for simple applications this may not be necesary.
			 *
			 * Implementations of the Stateable interface generally should not hold onto the stateNode
			 * reference outwith this method call.
			 *
			 * @param stateNode the root StateNode of the state tree for the application
			 */
			virtual void saveState(RefCountPtr<StateNode> stateNode) = 0 ;

			/**
			 * Initializes the state of this Stateable.
			 * The stateNode parameter represents the parent StateNode within the state tree.
			 * Generally state nodes should be grouped by creating nodes below thier parent,
			 * although for simple applications this may not be necesary.
			 *
			 * Implementations of the Stateable interface generally should not hold onto the stateNode
			 * reference outwith this method call.
			 * 
			 * @param stateNode the root StateNode of the state tree for the application
			 */
			virtual void initState(RefCountPtr<StateNode> stateNode) = 0 ;

			//---------------------------------------------------------------------------------------//

		protected:
			//---------------------------------------------------------------------------------------//
			// Protected Constructor

			/**
			 * Protected Constructor to disallow creating instances of this purely abstract class.
			 * This class must be implemented either as a stand alone object which knows how to handle
			 * the state of an associated object from a specified StateNode, or extended
			 * into a fully implementing object which knows how to save and initialize its own state from
			 * the specified StateNode
			 *
			 */
			Stateable() {} ;

			//---------------------------------------------------------------------------------------//

		private:


	} ; /* class Stateable */

} /* namespace cutil */

#endif /* _CUTIL_STATEABLE_H_ */
