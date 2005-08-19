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

#ifndef _CUTIL_PLUGIN_
#define _CUTIL_PLUGIN_

#include <cutil/PluginInfo.h>

#include <string>

namespace cutil
{
	/**
	 * Base Plugin class within the plugin framework
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class Plugin
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * Constructs a new Plugin.
			 *
			 */
			Plugin() {} ;

			/**
			 * Destructor
			 */
			virtual ~Plugin() {} ;


			//-------------------------------------------------------------------------------//
			// Accessors

			/**
			 * Returns the PluginInfo object describing this Plugin
			 *
			 * @return the PluginInfo describing this Plugin
			 */
			virtual const PluginInfo& getInfo() const = 0 ;


			//-------------------------------------------------------------------------------//

		protected:

			//-------------------------------------------------------------------------------//

		private:


	} ; /* class Plugin */

} /* namespace cutil */

#endif /* _CUTIL_PLUGIN_ */
