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

#ifndef _CUTIL_XMLSTATENODE_H_
#define _CUTIL_XMLSTATENODE_H_

#include <cutil/StateNode.h>

#include <cutil/XMLStateHandler.h> // reg for XMLStateHandler::ChildHandleManager

#include <string>

namespace xmlpp
{
	class Document ;
	class Element ;
}

namespace cutil
{
	/**
	 * XMLStateNode is an implementation of the StateNode interface to store state information within
	 * an xml document. This implementation wraps a XML document, and its contained nodes and elements
	 * providing a method of storing and retrieving data within the xml document through the StateNode
	 * interface.
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class XMLStateNode : public StateNode
	{
		public:
			//---------------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * Destructor
			 */
			virtual ~XMLStateNode() ;


			//---------------------------------------------------------------------------------------//
			// State Tree Handling

			/**
			 * Returns the parent StateNode of this StateNode.
			 * If the parent StateNode does not exist, 0 is returned
			 *
			 * This method returns a user handle for the underlying data of the StateNode
			 * implementation. Subsequent calls to getParent need not return the same
			 * handle instance, however, it is guaranteed that each will be a handle to
			 * the same underlying data instance.
			 *
			 * @return the parent StateNode to this StateNode, or 0 if this StateNode has noe parent
			 */
			virtual RefCountPtr<StateNode> getParent() ;

			/**
			 * Returns thenamed child StateHandler from the state tree below this StateHandler,
			 * If the child StateNode does not yet exist, it is created and returned, creating any
			 * parent nodes as required.
			 *
			 * This implementation does not yet support multiple tree path naming, ie only one level of child
			 * name may be supplied, 'childNodeName', and not 'path/to/childNodeName'
			 *
			 * This method returns a user handle for the underlying data of the StateNode
			 * implementation. Subsequent calls to getChild need not return the same
			 * handle instance, however, it is guaranteed that each will be a handle to
			 * the same underlying data instance.
			 *
			 * @param childPath the path to the child node
			 * @return the specified child node
			 */
			virtual RefCountPtr<StateNode> getChild(std::string childPath) ;

			/**
			 * Removes the specified child StateNode from the state tree.
			 * The specified child Statehandler must be below this StateNode within the state tree
			 *
			 * @param childPath the path to the child StateNode to remove
			 */
			virtual void removeChild(std::string childPath) ;

			/**
			 * Returns true of this StateNode contains the specified Child StateNode
			 *
			 * @param childPath the path of the specified child StateNode
			 * @return true if thie StateNode contains th specified StateNode
			 */
			virtual bool hasChild(const std::string& childPath) ;

			/**
			 * Populate the specified list with all the child StateNode names directly below this StateNode in the state tree
			 * The list is not cleared prior to any additions
			 *
			 * @param childList the list to populate
			 * @return the number of elements added to the list
			 */
			virtual int getChildren(std::list<std::string>& childList) ;

			/**
			 * Returns the complete path of this StateNode starting at the root '/'
			 *
			 * @return the complete path of this StateNode
			 */
			virtual const std::string& getPath() const ;


			//---------------------------------------------------------------------------------------//
			// State Handling

			/**
			 * Stores the specified value string within this StateHandler associated with the specified name string
			 *
			 * @param name name with which the specified value will be associated
			 * @param value the string value to store with the specified name
			 */
			virtual void setString(const std::string& name, const std::string& value) ;

			/**
			 * Stores the specified int value within this StateHandler associated with the specified name string
			 *
			 * @param name name with which the specified value will be associated
			 * @param value the int value to store with the specified name
			 */
			virtual void setInt(const std::string& name, int value) ;

			/**
			 * Stores the specified double value within this StateHandler associated with the specified name string
			 *
			 * @param name name with which the specified value will be associated
			 * @param value the double value to store with the specified name
			 */
			virtual void setDouble(const std::string& name, double value) ;

			/**
			 * Stores the specified bool value within this StateHandler associated with the specified name string
			 *
			 * @param name name with which the specified value will be associated
			 * @param value the bool value to store with the specified name
			 */
			virtual void setBool(const std::string& name, bool value) ;



			/**
			 * Returns the specified string value associated with the specified name from this StateHandler.
			 * If the specified name is not found, the specified default value will be returned
			 *
			 * @param name the name with which the value is associated
			 * @param def the default value if the specified name cannot be found
			 * @return the string value associated with the specified name
			 */
			virtual std::string getString(const std::string& name, const std::string& def) const ;

			/**
			 * Returns the specified int value associated with the specified name from this StateHandler.
			 * If the specified name is not found, or the value returned is not of the correct type
			 * the specified default value will be returned
			 *
			 * @param name the name with which the value is associated
			 * @param def the default value if the specified name cannot be found, or is not the correct type
			 * @return the int value associated with the specified name
			 */
			virtual int getInt(const std::string& name, int def) const ;

			/**
			 * Returns the specified int value associated with the specified name from this StateHandler.
			 * If the specified name is not found, or the value returned is not of the correct type
			 * the specified default value will be returned
			 *
			 * @param name the name with which the value is associated
			 * @param def the default value if the specified name cannot be found, or is not the correct type
			 * @return the int value associated with the specified name
			 */
			virtual double getDouble(const std::string& name, double def) const ;

			/**
			 * Returns the specified int value associated with the specified name from this StateHandler.
			 * If the specified name is not found, or the value returned is not of the correct type
			 * the specified default value will be returned
			 *
			 * @param name the name with which the value is associated
			 * @param def the default value if the specified name cannot be found, or is not the correct type
			 * @return the int value associated with the specified name
			 */
			virtual bool getBool(const std::string& name, bool def) const ;



			/**
			 * Removed the specified name value pair from this StateHandler
			 *
			 * @param name name of the name value pair to remove
			 */
			virtual void removeValue(const std::string& name) ;



			/** node tag for configuration information */
			static const std::string CONFIG_TAG ;

			/** attribute for the configuration node path/name/id */
			static const std::string CONFIG_ID ;

			/** node tag for name-value pairs of configuration information */
			static const std::string PARAM_TAG ;

			/** attribute within tag for the name of the name-value pair */
			static const std::string NAME_ATTR ;

			/** attribute within tag for the value of the name-value pair */
			static const std::string VALUE_ATTR ;

		protected:

			// making XMLStateHandler a friend allows it to create instances of StateNodes
			// we genereally dont want a user to do this
			friend class XMLStateHandler ;

			/**
			 * Constructs a new XMLStateNode with no parent Node and handled XML element.
			 * This constructor does not take a parent node, the contructed StateNode must therefore
			 * be the root node of the State Tree
			 * The actual state information for this StateNode is contained within the specified
			 * xmlpp::Element, this class acts as an interface between the element and the state node.
			 *
			 * @param element the xmlpp::Element which stores the actual data
			 * @param document the xml document we are acting as a state node for
			 * @param childHandler the container to manage constructed state handlers
			 */
			XMLStateNode(xmlpp::Element* element, xmlpp::Document* document, XMLStateHandler::ChildHandleManager& childHandler) ;

			/**
			 * Constructs a new XMLStateNode with the specified parent Node and handled XML element.
			 * The actual state information for this StateNode is contained within the specified
			 * xmlpp::Element, this class acts as an interface between the element and the state node.
			 *
			 * @param path the complete path to the new XMLStateNode
			 * @param parent the parent XMLStateNode
			 * @param element the xmlpp::Element which stores the actual data
			 * @param document the xml document we are acting as a state node for
			 * @param childHandler the container to manage constructed state handlers
			 */
			XMLStateNode(const std::string& path, RefCountPtr<StateNode> parent, xmlpp::Element* element, xmlpp::Document* document, XMLStateHandler::ChildHandleManager& childHandler) ;


		private:

			/**
			 * Returns the child Element from the set values with the specified name
			 * If the name/value pair does not exist, 0 is returned
			 *
			 * @param name the name associated with the value
			 */
			xmlpp::Element* getNameValueElement(const std::string& name) const ;

			/** this StateNodes complete path */
			std::string thePath ;

			/** the parent XMLStateNode to this XMLStateNode */
			RefCountPtr<StateNode> theParent ;

			/** the DOM Element this XMLStateNode wraps */
			xmlpp::Element* theDOMElement ;

			/** the DOM Document this State Node is a handler for */
			xmlpp::Document* theDOMDocument ;

			/** handler to manage constructed StateNodes */
			XMLStateHandler::ChildHandleManager& theChildHandleManager ;

	} ; /* class XMLStateNode */

} /* namespace cutil */

#endif /* _CUTIL_XMLSTATENODE_H_ */
