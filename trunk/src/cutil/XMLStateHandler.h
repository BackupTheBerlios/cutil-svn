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

#ifndef _CUTIL_XMLSTATEHANDLER_H_
#define _CUTIL_XMLSTATEHANDLER_H_

#include <cutil/StateHandler.h>

#include <cutil/RefCountPtr.h>

#include <map>
#include <string>

namespace xmlpp
{
	class Document ;
	class DomParser ;
}


namespace cutil
{
	/**
	 * XML based StateHandler implementation providing a storage method for state information.
	 * This implementation stares and retrieves state information from an XML document
	 *
	 * State data set and accessed through a name/value scheme, where a value is associated
	 * with a specified name. This name can later be used to access the named data value.
	 *
	 * Currently flush() and sync() are not fully implemented and should not be used
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class XMLStateHandler : public StateHandler
	{
		public:
			//---------------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * Construct a new XMLStateHandler to read and store state information within the specified xml filename.
			 * The rootNode may be set within the rootNode parameter to set the root node name for the state information,
			 * if left blank, this will default to XMLStatehandler::ROOT_NODE
			 *
			 * @param filename the file path of the xml data file
			 * @param rootNode root config node name
			 */
			XMLStateHandler(const std::string filename, const std::string rootNode = "") ;

			/**
			 * Destructor
			 */
			virtual ~XMLStateHandler() ;


			//---------------------------------------------------------------------------------------//
			// StateHandler implementations

			/**
			 * Initialize this StateHandler.
			 * This method provides an entry point to initialize a StateHandler. Initialization
			 * may perform some required setup, such as establish a connection to a backing
			 * store, or read a datafile
			 *
			 */
			virtual void initialize() ;

			/**
			 * Shutdown this StateHandler.
			 * This method provides an exit point for a state handler.
			 * This method should flush the current state information in the state tree and perform
			 * any last cleanup operations. This may be to close any open database connections,
			 * files or any other used resources.
			 *
			 */
			virtual void shutdown() ;

			//---------------------------------------------------------------------------------------//
			// Update Control

			/**
			 * Force a write of state information back to the persistant store
			 *
			 */
			virtual void flush() ;

			/**
			 * Synchronize this StateHandler to the persistant store of state information,
			 * This method ensures that this StateHandler has the most up to date information
			 * from the persistant store. This may be useful if another componenet has access
			 * to the backing store and may be making modification independant of this StateHandler
			 *
			 */
			virtual void sync() ;


			//---------------------------------------------------------------------------------------//
			// StateNode Access

			/**
			 * Returns the root StateNode within the state tree of this StateHandler
			 * This method returns a user handle for the underlying data of the StateNode
			 * implementation. Subsequent calls to getRootNode need not return the same
			 * handle instance, however, it is guaranteed that each will be a handle to
			 * the same underlying data instance.
			 *
			 * @return the root StateNode within the state tree of this StateHandler
			 */
			virtual RefCountPtr<StateNode> getRootNode() ;


			/** the root node of the configuration data file */
			static const std::string ROOT_NODE ;
			
			
			/**
			 * Inner class to manage the live nodes we have created and returned
			 *
			 */
			class ChildHandleManager
			{
				public:
					/**
					 * Default Constructor
					 */
					ChildHandleManager() ;

					/**
					 * Destructor
					 */
					~ChildHandleManager() ;
				
					/**
					 * Adds a child handle to the managed handles
					 *
					 * @param path the path of the child state node
					 * @param stateNode the state node handle
					 */
					void addChild(const std::string& path, cutil::RefCountPtr<cutil::StateNode> stateNode) ;

					/**
					 * Removes the managed state node with the specified path
					 * If the child is not managed, no action is taken
					 *
					 * @param path the path of the child state node to remove
					 */
					void removeChild(const std::string& path) ;

					/**
					 * Returns true if a state node is managed for the specified path
					 *
					 * @param path the path of the state node
					 * @return true if a state node is managed for the specified path
					 */
					bool hasChild(const std::string& path) const ;

					/**
					 * Returns the handle for the managed state node with the specified path
					 * If no state node is associated with the specified path, an empty handle
					 * is returned
					 *
					 * @param path the path of the state node
					 * @return the state node for the specified path, or an empty handle if
					 *         no state node is associated with the specified path
					 */
					cutil::RefCountPtr<cutil::StateNode> getChild(const std::string& path) ;

					/**
					 * Removes all managed State node handles
					 * Note this is does not remove the state node, only the reference from
					 * our managed container iss removed.
					 *
					 */
					void clear() ;
					
					/**
					 * Removes any unrequired child references from our child handles container
					 * unrequired references are reference with a ref count of exactly one, ie
					 * the reference within the container.
					 *
					 */
					void cleaupHandleReferences() ;

				protected:
				private:
					/**
					 * Disallow copy constructor
					 */
					ChildHandleManager(const ChildHandleManager& noCopy) {} ;

					/** contained handles typedef */
					typedef std::map<std::string, RefCountPtr<StateNode> > ChildHandleContainer ;

					/** convenienve typedef for pair contained within the contained handles */
					typedef std::pair<std::string, RefCountPtr<StateNode> > ChildHandlePair ;

					/** map to store created child handles */
					ChildHandleContainer theChildHandles ;
			} ;

			//---------------------------------------------------------------------------------------//
		protected:

			/**
			 * Parse the configuration file
			 *
			 * @param filename the location of the file to parse
			 * @return true if the settings file was parsed successfully, false otherwise
			 */
			bool parseConfigurationFile(std::string filename) ;

			//---------------------------------------------------------------------------------------//

		private:
			/**
			 * Creates a default Document if no config file exists.
			 *
			 */
			void createDefaultDocument() ;

			/** the parsed XML state information */
			xmlpp::Document* theConfigDomDocument ;

			/** dom parser, required as member as deleting the parser deletes the document */
			xmlpp::DomParser* theDomParser ;

			/** the root node name for the data */
			std::string theRootNodeName ;

			/** the path of the data file in use */
			std::string theDataFile ;
			
			ChildHandleManager theChildHandleManager ;


	} ; /* class XMLStateHandler */

} /* namespace cutil */

#endif /* _CUTIL_XMLSTATEHANDLER_H_ */
