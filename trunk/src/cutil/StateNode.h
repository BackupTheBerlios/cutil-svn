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

#ifndef _CUTIL_STATENODE_H_
#define _CUTIL_STATENODE_H_

#include <cutil/RefCountPtr.h>
#include <list>
#include <string>

namespace cutil
{
	/**
	 * A StateNode is an abstract storage method for state information.
	 * Implementations may choose to maintain state information within an XML data file.
	 * a database, or within a local filesystem.
	 * State data set and accessed through a name/value scheme, where a value is associated
	 * with a specified name. This name can later be used to access the named data value.
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class StateNode
	{
		public:
			//---------------------------------------------------------------------------------------//
			// Desctructor

			/**
			 * Destructor
			 */
			virtual ~StateNode() ;

			//---------------------------------------------------------------------------------------//
			// State Tree Handling

			/**
			 * Returns the parent StateNode of this StateNode.
			 * If the parent StateNode does not exist, the returned RefCountPtr will point to 0.
			 * This method returns a user handle for the underlying data of the StateNode
			 * implementation. Subsequent calls to getParent need not return the same
			 * handle instance, however, it is guaranteed that each will be a handle to
			 * the same underlying data instance.
			 *
			 * @return the parent StateNode to this StateNode
			 */
			virtual RefCountPtr<StateNode> getParent() = 0 ;

			/**
			 * Returns thenamed child StateHandler from the state tree below this StateHandler,
			 * If the child StateNode does not yet exist, it is created and returned, creating any
			 * parent nodes as required.
			 * The child node is specified by a '/' separated path, indicating parent StateNodes.
			 *
			 * This method returns a user handle for the underlying data of the StateNode
			 * implementation. Subsequent calls to getChild need not return the same
			 * handle instance, however, it is guaranteed that each will be a handle to
			 * the same underlying data instance.
			 *
			 * @param childPath the path to the child node
			 * @return the specified child node
			 */
			virtual RefCountPtr<StateNode> getChild(std::string childPath) = 0 ;

			/**
			 * Removes the specified child StateNode from the state tree.
			 * The specified child Statehandler must be below this StateNode within the state tree
			 *
			 * @param childPath the path to the child StateNode to remove
			 */
			virtual void removeChild(std::string childPath) = 0 ;

			/**
			 * Returns true of this StateNode contains the specified Child StateNode
			 *
			 * @param childPath the path of the specified child StateNode
			 * @return true if thie StateNode contains th specified StateNode
			 */
			virtual bool hasChild(const std::string& childPath) = 0 ;

			/**
			 * Populate the specified list with all the child StateNode names directly below this StateNode in the state tree
			 * The list is not cleared prior to any additions
			 *
			 * @param childList the list to populate
			 * @return the number of elements added to the list
			 */
			virtual int getChildren(std::list<std::string>& childList) = 0 ;

			/**
			 * Returns the complete path of this StateNode starting at the root '/'
			 *
			 * @return the complete path of this StateNode
			 */
			virtual const std::string& getPath() const = 0 ;


			//---------------------------------------------------------------------------------------//
			// State Handling

			/**
			 * Stores the specified value string within this StateHandler associated with the specified name string
			 *
			 * @param name name with which the specified value will be associated
			 * @param value the string value to store with the specified name
			 */
			virtual void setString(const std::string& name, const std::string& value) = 0 ;

			/**
			 * Stores the specified int value within this StateHandler associated with the specified name string
			 *
			 * @param name name with which the specified value will be associated
			 * @param value the int value to store with the specified name
			 */
			virtual void setInt(const std::string& name, int value) = 0 ;

			/**
			 * Stores the specified double value within this StateHandler associated with the specified name string
			 *
			 * @param name name with which the specified value will be associated
			 * @param value the double value to store with the specified name
			 */
			virtual void setDouble(const std::string& name, double value) = 0 ;

			/**
			 * Stores the specified bool value within this StateHandler associated with the specified name string
			 *
			 * @param name name with which the specified value will be associated
			 * @param value the bool value to store with the specified name
			 */
			virtual void setBool(const std::string& name, bool value) = 0 ;



			/**
			 * Returns the specified string value associated with the specified name from this StateHandler.
			 * If the specified name is not found, the specified default value will be returned
			 *
			 * @param name the name with which the value is associated
			 * @param def the default value if the specified name cannot be found
			 * @return the string value associated with the specified name
			 */
			virtual std::string getString(const std::string& name, const std::string& def) const = 0 ;

			/**
			 * Returns the specified int value associated with the specified name from this StateHandler.
			 * If the specified name is not found, or the value returned is not of the correct type
			 * the specified default value will be returned
			 *
			 * @param name the name with which the value is associated
			 * @param def the default value if the specified name cannot be found, or is not the correct type
			 * @return the int value associated with the specified name
			 */
			virtual int getInt(const std::string& name, int def) const = 0 ;

			/**
			 * Returns the specified int value associated with the specified name from this StateHandler.
			 * If the specified name is not found, or the value returned is not of the correct type
			 * the specified default value will be returned
			 *
			 * @param name the name with which the value is associated
			 * @param def the default value if the specified name cannot be found, or is not the correct type
			 * @return the int value associated with the specified name
			 */
			virtual double getDouble(const std::string& name, double def) const = 0 ;

			/**
			 * Returns the specified int value associated with the specified name from this StateHandler.
			 * If the specified name is not found, or the value returned is not of the correct type
			 * the specified default value will be returned
			 *
			 * @param name the name with which the value is associated
			 * @param def the default value if the specified name cannot be found, or is not the correct type
			 * @return the int value associated with the specified name
			 */
			virtual bool getBool(const std::string& name, bool def) const = 0 ;



			/**
			 * Removed the specified name value pair from this StateHandler
			 *
			 * @param name name of the name value pair to remove
			 */
			virtual void removeValue(const std::string& name) = 0 ;



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
			StateNode() ;
			
			/**
			 * Breaks the specified path into its component segmants and adds each to pathSegments
			 *
			 * @param path the path to decompose into path segments
			 * @param pathSegments the list to add each segment to
			 * @return the pathSegments list
			 */
			std::list<std::string>& breakPath(const std::string& path, std::list<std::string>& pathSegments) ;

			//---------------------------------------------------------------------------------------//

		private:



	} ; /* class StateNode */


} /* namespace cutil */

#endif /* _CUTIL_STATENODE_H_ */
