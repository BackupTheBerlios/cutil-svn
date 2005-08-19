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

#ifndef _CUTIL_PLUGINMANAGER_
#define _CUTIL_PLUGINMANAGER_

#include <cutil/PluginInfo.h>
#include <cutil/PluginManagerException.h>
#include <cutil/SharedLibraryException.h>

#include <list>
#include <map>
#include <string>

namespace cutil
{
	class Plugin ;
	class PluginFactory ;
	class PluginNameTransform ;
	class SharedLibrary ;
	template <class T> class PluginHandle ;


	/**
	 * PluginManager provides management of loaded SharedLibraries / Plugins
	 *
	 * The PluginManager maintains a reference count of each Plugin to ensure that
	 * it is safe to delete Plugins and close SharedLibraries. Attempting to
	 * unload a referenced Plugin will result in a PluginManagerException being thrown.
	 * The reference count can be maintained in two ways, by the user, or automatically
	 * through the use of a PluginHandle.
	 * It is preferable to use a PluginHandle whereever possible, to automatically handle
	 * reference counting on behalf of the user, however this approach requires
	 * additional linker flags to be specified during compilation when using g++.
	 * Namely, adding -Wl,E, in order to export the application symbols globally.
	 * The symbols of the SharedLibrary must also be exported Globally.
	 * (See http://gcc.gnu.org/faq.html#dso)
	 * For this reason, access to the reference counting methods remains public,
	 * (although this may change in future versions)
	 * allowing the user to access a Plugin directly through a call to getPlugin
	 * and update the reference counting as required. If the users chooses thie
	 * method, reference count MUST be handled by the user.
	 * A third method exists where the user can construct a PluginHandle themself.
	 * Following a call getPlugin, the returned Plugin can be bopund to a Plugin handle
	 * by casting the Plugin to the appropriate plugin type, and calling bind on the
	 * new PluginHandle passing the new Plugin and PluginManager. In this way
	 * the PluginHandle will aitomatically increment the reference count during binding,
	 * and decremenet the reference count on destruction, allowing the user to forget
	 * about the refence counting. The PluginHandle may then be used as required.
	 *
	 * Plugins are referenced within this PluginManager on two ways.
	 * 1. Using the name of the Plugin (As specified by the Plugin itself) and
	 *    the path of the SharedLibrary from which the Plugin is created
	 * 2. Using an application specific name (which need not match the Plugin
	 *    name), which is tranformed into a Plugin name and SharedLibrary name 
	 *    pair, as specified in 1. This name tranformation is performed using a
	 *    PluginNameTransform instance and may be used to provide run time
	 *    configurability of the loaded Plugins.
	 *
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class PluginManager
	{
		private:
			/**
			 * Internal struct detailing a registered Plugin 
			 */
			struct PluginRecord
			{
				PluginRecord() : thePlugin(0), theRemainLoadedFlag(false) {}

				/** the id of the dynamically loadable module */
				std::string theSharedObjectId ;

				/** infomation describing the plugin */
				PluginInfo thePluginInfo ;

				/** the plugin */
				Plugin* thePlugin ;

				/** indicates if this Plugin should remain loaded despite no references to it */
				bool theRemainLoadedFlag ;

				/** reference count for this Plugin */
				int theRefCount ;
			} ;

			struct SharedObjectRecord
			{
				SharedObjectRecord() : theSharedObject(0), thePluginFactory(0), thePluginCreationCount(0) {}

				/** the shared object from which Plugins are created */
				SharedLibrary* theSharedObject ;

				/** the PluginFactory used to construct actual Plugin instances */
				PluginFactory* thePluginFactory ;

				/** count of the total number of plugins created via the SharedObject */
				int thePluginCreationCount ;
			} ;

			typedef std::multimap<std::string, PluginManager::PluginRecord*> PluginContainer_t ;
			typedef std::pair<std::string, PluginManager::PluginRecord*> PluginContPair_t ;
			typedef std::pair<PluginContainer_t::iterator, PluginContainer_t::iterator> PluginRange_t ;
			typedef std::pair<PluginContainer_t::const_iterator, PluginContainer_t::const_iterator> ConstPluginRange_t ;

			typedef std::map<std::string, SharedObjectRecord*> SharedObjectContainer_t ;

		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * Constructs a new SharedLibrary for the specified modules name.
			 * The SharedLibrary is not opened until the user call open.
			 *
			 */
			PluginManager() ;

			/**
			 * Destructor
			 * Closes all registered and open SharedLibrary
			 *
			 */
			~PluginManager() ;

			//-------------------------------------------------------------------------------//
			// Accessors / Mutators

			/**
			 * Sets the PluginNameTransform used to convert a logical Plugin name to its physical name
			 * The PluginNameTransform remains owned by the caller. Any previously set PluginNameTransform
			 * is returned.
			 *
			 * @param nameTransform the PluginNameTransform to lookup plugin names
			 * @return the previously set PluginNameTransform, if one has been set.
			 */
			PluginNameTransform* setNameTransform(PluginNameTransform* nameTransform) ;

			/**
			 * Removes and returns any set PluginNameTransform
			 *
			 * @return the previously set PluginNameTransform, if one has been set.
			 */
			PluginNameTransform* removeNameTransform() ;

			/**
			 * Removes and returns any set PluginNameTransform
			 *
			 * @return the previously set PluginNameTransform, if one has been set.
			 */
			const PluginNameTransform* getNameTransform() const ;

			/**
			 * Sets whether unloaded Plugins are automatically loaded during named Plugin access
			 * With this flag set, Plugins which are unloaded, or unregistred are automatically
			 * looked up and loaded during a request fora named Plugin. The opposite is to
			 * receive a PluginException on request of an unloade Plugin.
			 *
			 * @param autoload set true to automatically load unloaded Plugins on request
			 */
			void setAutoLoad(bool autoload) ;

			/**
			 * Returns whether Plugins are automatically loaded on named Plugin access.
			 *
			 * @return true if Plugins are automatically loaded on named access, false otherwise.
			 */
			bool getAutoLoad() const ;

			/**
			 * Indicates that the named Plugin should remain loaded despite no references to it.
			 * If set true, the named Plugin will remain loaded despite no references to the
			 * Plugin being used. This avoids the need to (possibly) load the SharedLibrary
			 * containing the Plugin and constructing a new instance.
			 * If set false, the Plugin is released once all references to the Plugin are
			 * released.
			 *
			 * Plugin references are handle internally through the PluginHandle class.
			 *
			 * @param name the Plugin of which to update the remain loaded flag
			 * @param loaded the remain loaded state.
			 * @return true if the named Plugin is registered, false otherwise
			 */
			bool setRemainLoaded(const std::string name, bool loaded) ;

			/**
			 * Returns whether the named Plugin is kept loaded despite no references to it being used.
			 * If the named Plugin is not registred with this PluginManage, false is returned.
			 *
			 * @see setRemainLoaded
			 * @param name the Plugin of which to update the remain loaded flag
			 * @return true if the named Plugin remains loaded, false otherwise.
			 */
			bool getRemainLoaded(const std::string name) ;

			//-------------------------------------------------------------------------------//
			// PluginManager Operations

			/**
			 * Returns a list of the Plugins available through the specified SharedLibrary.
			 * A particular SharedLibrary may contain multiple independant Plugins, this
			 * method may be used to list all Plugins available through a SharedLibrary.
			 * The module parameter represents the filename of filepath of the SharedLibrary
			 * to be loaded, and is searched in the usual system manner.
			 *
			 * @param module the named SharedLibrary
			 * @param pluginList populated with a list of the Plugins available through module
			 * @return pluginList populated
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 */
			std::list<PluginInfo>& getAvailablePlugins(const std::string& module, std::list<PluginInfo>& pluginList) const throw(SharedLibraryException) ;


			// Plugin Registration

			/**
			 * Registers all Plugins contained within the specified SharedLibrary
			 * All Plugins contained within the specified SharedLibrary are registred
			 * with this PluginManager, however, none of the Plugins are actually loaded.
			 * If a Plugin is already registred, or loaded, it is not registred again.
			 * The module paramter represents the filename of filepath of the SharedLibrary
			 * to be loaded, and is searched in the usual system manner.
			 *
			 * @param module the SharedLibrary to load
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 */
			void registerPlugins(const std::string& module) throw(SharedLibraryException, PluginManagerException) ;

			/**
			 * Registers the named plugin within the specified SharedLibrary.
			 * The Plugin is not loaded until loadPlugin is called, or, if
			 * getAutoLoad is true, on first access to the Plugin.
			 * If the Plugin is already registred, or loaded, it is not registred again.
			 * The module paramter represents the filename of filepath of the SharedLibrary
			 * to be loaded, and is searched in the usual system manner.
			 * The name parameter represents the name of a Plugin available through the
			 * specified SharedObject.
			 *
			 * @param module the SharedLibrary to load
			 * @param name the Plugin to load
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 */
			void registerPlugin(const std::string& module, const std::string& name) throw(SharedLibraryException, PluginManagerException) ;

			/**
			 * Registers the application logically named plugin.
			 * The application logical name of the Plugin is transformed using the
			 * registred PluginNameTransform, to obtain the physical SharedLibrary 
			 * name and Plugin name. If no PluginNameTransform
			 * is registred, no tranform can be applied and this method returns false.
			 * The Plugin is not loaded until loadPlugin is called, or, if
			 * getAutoLoad is true, on first access to the Plugin.
			 * If the Plugin is already registred, or loaded, it is not registred again.
			 *
			 * @param name the logical name of the Plugin to load
			 * @return true if a name transform for the Plugin exists, false otherwise.
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 */
			bool registerPlugin(const std::string& name) throw(SharedLibraryException, PluginManagerException) ;

			/**
			 * Returns whether the named plugin has been registred with this PluginManager
			 * The module paramter represents the filename of filepath of the SharedLibrary
			 * to be loaded, and is searched in the usual system manner.
			 * The name parameter represents the name of a Plugin available through the
			 * specified SharedObject.
			 *
			 * @param name the logical name of the Plugin
			 * @return true if the named Plugin has been registred, false otherwise
			 */
			bool isRegistered(const std::string& module, const std::string& name) ;

			/**
			 * Returns whether the logically named plugin has been registred with this PluginManager
			 * The application logical name of the Plugin is transformed using the
			 * registred PluginNameTransform, to obtain the physical SharedLibrary 
			 * name and Plugin name. If no PluginNameTransform
			 * is registred, no tranform can be applied and this method returns false.
			 *
			 * @param name the logical name of the Plugin
			 * @return true if the named Plugin has been registred, false otherwise
			 */
			bool isRegistered(const std::string& name) ;


			// Plugin Loading

			/**
			 * Loads all Plugins contained within the specified SharedLibrary
			 * All Plugins contained within the specified SharedLibrary are loaded
			 * with this PluginManager.
			 * The module paramter represents the filename of filepath of the SharedLibrary
			 * to be loaded, and is searched in the usual system manner.
			 *
			 * @param module the SharedLibrary to load
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 */
			void loadPlugins(const std::string& module) throw(SharedLibraryException, PluginManagerException) ;

			/**
			 * Loads the named plugin within the specified SharedLibrary.
			 * The module paramter represents the filename of filepath of the SharedLibrary
			 * to be loaded, and is searched in the usual system manner.
			 * The name parameter represents the name of a Plugin available through the
			 * specified SharedObject.
			 *
			 * @param module the SharedLibrary to load
			 * @param name the Plugin to load
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 */
			void loadPlugin(const std::string& module, const std::string& name) throw(SharedLibraryException, PluginManagerException) ;

			/**
			 * Loads the application logically named plugin.
			 * The application logical name of the Plugin is transformed using the
			 * registred PluginNameTransform, to obtain the physical SharedLibrary 
			 * name and Plugin name. If no PluginNameTransform
			 * is registred, no tranform can be applied and this method returns false.
			 *
			 * @param name the logical name of the Plugin to load
			 * @return true if a name transform for the Plugin exists, false otherwise.
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 */
			bool loadPlugin(const std::string& name) throw(SharedLibraryException, PluginManagerException) ;


			/**
			 * Returns whether the named plugin has been loaded.
			 * The module paramter represents the filename of filepath of the SharedLibrary
			 * to be loaded, and is searched in the usual system manner.
			 * The name parameter represents the name of a Plugin available through the
			 * specified SharedObject.
			 *
			 * @param name the logical name of the Plugin
			 * @return true if the named Plugin has been loaded, false otherwise
			 */
			bool isLoaded(const std::string& module, const std::string& name) ;

			/**
			 * Returns whether the logically named plugin has been loaded.
			 * The application logical name of the Plugin is transformed using the
			 * registred PluginNameTransform, to obtain the physical SharedLibrary 
			 * name and Plugin name. If no PluginNameTransform
			 * is registred, no tranform can be applied and this method returns false.
			 *
			 * @param name the logical name of the Plugin
			 * @return true if the named Plugin has been loaded, false otherwise
			 */
			bool isLoaded(const std::string& name) ;


			// Plugin Unloading / Unregistering

			/**
			 * Unloads the specified logically named Plugin
			 * The Plugin is unloaded immediatly, ignoring any set remain loaded flag.
			 * If this is the last Plugin from the SharedLibrary containing the Plugin,
			 * the SharedLibrary is also closed.
			 * The Plugin remains in a registred state until unregisterPlugin is called.
			 * If the specified Plugin is currenyl loaded, this method will attempt to
			 * unload the plugin, returning true on success. If the Plugin is not
			 * registered, or has not been loaded, this methodd returns false.
			 * The application logical name of the Plugin is transformed using the
			 * registred PluginNameTransform, to obtain the physical SharedLibrary 
			 * name and Plugin name. If no PluginNameTransform
			 * is registred, no tranform can be applied and this method returns false.
			 * If the named plugin is still referenced, a PluginManagerException
			 * will be thrown, leaving the Plugin in its current loaded state.
			 *
			 * @param name the logical name of the Plugin to unload
			 * @return true if a name transform for the Plugin exists, false otherwise.
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 * @throw PluginManagerException if the Plugin is still referenced
			 */
			bool unloadPlugin(const std::string& name) throw(SharedLibraryException, PluginManagerException) ;

			/**
			 * Unloads the named Plugin
			 * The Plugin is unloaded immediatly, ignoring any set remain loaded flag.
			 * If this is the last Plugin from the SharedLibrary containing the Plugin,
			 * the SharedLibrary is also closed.
			 * The Plugin remains in a registred state until unregisterPlugin is called.
			 * If the named plugin is still referenced, a PluginManagerException
			 * will be thrown, leaving the Plugin in its current loaded state.
			 * If the specified Plugin is currenyl loaded, this method will attempt to
			 * unload the plugin, returning true on success. If the Plugin is not
			 * registered, or has not been loaded, this methodd returns false.
			 *
			 * @param name the logical name of the Plugin to unload
			 * @return true named Plugin was indeed unloaded, false otherwise
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 * @throw PluginManagerException if the Plugin is still referenced
			 */
			bool unloadPlugin(const std::string& module, const std::string& name) throw(SharedLibraryException, PluginManagerException) ;

			/**
			 * Unloads all Plugins
			 * All Plugin is unloaded immediatly, ignoring any set remain loaded flag.
			 * All SharedLibrary from which the Plugins were created, are also closed.
			 * The Plugins remain in a registred state until unregisterPlugin is called.
			 * If any of the registered Plugins is still referenced, a PluginManagerException
			 * will be thrown leaving all Plugins in their current loaded state
			 *
			 * @param name the logical name of the Plugin to unload
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 * @throw PluginManagerException if any of the Plugins are still referenced
			 */
			void unloadAll() throw(SharedLibraryException, PluginManagerException) ;


			/**
			 * Unregisters the specified logically named Plugin
			 * The Plugin is unloaded (if loaded) with a call to unloadPlugin(name), then
			 * unregistred immediatly.
			 * If the Plugin was previously registered, and unregistsred sucessfuly, true
			 * is returned. If the Plugin was not registered, false is returned. 
			 * If no PluginNameTransform is registred, or does not return a Plugin
			 * name, false is returned.
			 * If the named plugin is still referenced, a PluginManagerException
			 * will be thrown, leaving the Plugin in a registred, and possibly loaded state.
			 *
			 * @param name the logical name of the Plugin to unload
			 * @return true if a Plugin was indeed unregistred, false otherwise
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 * @throw PluginManagerException if any of the Plugins are still referenced
			 */
			bool unregisterPlugin(const std::string& name) throw(SharedLibraryException, PluginManagerException) ;

			/**
			 * Unregisters the specified logically named Plugin
			 * The Plugin is unloaded (if loaded) with a call to unloadPlugin(module, name), then
			 * unregistred immediatly.
			 * If the Plugin was previously registered, and unregistsred sucessfuly, true
			 * is returned. If the Plugin was not registered, false is returned. 
			 * If the named plugin is still referenced, a PluginManagerException
			 * will be thrown, leaving the Plugin in a registred, and possibly loaded state.
			 *
			 * @param name the logical name of the Plugin to unload
			 * @return true if a Plugin was indeed unregistred, false otherwise
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 * @throw PluginManagerException if any of the Plugins are still referenced
			 */
			bool unregisterPlugin(const std::string& module, const std::string& name) throw(SharedLibraryException, PluginManagerException) ;

			/**
			 * Unregisters all Plugins
			 * All Plugin are unloaded (if loaded) with a call to unloadAll() 
			 * and unregistered immediatly,
			 * If any of the registered Plugins is still referenced, a PluginManagerException
			 * will be thrown leaving all Plugins in their current loaded and registred state
			 *
			 * @param name the logical name of the Plugin to unload
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 * @throw PluginManagerException if the Plugin is still referenced
			 */
			void unregisterAll() throw(SharedLibraryException, PluginManagerException) ;


			// Plugin Access

			/**
			 * Returns direct access to the named Plugin.
			 * This method returns direct access to the managed Plugin, the user
			 * must update the reference count of the Plugin through a call to refPlugin
			 * and unreference the Plugin with a call to unrefPlugin when complete.
			 * The user should consider using a PluginHandle to automatically handle
			 * reference counting.
			 * The Plugin remains managed by thie PluginManager, the user MUST NOT
			 * attemp to delete the Plugin.
			 *
			 * @param module the SharedLibrary to access
			 * @param name the Plugin to access
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 * @throw PluginManagerException if the Plugin is not loaded, or does not exist
			 * @return the named Plugin
			 * @note [design] we return a pointer here over a reference to allow the user to
			 *       lose their handle to the plugin at any point (by setting their pointer to zero).
			 *       This lets the users unreference their Plugin and call unrefPlugin without
			 *       having to wait for the handle to the plugin to fall out of scope, as would
			 *       happen if we were tp return a reference.
			 */
			cutil::Plugin* getPlugin(const std::string& module, const std::string& name) throw(SharedLibraryException, PluginManagerException) ;

			/**
			 * Returns direct access to the named Plugin.
			 * This method returns direct access to the managed Plugin, the user
			 * must update the reference count of the Plugin through a call to refPlugin
			 * and unreference the Plugin with a call to unrefPlugin when complete.
			 * The user should consider using a PluginHandle to automatically handle
			 * reference counting.
			 * The Plugin remains managed by thie PluginManager, the user MUST NOT
			 * attemp to delete the Plugin.
			 *
			 * @param module the SharedLibrary to access
			 * @param name the Plugin to access
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 * @throw PluginManagerException if the Plugin is not loaded, or does not exist
			 * @return the named Plugin
			 */
			cutil::Plugin* getPlugin(const std::string& name) throw(SharedLibraryException, PluginManagerException) ;

			/**
			 * Returns a PluginHandle for the named Plugin
			 * The PluginHandle may be used just a resular pointer, however the PluginHandle
			 * automatically handles updating reference counting of the Plugin within thie PluginManager 
			 *
			 * @param module the SharedLibrary to access
			 * @param name the Plugin to access
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 * @throw PluginManagerException if the Plugin is not loaded, or does not exist, or the Plugin
			 *        does not mathch the requested type
			 * @return the named Plugin
			 */
			template<class T>
			PluginHandle<T> getPluginHandle(const std::string& module, const std::string& name) throw(SharedLibraryException, PluginManagerException) ;

			/**
			 * Returns a PluginHandle for the named Plugin
			 * The PluginHandle may be used just a resular pointer, however the PluginHandle
			 * automatically handles updating reference counting of the Plugin within thie PluginManager 
			 *
			 * @param module the SharedLibrary to access
			 * @param name the Plugin to access
			 * @throw SharedLibraryException if there is an error accessing the SharedObject
			 * @throw PluginManagerException if the Plugin is not loaded, or does not exist, or the Plugin
			 *        does not mathch the requested type
			 * @return the named Plugin
			 */
			template<class T>
			PluginHandle<T> getPluginHandle(const std::string& name) throw(SharedLibraryException, PluginManagerException) ;


			//-------------------------------------------------------------------------------//
			// Reference Counting
			// @note [design] ideally these should be protected or private, granting the
			//       PluginHandle friend access. However, the use of template methods
			//       to access a PluginHandle required application symbols to be global,
			//       which, for whateveer reason may not be desirable to the user.
			//       for this reason, these methods remain public, for now.
			//       see class description.

			/**
			 * Decrements the use count of the specified Plugin by one.
			 * The reference count of the Plugin is automatically handled
			 * by the PluginHandle class. The Plugin use count of the
			 * ShreadLibrary containing the Plugin is decremented by one.
			 * This method is an internal reference counting method used by
			 * the PluginHandle class to maintain reference counts of used Plugins.
			 *
			 * @param handle the Plugin of which to decrement the reference count
			 */
			void unrefPlugin(const Plugin& plugin) ;

			/**
			 * Increments the use count of the specified Plugin by one.
			 * The reference count of the Plugin is automatically handled
			 * by the PluginHandle class. The Plugin use count of the
			 * ShreadLibrary containing the Plugin is Incremented by one.
			 * This method is an internal reference counting method used by
			 * the PluginHandle class to maintain reference counts of used Plugins.
			 *
			 * @param handle the Plugin of which to increment the reference count 
			 */
			void refPlugin(const Plugin& plugin) ;


			//-------------------------------------------------------------------------------//
			// Iterator Access

			// @todo add iterator access - requires an Adaptor / FunctionIterator template

		protected:

			/**
			 * Disallow copy constructor
			 */
			PluginManager(const PluginManager& pm) { }

			//-------------------------------------------------------------------------------//

		private:

			//
			// nested algorithm classes
			//

			class PluginRecModuleEq : public std::unary_function<PluginContPair_t, bool>
			{
				public:
					PluginRecModuleEq(const std::string& module) ;

					result_type operator() (const argument_type a) const ;
				protected:
				private:
					std::string theModule ;
			} ;

			class PluginRecPluginEq : public std::unary_function<PluginContPair_t, bool>
			{
				public:
					PluginRecPluginEq(const Plugin& plugin) ;

					result_type operator() (const argument_type a) const ;
				protected:
				private:
					const Plugin& thePlugin ;
			} ;

			class PluginInfoNameEq : public std::unary_function<PluginInfo, bool>
			{
				public:
					PluginInfoNameEq(const std::string& name) ;

					result_type operator() (const PluginInfo& a) const ;
				protected:
				private:
					std::string theName ;
			} ;


			//
			// Helper Methods
			//

			/**
			 * Accesses the registered Plugins and returns the PluginRecord for the specified Plugin
			 *
			 * @param module the SharedLibrary of the registred Plugin
			 * @param name the registred Plugin
			 * @return the PluginRecord for the specified Plugin, or 0 if the record cannot be found
			 */
			PluginRecord* getPluginRecord(const std::string& module, const std::string& name) const ;

			/**
			 * Accesses the registered Plugins and returns the PluginRecord for the specified Plugin
			 *
			 * @param plugin the Pluin for which to access the PluginRecord
			 * @return the PluginRecord for the specified Plugin, or 0 if the record cannot be found
			 */
			PluginRecord* getPluginRecord(const Plugin& plugin) const ;

			/**
			 * Accesses the registered SharedLibraries and returns the SharedObjectRecord for the specified SharedLibrary
			 *
			 * @param module the path of the SharedLibrary
			 * @return the SharedObjectRecord for the specified SharedLibrary, or 0 if the record cannot be found
			 */
			SharedObjectRecord* getSharedObjectRecord(const std::string& module) const ;

			/**
			 * Registers the specified SharedLibrary
			 * The SharedLibray is not constructed, loaded, or opened.
			 *
			 * @param module the path of the SharedLibrary to register
			 * @return the SharedObjectRecord for the newly registred SharedLibrary
			 */
			SharedObjectRecord* registerSharedLibrary(const std::string& module) ;

			/**
			 * Returns the SharedLibrary as soecified by its module path
			 * If the specified SharedLibray has not previously been registerd within this
			 * PluginManager, or the instance constructed, it is regustred and create.
			 *
			 * @param module the SharedLibrary path
			 * @return the Shared Library
			 * @throw SharedLibraryException if there is an error accessing the SharedLibrary
			 */
			SharedLibrary* getSharedLibrary(const std::string& modules) throw (SharedLibraryException) ;

			/**
			 * Closes and unloads the specified SharedLibrary,
			 * If there are remaining reference to Plugins constructed from this SharedLibrary,
			 * a PluginManagerException is thrown
			 * If a PluginFactory instance created from the SharedLibrary remains, it is released.
			 *
			 * @param module the SharedLibrary to unload
			 * @throw SharedLibraryException if there is an error accessing the SharedLibrary
			 * @throw PluginManagerException if Plugin references remain constructed from the SharedLibrary 
			 */
			void unloadSharedLibrary(const std::string& module) throw (SharedLibraryException, PluginManagerException) ;

			/**
			 * Constructs a new PluginFactory instance from the specified SharedLibrary.
			 *
			 * @param lib the SharedLibrary from which to create the PluginFactory
			 * @return the newly constructed PluginFactory
			 * @throw SharedLibraryException if there is an error accessing the SharedLibrary
			 */
			PluginFactory* createPluginFactory(SharedLibrary& lib) const throw (SharedLibraryException) ;

			/**
			 * Releases the specified PluginFactory created from the specified SharedLibrary.
			 *
			 * @param lib the SharedLibrary from the PluginFactory was created
			 * @throw SharedLibraryException if there is an error accessing the SharedLibrary
			 */
			void releasePluginFactory(SharedLibrary& lib, PluginFactory* factory) const throw (SharedLibraryException) ;

			/**
			 * Returns the PluginFactory for the specified SharedLibrary.
			 * If no PluginFactory curreny exists, it is created and added to the
			 * approproate SharedObjectRecord for future use.
			 *
			 * @param module the SharedLIbrary from which to obtain the PluginFactory
			 * @return the PluginFactory for the specified SharedLIbrary
			 * @throw SharedLibraryException if there is an error accessing the SharedLibrary
			 */
			PluginFactory* getPluginFactory(const std::string& module) throw (SharedLibraryException) ;







			//
			// member data
			//

			/** contains the currently registred plugin data */
			PluginContainer_t thePlugins ;

			/** contains all opened shared objects */
			SharedObjectContainer_t theSharedObjects ;

			/** indicates if Plugins are automatically loaded during an access request */
			bool theAutoLoadFlag ;

			/** used to transform logical Plugins names to physical modules/Plugin names */
			PluginNameTransform* theNameTransform ;

			// typedefs for function pointer to create and destroy a PluginFactory
			typedef PluginFactory* (*createPluginFactoryFunc)(void) ;
			typedef void (*releasePluginFactoryFunc)(PluginFactory*) ;


	} ; /* class SharedLibrary */


	/**
	 * PluginHandle provides a smart handle to a Plugin.
	 * The PluginHandle aurtomatically handling reference and unreferencing the Plugin
	 * within the managaing PluginManager.
	 * 
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	template <class T>
	class PluginHandle
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * Construct a PluginHandle which does not initially contain a Plugin or PluginManager
			 */
			PluginHandle() ;

			/**
			 * Constructs a new PluginHandle for the specified Plugin
			 * Construction of the PluginHandle automatically increments the reference
			 * count of the Plugin within the managing PluginHandle.
			 *
			 * @param plugin the Plugin to handle
			 * @param pluginManager the PluginManager managing the Plugin
			 * @throw PluginManagerException if the specified PluginManager does not manage
			 *        the specified Plugin
			 */
			PluginHandle(T& plugin, PluginManager& pluginManager) throw (PluginManagerException) ;

			/**
			 * Constructs a copy of the specified PluginHandle.
			 * The Plugin and PluginManager of handle are copied to this PluginHandle,
			 * incrementing the reference count of the Plugin if isValid()
			 *
			 */
			PluginHandle(const PluginHandle<T>& handle) ;

			/**
			 * Destructor, If isValid(), decrements the reference count of the handled Plugin.
			 *
			 */
			~PluginHandle() ;


			//---------------------------------------------------------------------------------------//
			// Accessors / Mutators

			/**
			 * Returns a pointer to the handled Plugin
			 *
			 * @return a pointer to the handled Plugin
			 */ 
			T* operator->() const ;

			/**
			 * Binds the specified Plugin and PluginManager to this PluginHandle
			 * If this PluginHandle previously held a handle to a Plugin, its reference count
			 * within the managing PluginManager is first decremented. The new Plugin is
			 * then bound, and if isValid(), increments the reference count of the new Plugin
			 *
			 * @param plugin the Plugin to handle
			 * @param pluginManager the PluginManager managing the Plugin
			 * @throw PluginManagerException if the specified PluginManager does not manage
			 *        the specified Plugin
			 */
			void bind(T& plugin, PluginManager& pluginManager) throw (PluginManagerException) ;

			/**
			 * Clears this PluginHandle of its handle to a Plugin.
			 * If this PluginHandle isValid(), the reference count of the Plugin
			 * is first decremented. After a call to clear, this Plugin handle
			 * is in an invalid state, so cannot be used until a new Plugin is
			 * bound with a call to bind.
			 *
			 */
			void clear() ;

			/**
			 * Returns whether this PluginHandle is in a valid state,
			 * A valid PluginHandle will contain a valid handle to a Plugin
			 * and PluginManager.
			 * isValid should be called prior to using the operator-> to verify
			 * this PluginHandle contains a valid, and usable Plugin.
			 *
			 * @return true if this PluginHandle is in a valid state, false otherwise
			 */
			bool isValid() const ;

			/**
			 * Compares this PluginHandle the specified PluginHandle for equality.
			 * Two PluginHandle are equal if they both handle the same Plugin from
			 * the same PluginManager.
			 *
			 * @param handle the PluginHandle to compare to this for equlity
			 */
			bool operator==(const PluginHandle<T>& handle) const ;

			/**
			 * Compares this PluginHandle the specified PluginHandle for inequality.
			 * Two PluginHandle are not equal if they handle the different Plugins, either
			 * from the same, or dirreferent PluginManager, or the same Plugin from
			 * differeing PluginManagers.
			 *
			 * @param handle the PluginHandle to compare to this for inequlity
			 */
			bool operator!=(const PluginHandle<T>& handle) const ;

			//---------------------------------------------------------------------------------------//

		protected:

			//---------------------------------------------------------------------------------------//

		private:

			/** the (possibly derived) Plugin */
			T* thePlugin ;

			/** The PluginManager managing the Plugin */
			PluginManager* theManager ;
	} ;




	//---------------------------------------------------------------------------------------//
	// Template Implementation


	template<class T>
	PluginHandle<T>
	PluginManager::getPluginHandle(const std::string& module, const std::string& name) throw(SharedLibraryException, PluginManagerException)
	{
		PluginHandle<T> handle ;

		Plugin* plugin = getPlugin(module, name) ;
		if(plugin)
		{
			T* derived = dynamic_cast<T*>(plugin) ;
			if(derived)
			{
				handle.bind(*derived, *this) ;
			}
		}

		return(handle) ;
	}

	template<class T>
	PluginHandle<T>
	PluginManager::getPluginHandle(const std::string& name) throw(SharedLibraryException, PluginManagerException)
	{
		PluginHandle<T> handle ;
		Plugin* plugin = getPlugin(name) ;

		if(plugin)
		{
			T* derived = dynamic_cast<T*>(plugin) ;
			if(derived)
			{
				handle.bind(*derived, *this) ;
			}
		}

		return(handle) ;
	}





	//---------------------------------------------------------------------------------------//
	// PluginHandle Template Implementation


	template <class T>
	PluginHandle<T>::PluginHandle()
			: thePlugin(0), theManager(0)
	{}

	template <class T>
	PluginHandle<T>::PluginHandle(T& plugin, PluginManager& pluginManager) throw (PluginManagerException)
			: thePlugin(&plugin), theManager(&pluginManager)
	{
		theManager->refPlugin(*thePlugin) ;
	}

	template <class T>
	PluginHandle<T>::PluginHandle(const PluginHandle<T>& handle)
			: thePlugin(handle.thePlugin), theManager(handle.theManager)
	{
		if(isValid())
		{
			theManager->refPlugin(*thePlugin) ;
		}
	}

	template <class T>
	PluginHandle<T>::~PluginHandle()
	{
		if(isValid())
		{
			theManager->unrefPlugin(*thePlugin) ;
		}
	}

	template <class T>
	T* PluginHandle<T>::operator->() const
	{
		return(thePlugin) ;
	}

	template <class T>
	void PluginHandle<T>::bind(T& plugin, PluginManager& pluginManager) throw (PluginManagerException)
	{
		if(isValid())
		{
			theManager->unrefPlugin(*thePlugin) ;
		}

		thePlugin = &plugin ;
		theManager = &pluginManager ;

		if(isValid())
		{
			theManager->refPlugin(*thePlugin) ;
		}
	}

	template <class T>
	void PluginHandle<T>::clear()
	{
		if(isValid())
		{
			theManager->unrefPlugin(*thePlugin) ;
			thePlugin = 0 ;
			theManager = 0 ;
		}
	}

	template <class T>
	bool PluginHandle<T>::isValid() const
	{
		return((thePlugin != 0) && (theManager != 0)) ;
	}

	template <class T>
	bool PluginHandle<T>::operator==(const PluginHandle<T>& handle) const
	{
		return((this->thePlugin == handle.thePlugin) && (this->theManager == handle.theManager)) ;
	}

	template <class T>
	bool PluginHandle<T>::operator!=(const PluginHandle<T>& handle) const
	{
		return((this->thePlugin != handle.thePlugin) && (this->theManager != handle.theManager)) ;
	}


} /* namespace cutil */

#endif /* _CUTIL_PLUGINMANAGER_ */
