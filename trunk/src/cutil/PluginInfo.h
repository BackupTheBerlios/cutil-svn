/*
 * Copyright (C) 2005  Colin Law
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

#ifndef _CUTIL_PLUGININFO_
#define _CUTIL_PLUGININFO_

#include <string>

namespace cutil
{
	class PluginInfo
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			PluginInfo() ;
			PluginInfo(const std::string& name, const std::string& type, const std::string description) ;
			~PluginInfo() ;

			//-------------------------------------------------------------------------------//
			// Accessors

			/**
			 * Returns the name of the Plugin
			 *
			 * @return the name of the plugin
			 */
			const std::string& getName() const ;

			/**
			 * Returns a string representation of the plugin type
			 *
			 * @return the plugin type
			 */
			const std::string& getType() const ;

			/**
			 * Returns a description of the plugin
			 *
			 * @return description of the plugin
			 */
			const std::string& getDescription() const ;


			//-------------------------------------------------------------------------------//

		protected:

			//-------------------------------------------------------------------------------//

		private:
			/** the plugin name */
			std::string theName ;

			/** the plugin type */
			std::string theType ;

			/** the plugin description */
			std::string theDescription ;

	} ; /* class PluginInfo */

} /* namespace cutil */

#endif /* _CUTIL_PLUGININFO_ */
