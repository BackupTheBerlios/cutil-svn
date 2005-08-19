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

#ifndef _CUTIL_PLUGINFACTORY_
#define _CUTIL_PLUGINFACTORY_

#include <cutil/SharedLibraryException.h>

#include <list>
#include <string>

namespace cutil
{
	// forward declaration
	class Plugin ;
	class PluginInfo ;

	/**
	 * PluginFactory provides the creation point for various plugin held within a dynamically loadable module
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class PluginFactory
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * Constructs a new PluginFactory to construvt named Plugins
			 *
			 */
			PluginFactory() {} ;

			/**
			 * Destructor
			 */
			virtual ~PluginFactory() {} ;


			//-------------------------------------------------------------------------------//
			// Accessors

			/**
			 * Returns the number of plugin available through this PluginFactory
			 *
			 * @return the number of Plugins available through this PluginFactory
			 */
			virtual int getPluginCount() const = 0 ;

			/**
			 * Populates the specified std::list with the names of each Plugin available through this PluginFactory
			 *
			 * @param pluginList std::list to be populated with available Plugin names
			 * @param the pluginList parameter populated with available Plugin names
			 */
			virtual std::list<PluginInfo>& getAvailablePlugins(std::list<PluginInfo>& pluginList) const = 0 ;


			//-------------------------------------------------------------------------------//
			// PluginFactory Operations

			/**
			 * Constructs a new instance of the named Plugin.
			 * If the named Plugin is available through this PluginFactory, a new instance of the
			 * named Plugin is created and returned, otherwise a SharedLibraryException.
			 * Ownership of the newly created Plugin is passes to the caller. The caller must assume
			 * responsibility for releasing the resources of the constructed plugin once it is no longer
			 * required. The destroyPlugin method exists to safely release the resources of a Plugin
			 * constructed with this PluginFactory.
			 *
			 * @param name the name of the Plugin to create
			 * @throw SharedLibraryException if the named plugin is not availble through this PluginFactory
			 * @return the newly created Plugin Object.
			 */
			virtual Plugin* createPlugin(const std::string& name) const throw(SharedLibraryException) = 0 ;

			/**
			 * Releases the resources of the specified Plugin created with a call to createPlugin on this PluginFactory 
			 * This method exists to safely release resources of the specified Plugin, where the Plugin may have
			 * been created with options unknown to the caller of createPlugin.
			 *
			 * @param plugin the Plugin to release
			 * @throw SharedLibraryException if the Plugin cannot be released.
			 */
			virtual void destroyPlugin(Plugin* plugin) const throw(SharedLibraryException) = 0 ;

		protected:

			//-------------------------------------------------------------------------------//

		private:


	} ; /* class PluginFactory */

} /* namespace cutil */

#endif /* _CUTIL_PLUGINFACTORY_ */
