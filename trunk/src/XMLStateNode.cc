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

#include <cutil/XMLStateNode.h>

#include <cutil/StateNode.h>
#include <cutil/XMLStateHandler.h>

#include <libxml++/document.h>
#include <libxml++/nodes/element.h>
#include <libxml++/nodes/node.h>

#include <sstream>

using cutil::StateNode ;
using cutil::XMLStateNode ;
using namespace xmlpp ;

//---------------------------------------------------------------------------------------//
// Initialize static memebers

const std::string XMLStateNode::CONFIG_TAG = "config_node" ;
const std::string XMLStateNode::CONFIG_ID = "config_id" ;
const std::string XMLStateNode::PARAM_TAG = "config_param" ;
const std::string XMLStateNode::NAME_ATTR = "name" ;
const std::string XMLStateNode::VALUE_ATTR = "value" ;

//---------------------------------------------------------------------------------------//
// Constructor / Desctructor

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
XMLStateNode::XMLStateNode(xmlpp::Element* element, xmlpp::Document* document, XMLStateHandler::ChildHandleManager& childHandler)
	: thePath("/"), theParent(cutil::RefCountPtr<cutil::StateNode>()), theDOMElement(element), theDOMDocument(document), theChildHandleManager(childHandler)
{
	
}

/**
 * Constructs a new XMLStateNode with the specified parent Node and handled XML element.
 * The parent node should be set to 0, if this XMLStateNode is the root node within a
 * state tree.
 * The actual state information for this StateNode is contained within the specified
 * xmlpp::Element, this class acts as an interface between the element and the state node.
 *
 * @param path the complete path to the new XMLStateNode
 * @param parent the parent XMLStateNode, or 0 if this node is the root of the state tree
 * @param element the xmlpp::Element which stores the actual data
 * @param document the xml document we are acting as a state node for
 * @param childHandler the container to manage constructed state handlers
 */
XMLStateNode::XMLStateNode(const std::string& path, RefCountPtr<StateNode> parent, Element* element, Document* document, XMLStateHandler::ChildHandleManager& childHandler)
		: thePath(path), theParent(parent), theDOMElement(element), theDOMDocument(document), theChildHandleManager(childHandler)
{
	// nothing else to do
}

/**
 * Destructor
 */
XMLStateNode::~XMLStateNode()
{
	// nothong else to do
	// ... Elements are owned and so freed by the DOMDocument
}




//---------------------------------------------------------------------------------------//
// StateNode implementations


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
cutil::RefCountPtr<StateNode>
XMLStateNode::getParent()
{
	return(theParent) ;
}

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
cutil::RefCountPtr<StateNode>
XMLStateNode::getChild(std::string childPath)
{
	cutil::RefCountPtr<StateNode> stateNode ;
	
	// first check our child handles container in case we already have an handle
	// a handle may already exist if getChild has previously been called for the same path
	// and the reference has not been released (and cleanedup within this class)
	
	// we need to combine the specified child path with our own path
	// to get the complete path as the ChildHandleManage stores complete paths
	// @note [todo] need to check for extra cahrs here, duplacate /'s etc.
	std::string completePath = getPath() ;
	completePath.append(childPath) ;
	
	if(theChildHandleManager.hasChild(childPath))
	{
		stateNode = theChildHandleManager.getChild(completePath) ;
	}
	else
	{
		// no handle already exists, so we will have to create one
		// possibly creating the node also

		bool foundNode = false ;

		Node::NodeList nodeList = theDOMElement->get_children() ;

		Element* child = 0 ;

		Node::NodeList::const_iterator citer = nodeList.begin() ;
		while((citer != nodeList.end()) && (!foundNode))
		{
			Element* tempElement = (Element*)(*citer) ;
			
			if(tempElement->get_name() == XMLStateNode::CONFIG_TAG)
			{
				Attribute* attr = tempElement->get_attribute(XMLStateNode::CONFIG_ID) ;
				if((attr != 0) && (attr->get_value() == childPath))
				{
					foundNode = true ;
					child = tempElement ;
				}
			}
			
			citer++ ;
		}
		
		if(!foundNode)
		{
			// didnt find the element, so create a new one
			child = theDOMElement->add_child(XMLStateNode::CONFIG_TAG) ;
			child->set_attribute(XMLStateNode::CONFIG_ID, childPath) ;
		}

		if(child != 0)
		{
			// get the StateNode (as handled by a RefCountPtr) for use as the parent
			// of the newly created StateNode
			cutil::RefCountPtr<StateNode> parentHandler = theChildHandleManager.getChild(getPath()) ;
			stateNode.bind(new XMLStateNode(completePath, parentHandler, child, theDOMDocument, theChildHandleManager)) ;

			// since we have created a new handler, we place it into the child container
			// making sure we use the complete path, not just relative to this StateNode
			theChildHandleManager.addChild(completePath, stateNode) ;
		}
	}

	// return the child XMLStateNode wrapping a DOMElement
	return(stateNode) ;
}

/**
 * Removes the specified child StateNode from the state tree.
 * The specified child Statehandler must be below this StateNode within the state tree
 *
 * @param childPath the path to the child StateNode to remove
 */
void
XMLStateNode::removeChild(std::string childPath)
{
	// temporary text buffers
	bool foundNode = false ;
	Node::NodeList nodeList = theDOMElement->get_children() ;

	Element* toRemove = 0 ;

	Node::NodeList::const_iterator citer = nodeList.begin() ;
	while((citer != nodeList.end()) && (!foundNode))
	{
		Element* tempElement = (Element*)(*citer) ;
		if(tempElement->get_name() == XMLStateNode::CONFIG_TAG)
		{
			Attribute* attr = tempElement->get_attribute(XMLStateNode::CONFIG_ID) ;
			if((attr != 0) && (attr->get_value() == childPath))
			{
				foundNode = true ;
				toRemove = tempElement ;
			}
		}
		citer++ ;
	}

	if(toRemove)
	{
		theDOMElement->remove_child(toRemove) ;
	}

	// before we return, we perform some cleanup on child handle reference
	// we dont remove the reference incase there are other references to it. we
	// simply let the handler clean itself automatically
	theChildHandleManager.cleaupHandleReferences() ;
}

/**
 * Returns true of this StateNode contains the specified Child StateNode
 *
 * @param childPath the path of the specified child StateNode
 * @return true if thie StateNode contains th specified StateNode
 */
bool
XMLStateNode::hasChild(const std::string& childPath)
{
	bool ret = false;

	Node::NodeList nodeList = theDOMElement->get_children() ;
	for(Node::NodeList::const_iterator citer = nodeList.begin(); citer != nodeList.end(); ++citer)
	{
		Element* tempElement = static_cast<Element*>(*citer) ;
		if(tempElement->get_name() == XMLStateNode::CONFIG_TAG)
		{
			Attribute* attr = tempElement->get_attribute(XMLStateNode::CONFIG_ID) ;
			if((attr != 0) && (attr->get_value() == childPath))
			{
				ret = true;
			}
		}
	}

	return(ret) ;
}

/**
 * Populate the specified list with all the child StateNode names directly below this StateNode in the state tree
 * The list is not cleared prior to any additions
 *
 * @param childList the list to populate
 * @return the number of elements added to the list
 */
int
XMLStateNode::getChildren(std::list<std::string>& childList)
{
	// track the number of items we add
	int count = 0 ;

	Node::NodeList nodeList = theDOMElement->get_children() ;

	for(Node::NodeList::const_iterator citer = nodeList.begin(); citer != nodeList.end(); ++citer)
	{
		Element* tempElement = (Element*)(*citer) ;

		if(tempElement->get_name() == XMLStateNode::CONFIG_TAG)
		{
			Attribute* attr = tempElement->get_attribute(XMLStateNode::CONFIG_ID) ;
			childList.push_back(attr->get_value()) ;
		}
	}

	// before we return, we perform some cleanup on child handle reference 
	theChildHandleManager.cleaupHandleReferences() ;

	return(count) ;
}

/**
 * Returns the complete path of this StateNode starting at the root '/'
 *
 * @return the complete path of this StateNode
 */
const std::string&
XMLStateNode::getPath() const
{
	return(thePath) ;
}

//---------------------------------------------------------------------------------------//
// State Handling

/**
 * Stores the specified value string within this StateHandler associated with the specified name string
 *
 * @param name name with which the specified value will be associated
 * @param value the string value to store with the specified name
 */
void
XMLStateNode::setString(const std::string& name, const std::string& value)
{
	Element* configParamElem = getNameValueElement(name) ;

	if(configParamElem == 0)
	{
		configParamElem = theDOMElement->add_child(XMLStateNode::PARAM_TAG) ;
	}

	configParamElem->set_attribute(XMLStateNode::NAME_ATTR, name) ;
	configParamElem->set_attribute(XMLStateNode::VALUE_ATTR, value) ;
}

/**
 * Stores the specified int value within this StateHandler associated with the specified name string
 *
 * @param name name with which the specified value will be associated
 * @param value the int value to store with the specified name
 */
void
XMLStateNode::setInt(const std::string& name, int value)
{
	Element* configParamElem = getNameValueElement(name) ;

	if(configParamElem == 0)
	{
		configParamElem = theDOMElement->add_child(XMLStateNode::PARAM_TAG) ;
	}

	configParamElem->set_attribute(XMLStateNode::NAME_ATTR, name) ;
	std::ostringstream buf ;
	buf << value ;
	configParamElem->set_attribute(XMLStateNode::VALUE_ATTR, buf.str()) ;
}

/**
 * Stores the specified double value within this StateHandler associated with the specified name string
 *
 * @param name name with which the specified value will be associated
 * @param value the double value to store with the specified name
 */
void
XMLStateNode::setDouble(const std::string& name, double value)
{
	Element* configParamElem = getNameValueElement(name) ;

	if(configParamElem == 0)
	{
		configParamElem = theDOMElement->add_child(XMLStateNode::PARAM_TAG) ;
	}

	configParamElem->set_attribute(XMLStateNode::NAME_ATTR, name) ;
	std::ostringstream buf ;
	buf << value ;
	configParamElem->set_attribute(XMLStateNode::VALUE_ATTR, buf.str()) ;
}

/**
 * Stores the specified bool value within this StateHandler associated with the specified name string
 *
 * @param name name with which the specified value will be associated
 * @param value the bool value to store with the specified name
 */
void
XMLStateNode::setBool(const std::string& name, bool value)
{
	Element* configParamElem = getNameValueElement(name) ;

	if(configParamElem == 0)
	{
		configParamElem = theDOMElement->add_child(XMLStateNode::PARAM_TAG) ;
	}

	configParamElem->set_attribute(XMLStateNode::NAME_ATTR, name) ;
	configParamElem->set_attribute(XMLStateNode::VALUE_ATTR, (value ? "true" : "false")) ;
}



/**
 * Returns the specified string value associated with the specified name from this StateHandler.
 * If the specified name is not found, the specified default value will be returned
 *
 * @param name the name with which the value is associated
 * @param def the default value if the specified name cannot be found
 * @return the string value associated with the specified name
 */
std::string
XMLStateNode::getString(const std::string& name, const std::string& def) const
{
	std::string ret = def ;

	Element* element = getNameValueElement(name) ;

	if(element != 0)
	{
		Attribute* attr = element->get_attribute(XMLStateNode::VALUE_ATTR) ;
		ret = attr->get_value() ;
	}

	return(ret) ;
}

/**
 * Returns the specified int value associated with the specified name from this StateHandler.
 * If the specified name is not found, or the value returned is not of the correct type
 * the specified default value will be returned
 *
 * @param name the name with which the value is associated
 * @param def the default value if the specified name cannot be found, or is not the correct type
 * @return the int value associated with the specified name
 */
int
XMLStateNode::getInt(const std::string& name, int def) const
{
	int ret = def ;

	Element* element = getNameValueElement(name) ;

	if(element != 0)
	{
		Attribute* attr = element->get_attribute(XMLStateNode::VALUE_ATTR) ;
		std::string s = attr->get_value() ;
		ret = atoi(s.c_str()) ;
	}

	return(ret) ;
}

/**
 * Returns the specified int value associated with the specified name from this StateHandler.
 * If the specified name is not found, or the value returned is not of the correct type
 * the specified default value will be returned
 *
 * @param name the name with which the value is associated
 * @param def the default value if the specified name cannot be found, or is not the correct type
 * @return the int value associated with the specified name
 */
double
XMLStateNode::getDouble(const std::string& name, double def) const
{
	double ret = def ;

	Element* element = getNameValueElement(name) ;

	if(element != 0)
	{
		Attribute* attr = element->get_attribute(XMLStateNode::VALUE_ATTR) ;
		std::string s = attr->get_value() ;
		ret = strtod(s.c_str(), 0) ;
	}

	return(ret) ;
}

/**
 * Returns the specified int value associated with the specified name from this StateHandler.
 * If the specified name is not found, or the value returned is not of the correct type
 * the specified default value will be returned
 *
 * @param name the name with which the value is associated
 * @param def the default value if the specified name cannot be found, or is not the correct type
 * @return the int value associated with the specified name
 */
bool
XMLStateNode::getBool(const std::string& name, bool def) const
{
	bool ret = def ;

	Element* element = getNameValueElement(name) ;

	if(element != 0)
	{
		Attribute* attr = element->get_attribute(XMLStateNode::VALUE_ATTR) ;
		std::string s = attr->get_value() ;
		if(s == "true")
		{
			ret = true ;
		}
		else
		{
			ret = false ;
		}
	}

	return(ret) ;
}



/**
 * Removed the specified name value pair from this StateHandler
 *
 * @param name name of the name value pair to remove
 */
void
XMLStateNode::removeValue(const std::string& name)
{
	Element* element = getNameValueElement(name) ;

	if(element != 0)
	{
		theDOMElement->remove_child(element) ;
	}
}


/**
 * Returns the child Element from the set values with the specified name
 * If the name/value pair does not exist, 0 is returned
 *
 * @param name the name associated with the value
 */
Element*
XMLStateNode::getNameValueElement(const std::string& name) const
{
	Element* ret = 0 ;

	Node::NodeList nodeList = theDOMElement->get_children(XMLStateNode::PARAM_TAG) ;

	for(Node::NodeList::const_iterator citer = nodeList.begin(); citer != nodeList.end(); ++citer)
	{
		Element* tempElement = (Element*)(*citer) ;

		Attribute* attr = tempElement->get_attribute(XMLStateNode::NAME_ATTR) ;

		if((attr != 0) && (attr->get_value() == name))
		{
			ret = tempElement ;

			// break from the for loop
			break ;
		}
	}

	return(ret) ;
}

