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

#ifndef _CUTIL_STATEHANDLER_H_
#define _CUTIL_STATEHANDLER_H_

#include <cutil/RefCountPtr.h>

#include <list>
#include <string>

namespace cutil
{
	class StateNode ;

	/**
	 * A StateHandler is an abstract storage method for state information.
	 * Implementations may choose to maintain state information within an XML data file.
	 * a database, or within a local filesystem.
	 * State data set and accessed through a name/value scheme, where a value is associated
	 * with a specified name. This name can later be used to access the named data value.
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class StateHandler
	{
		public:
			//---------------------------------------------------------------------------------------//
			// Desctructor

			/**
			 * Destructor
			 */
			virtual ~StateHandler() ;


			//---------------------------------------------------------------------------------------//
			// State Tree Handling

			/**
			 * Initialize this StateHandler.
			 * This method provides an entry point to initialize a StateHandler, this initialization
			 * may perform some initially required setup, such as establish a connection to a backing
			 * store.
			 *
			 */
			virtual void initialize() = 0 ;

			/**
			 * Shutdown this StateHandler.
			 * This method provides an exit point for a state handler.
			 * This method should flush the current state information in the state tree and perform
			 * any last cleanup operations. This may be to close any open database connections,
			 * files or any other used resources.
			 *
			 */
			virtual void shutdown() = 0 ;

			//---------------------------------------------------------------------------------------//
			// Update Control

			/**
			 * Force a write of state information back to the persistant store
			 *
			 */
			virtual void flush() = 0 ;

			/**
			 * Synchronize this StateHandler to the persistant store of state information,
			 * This method ensures that this StateHandler has the most up to date information
			 * from the persistant store. This may be useful if another componenet has access
			 * to the backing store and may be making modification independant of this StateHandler
			 *
			 */
			virtual void sync() = 0 ;

			/**
			 * Returns the root StateNode within the state tree of this StateHandler
			 * This method returns a user handle for the underlying data of the StateNode
			 * implementation. Subsequent calls to getRootNode need not return the same
			 * handle instance, however, it is guaranteed that each will be a handle to
			 * the same underlying data instance.
			 *
			 * @return the root StateNode within the state tree of this StateHandler
			 */
			virtual RefCountPtr<StateNode> getRootNode() = 0 ;

			//---------------------------------------------------------------------------------------//

		protected:

			//---------------------------------------------------------------------------------------//
			// Protected Constructor

			/**
			 * Protected Constructor to disallow creating instances of this purely abstract class.
			 * This class should be implemented to create an object which knows how to persitantly
			 * store state information for another object.
			 * An implementation may, for example, choose to save state data as an XML file, within
			 * a database, or in a local file system.
			 *
			 */
			StateHandler() ;

			//---------------------------------------------------------------------------------------//

		private:


	} ; /* class StateHandler */


} /* namespace cutil */

#endif /* _CUTIL_STATEHANDLER_H_ */
