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

#ifndef _CUTIL_PLUGINNAMETRANSFORM_
#define _CUTIL_PLUGINNAMETRANSFORM_

#include <string>

namespace cutil
{
	class PluginNameTransform
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * Provides a simple interface for plugin name loopup.
			 * PluginNameTransform provides a simple transofmr on an internal
			 * logical plugin name, returning the physical name of the dynamically loadable
			 * module and plugin name.
			 *
			 */
			PluginNameTransform() {} ;

			/**
			 * Destructor
			 */
			virtual ~PluginNameTransform() {} ;

			//-------------------------------------------------------------------------------//
			// Operations

			struct TransformData
			{
				/** physical dynamically loadable module */
				std::string theModule ;

				/** plugin name within loadable module */
				std::string thePluginName ;
			} ;

			/**
			 * Looks up the logical name of the plugin, returning the physical module and plugin name.
			 * The logical plugin name represents the name of the plugin internal to an application. 
			 * A lookup is performed to map the logical name to a physical name, populating the
			 * physical details within the data parameter and returning true if a mapping exists,
			 * or false if not. The physical information represnts the dynamically loaded module
			 * name and the name of the plugin containe3d within the module.
			 *
			 * @param name logical plugin name
			 * @param data populated with the physicalmodule and plugin name
			 * @return true if the lookup on the logocal name was a success, false otherwise
			 */
			virtual bool lookup(const std::string& name, TransformData& data) = 0 ;

			//-------------------------------------------------------------------------------//

		protected:

			//-------------------------------------------------------------------------------//

		private:

	} ; /* class PluginNameTransform */

} /* namespace cutil */

#endif /* _CUTIL_PLUGINNAMETRANSFORM_ */
