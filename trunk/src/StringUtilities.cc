/*
 * Copyright (C) 2006  Colin Law
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
 *
 * $Id$
 */

#include <cutil/StringUtilities.h>

using cutil::StringUtilities ;

/**
 * Splits the string str into tokens using the individual characters of delim as delimeters.
 *
 * @param str string to split
 * @param delim set of characters on which to split str
 * @param tokens populated with the results of str, split on delim
 * @return tokens parameter populated with the results of str, split on delim
 */
std::vector<std::string>&
StringUtilities::Split(const std::string& str, const std::string& delim, std::vector<std::string>& tokens)
{
	std::string::size_type last_pos = str.find_first_not_of(delim, 0) ;
	std::string::size_type pos = str.find_first_of(delim, last_pos) ;

	while(pos != std::string::npos || last_pos != std::string::npos)
	{
		tokens.push_back(str.substr(last_pos, pos - last_pos)) ;
		last_pos = str.find_first_not_of(delim, pos) ;
		pos = str.find_first_of(delim, last_pos) ;
	}

	return(tokens) ;
}

