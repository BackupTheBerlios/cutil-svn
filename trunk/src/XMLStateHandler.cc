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

#include <cutil/XMLStateHandler.h>

#include <cutil/RefCountPtr.h>
#include <cutil/XMLStateNode.h>

#include <libxml++/document.h>
#include <libxml++/parsers/domparser.h>

#include <exception>
#include <list>
#include <string>

using namespace xmlpp ;
using cutil::XMLStateHandler ;
using cutil::XMLStateNode ;

//---------------------------------------------------------------------------------------//
// Initialize static members
const std::string XMLStateHandler::ROOT_NODE = "state_config" ;

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
XMLStateHandler::XMLStateHandler(const std::string filename, const std::string rootNode) : theDataFile(filename)
{
	theConfigDomDocument = 0 ;
	theDomParser = 0 ;

	if(rootNode.empty())
	{
		theRootNodeName = XMLStateHandler::ROOT_NODE ;
	}
	else
	{
		theRootNodeName = rootNode ;
	}

}

/**
 * Destructor
 */
XMLStateHandler::~XMLStateHandler()
{
	shutdown() ;
}


//---------------------------------------------------------------------------------------//
// StateHandler implementations

/**
 * Initialize this StateHandler.
 * This method provides an entry point to initialize a StateHandler. Initialization
 * may perform some required setup, such as establish a connection to a backing
 * store, or read a datafile
 *
 */
void
XMLStateHandler::initialize()
{
	// simply parse the configuration file
	bool parsed = parseConfigurationFile(theDataFile) ;

	if(parsed)
	{
		// if the document parsed ok, we need to check the root node
		Document* doc = theDomParser->get_document() ;

		// check the root node of the document is what we expect
		// if it is not, we do not have a valid confiuguration file
		Element* rootNode = doc->get_root_node() ;
		bool verified = (rootNode->get_name() == theRootNodeName) ;

		if(verified)
		{
			// we have a valid root node, so assume the document is valid
			// in this case we are required to keep the DomParser, as releasing it
			// also releases our document
			// we should really be able to 'adopt' reposnsibity for the document
			theConfigDomDocument = doc ;
		}
		else
		{
			// the root node does not match, so assume the document is not a valid
			// configuration file
			// in this csae we create a default document, and dipose of the DomParser
			// which in tern will take the 'wrong' document with it

			delete theDomParser ;
			theDomParser = 0 ;

			createDefaultDocument() ;
		}
	}
	else
	{
		// didn't parse, assume the defaults - create a new tree
		createDefaultDocument() ;
	}
}

/**
 * Shutdown this StateHandler.
 * This method provides an exit point for a state handler.
 * This method should flush the current state information in the state tree and perform
 * any last cleanup operations. This may be to close any open database connections,
 * files or any other used resources.
 *
 */
void
XMLStateHandler::shutdown()
{
	if(theConfigDomDocument != 0)
	{
		theChildHandleManager.clear() ;

		try
		{
			// write the configuration to the datafile
			theConfigDomDocument->write_to_file_formatted(theDataFile) ;
		}
		catch(std::exception e)
		{

		}

		// dispose of the DOM document and parser

		// if we have a valid parser, the we simply delete it, which
		// in tern will dispose of the document too
		if(theDomParser)
		{
			delete theDomParser ;
			theDomParser = 0 ;

			//and we simply set the document to null ;
			theConfigDomDocument = 0 ;
		}
		else
		{
			// if we dont have a DomParser, we must have created the document ourself
			// and so we need to delete it
			delete theConfigDomDocument ;
			theConfigDomDocument = 0 ;
		}
	}
}



//---------------------------------------------------------------------------------------//
// Update Control

/**
 * Force a write of state information back to the persistant store
 *
 */
void
XMLStateHandler::flush()
{
	// write the configuration to the datafile
	if(theConfigDomDocument != 0)
	{
		theConfigDomDocument->write_to_file_formatted(theDataFile) ;
	}
}

/**
 * Synchronize this StateHandler to the persistant store of state information,
 * This method ensures that this StateHandler has the most up to date information
 * from the persistant store. This may be useful if another componenet has access
 * to the backing store and may be making modification independant of this StateHandler
 *
 */
void
XMLStateHandler::sync()
{
	// @note [todo] this needs alot more thought
	parseConfigurationFile(theDataFile) ;
}


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
cutil::RefCountPtr<cutil::StateNode>
XMLStateHandler::getRootNode()
{
	cutil::RefCountPtr<cutil::StateNode> rootNode ;

	if(theChildHandleManager.hasChild("/"))
	{
		rootNode = theChildHandleManager.getChild("/") ;
	}
	else
	{
		rootNode.bind(new XMLStateNode(theConfigDomDocument->get_root_node(), theConfigDomDocument, theChildHandleManager)) ;
	}

	return(rootNode) ;
}


/**
 * Parse the configuration file
 *
 * @param filename the location of the file to parse
 * @return true if the settings file was parsed successfully, false otherwise
 */
bool
XMLStateHandler::parseConfigurationFile(std::string filename)
{
	// track our return status
	bool ret = false ;

	if(theConfigDomDocument != 0)
	{
		// we make the assumption here that we can release the current config document
		// and parse a new settings file

		// if we have a DomParser, we simply delete the parser, which in tern
		// removes our doeument, otherwise, we must have created the document
		// and need to delete it ourselves
		if(theDomParser)
		{
			delete theDomParser ;
			theDomParser = 0 ;

			theConfigDomDocument = 0 ;
		}
		else
		{
			delete theConfigDomDocument ;
			theConfigDomDocument = 0 ;
		}
	}

	if(!theDomParser)
	{
		theDomParser = new DomParser() ;
	}

	try
	{
		theDomParser->parse_file(filename) ;
		ret = true ;
	}
	catch(std::exception& ex)
	{
		ret = false ;
	}

	return(ret) ;
}



/**
 * Creates a default Document if no config file exists.
 *
 */
void
XMLStateHandler::createDefaultDocument()
{
	theConfigDomDocument = new Document() ;

	theConfigDomDocument->create_root_node(theRootNodeName) ;
}







//---------------------------------------------------------------------------------------//
// Inner classes
// Inner class used to manage the live nodes we have created and returned

/**
 * Default Constructor
 */
XMLStateHandler::ChildHandleManager::ChildHandleManager()
{

}

/**
 * Destructor
 */
XMLStateHandler::ChildHandleManager::~ChildHandleManager()
{

}

/**
 * Adds a child handle to the managed handles
 *
 * @param path the path of the child state node
 * @param stateNode the state node handle
 */
void
XMLStateHandler::ChildHandleManager::addChild(const std::string& path, cutil::RefCountPtr<cutil::StateNode> stateNode)
{
	theChildHandles.insert(ChildHandlePair(path, stateNode)) ;
	
	cleaupHandleReferences() ;
}

/**
 * Removes the managed state node with the specified path
 * If the child is not managed, no action is taken
 *
 * @param path the path of the child state node to remove
 */
void
XMLStateHandler::ChildHandleManager::removeChild(const std::string& path)
{
	theChildHandles.erase(path) ;
	
	cleaupHandleReferences() ;
}

/**
 * Returns true if a state node is managed for the specified path
 *
 * @param path the path of the state node
 * @return true if a state node is managed for the specified path
 */
bool
XMLStateHandler::ChildHandleManager::hasChild(const std::string& path) const
{
	bool ret = false ;

	ChildHandleContainer::const_iterator citer = theChildHandles.find(path) ;
	if(citer != theChildHandles.end())
	{
		ret = true ;
	}

	return(ret) ;
}

/**
 * Returns the handle for the managed state node with the specified path
 * If no state node is associated with the specified path, an empty handle
 * is returned
 *
 * @param path the path of the state node
 * @return the state node for the specified path, or an empty handle if
 *         no state node is associated with the specified path
 */
cutil::RefCountPtr<cutil::StateNode>
XMLStateHandler::ChildHandleManager::getChild(const std::string& path)
{
	cutil::RefCountPtr<cutil::StateNode> stateNode ;
	
	ChildHandleContainer::const_iterator citer = theChildHandles.find(path) ;
	if(citer != theChildHandles.end())
	{
		stateNode = (*citer).second ;
	}
	
	cleaupHandleReferences() ;
	return(stateNode) ;
}

/**
 * Removes all managed State node handles
 * Note this is does not remove the state node, only the reference from
 * our managed container iss removed.
 *
 */
void
XMLStateHandler::ChildHandleManager::clear()
{
	theChildHandles.clear() ;
}

/**
 * Removes any unrequired child references from our child handles container
 * unrequired references are reference with a ref count of exactly one, ie
 * the reference within the container.
 *
 */
void
XMLStateHandler::ChildHandleManager::cleaupHandleReferences()
{
	ChildHandleContainer::iterator iter = theChildHandles.begin() ;

	while(iter != theChildHandles.end())
	{
		ChildHandleContainer::iterator next = iter ;
		++next ;

		// if the only reference to the state node is the one we have in our container
		// we can remove the reference and allow the smart pointer to delete the state node
		// (the underlying xml node is handled by xmlpp)
		if(((*iter).second).getRefCount() == 1)
		{
			if(((*iter).second)->getPath() != "/")
			{
				theChildHandles.erase(iter) ;
			}
		}

		iter = next ;
	}
}

