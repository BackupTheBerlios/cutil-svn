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

#include <cutil/StateNode.h>

#include <list>
#include <string>

using cutil::StateNode ;


//---------------------------------------------------------------------------------------//
// Constructor / Destructor


/**
 * Protected Constructor to disallow creating instances of this purely abstract class.
 * This class should be implemented to create an object which knows how to persitantly
 * store state information for another object.
 * An implementation may, for example, choose to save state data as an XML file, within
 * a database, or in a local file system.
 *
 */
StateNode::StateNode()
{}


/**
 * Destructor
 */
StateNode::~StateNode()
{}


/**
 * Breaks the specified path into its component segmants and adds each to pathSegments
 *
 * @param path the path to decompose into path segments
 * @param pathSegments the list to add each segment to
 * @return the pathSegments list
 */
std::list<std::string>&
StateNode::breakPath(const std::string& path, std::list<std::string>& pathSegments)
{
	bool done = false ;
	unsigned int marker = 0 ;

	while(!done)
	{
		unsigned int pos = path.find("/", marker) ;

		if(pos == std::string::npos)
		{
			done = true ;
			if(marker != path.length())
			{
				std::string s = path.substr(marker, (path.length() - marker)) ;
				pathSegments.push_back(s) ;
			}
		}
		else
		{
			std::string s = path.substr(marker, pos - marker) ;
			marker = pos + 1 ;

			if(!s.empty())
			{
				pathSegments.push_back(s) ;
			}
		}
	}

	return(pathSegments) ;
}
