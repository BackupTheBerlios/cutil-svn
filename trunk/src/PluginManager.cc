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

#include <cutil/PluginManager.h>

#include <cutil/Plugin.h>
#include <cutil/PluginInfo.h>
#include <cutil/PluginFactory.h>
#include <cutil/PluginNameTransform.h>
#include <cutil/PluginManagerException.h>
#include <cutil/SharedLibrary.h>
#include <cutil/SharedLibraryException.h>

#include <functional>
#include <iostream>
#include <sstream>
#include <string>

using cutil::PluginInfo ;
using cutil::PluginFactory ;
using cutil::PluginManager ;
using cutil::PluginNameTransform ;
using cutil::SharedLibrary ;

//-------------------------------------------------------------------------------//
// Constructor / Desctructor

/**
 * Constructs a new SharedLibrary for the specified modules name.
 * The SharedLibrary is not opened until the user call open.
 *
 */
PluginManager::PluginManager()
{
	theAutoLoadFlag = false ;
	theNameTransform = 0 ;
}

/**
 * Destructor
 * Closes all registered and open SharedLibrary
 *
 */
PluginManager::~PluginManager()
{
	try
	{
		unregisterAll() ;
	}
	catch(PluginManagerException& pme)
	{
		std::cerr << "Error in PluginManager destructor: " << pme.toString() << std::endl ;
	}
	catch(SharedLibraryException& sle)
	{
		std::cerr << "Error in PluginManager destructor: " << sle.toString() << std::endl ;
	}
}

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
PluginNameTransform*
PluginManager::setNameTransform(PluginNameTransform* nameTransform)
{
	PluginNameTransform* prev = theNameTransform ;
	theNameTransform = nameTransform ;
	return(prev) ;
}

/**
 * Removes and returns any set PluginNameTransform
 *
 * @return the previously set PluginNameTransform, if one has been set.
 */
PluginNameTransform*
PluginManager::removeNameTransform()
{
	return(setNameTransform(0)) ;
}

/**
 * Removes and returns any set PluginNameTransform
 *
 * @return the previously set PluginNameTransform, if one has been set.
 */
const PluginNameTransform*
PluginManager::getNameTransform() const
{
	return(theNameTransform) ;
}

/**
 * Sets whether unloaded Plugins are automatically loaded during named Plugin access
 * With this flag set, Plugins which are unloaded, or unregistred are automatically
 * looked up and loaded during a request fora named Plugin. The opposite is to
 * receive a PluginException on request of an unloade Plugin.
 *
 * @param autoload set true to automatically load unloaded Plugins on request
 */
void
PluginManager::setAutoLoad(bool autoload)
{
	theAutoLoadFlag = autoload ;
}

/**
 * Returns whether Plugins are automatically loaded on named Plugin access.
 *
 * @return true if Plugins are automatically loaded on named access, false otherwise.
 */
bool
PluginManager::getAutoLoad() const
{
	return(theAutoLoadFlag) ;
}

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
bool
PluginManager::setRemainLoaded(const std::string name, bool loaded)
{
	bool ret = false ;

	if(theNameTransform)
	{
		PluginNameTransform::TransformData d ;
		if(theNameTransform->lookup(name, d))
		{
			PluginRecord* plugRec = getPluginRecord(d.theModule, d.thePluginName) ;
			if(plugRec)
			{
				plugRec->theRemainLoadedFlag = loaded ;
				ret = true ;
			}
		}
	}

	return(ret) ;
}

/**
 * Returns whether the named Plugin is kept loaded despite no references to it being used.
 * If the named Plugin is not registred with this PluginManage, false is returned.
 *
 * @see setRemainLoaded
 * @param name the Plugin of which to update the remain loaded flag
 * @return true if the named Plugin remains loaded, false otherwise.
 */
bool
PluginManager::getRemainLoaded(const std::string name)
{
	bool ret = false ;

	if(theNameTransform)
	{
		PluginNameTransform::TransformData d ;
		if(theNameTransform->lookup(name, d))
		{
			PluginRecord* plugRec = getPluginRecord(d.theModule, d.thePluginName) ;
			if(plugRec)
			{
				ret = plugRec->theRemainLoadedFlag ;
			}
		}
	}

	return(ret) ;
}

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
std::list<PluginInfo>&
PluginManager::getAvailablePlugins(const std::string& module, std::list<PluginInfo>& pluginList) const throw(SharedLibraryException)
{
	SharedObjectRecord* soRec = getSharedObjectRecord(module) ;

	if(soRec && soRec->theSharedObject)
	{
		// remeber varuious states so we can leave everything as it was
		// before getAvailablePlugins was called
		bool soOpen = soRec->theSharedObject->isOpen() ;
		bool hasFactory = soRec->thePluginFactory ? true : false ;

		if(!soOpen)
		{
			// may throw
			soRec->theSharedObject->open() ;
		}

		PluginFactory* factory = 0 ;
		if(!hasFactory)
		{
			factory = createPluginFactory(*(soRec->theSharedObject)) ;
		}
		else
		{
			factory = soRec->thePluginFactory ;
		}

		if(factory)
		{
			factory->getAvailablePlugins(pluginList) ;

			if(!hasFactory)
			{
				// safely release the facory object
				releasePluginFactory(*(soRec->theSharedObject), factory) ;

				// only close if we dont have a factory.
				if(!soOpen)
				{
					// may throw
					soRec->theSharedObject->close() ;
				}
			}
		}
	}
	else // !soRec or !soRec->theSharedObject
	{
		SharedLibrary lib(module) ;
		lib.open() ;

		PluginFactory* factory = createPluginFactory(lib) ;
		if(factory)
		{
			// get the PluginFactor
			factory->getAvailablePlugins(pluginList) ;

			// safely release the facory object
			releasePluginFactory(lib, factory) ;
		}
		lib.close() ;
	}

	return(pluginList) ;
}


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
void
PluginManager::registerPlugins(const std::string& module) throw(SharedLibraryException, PluginManagerException)
{
	std::list<PluginInfo> pluginList ;
	getAvailablePlugins(module, pluginList) ;

	for(std::list<PluginInfo>::const_iterator citer = pluginList.begin(); citer != pluginList.end(); ++citer)
	{
		registerPlugin(module, citer->getName()) ;
	}
}

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
void
PluginManager::registerPlugin(const std::string& module, const std::string& name) throw(SharedLibraryException, PluginManagerException)
{
	// is the plugin already registred?
	PluginRecord* prec = getPluginRecord(module, name) ;

	if(!prec)
	{
		// verifyt the SharedLibrary contains the Plugin
		std::list<PluginInfo> pluginList ;

		// may throw
		getAvailablePlugins(module, pluginList) ;

		PluginInfoNameEq pred(name) ;
		std::list<PluginInfo>::const_iterator citer = find_if(pluginList.begin(), pluginList.end(), pred) ;
		if(citer != pluginList.end())
		{
			prec = new PluginRecord() ;
			prec->theSharedObjectId = module ;
			prec->thePluginInfo = *citer ;
			prec->theRemainLoadedFlag = false ;
			prec->theRefCount = 0 ;

			thePlugins.insert(PluginContPair_t(name, prec)) ;
		}
		else
		{
			// SharedLibrary does not contain the specified Plugin name
			std::ostringstream buf ;
			buf << "Cannot register Plugin, SharedLibrary does not contain specified plugin [module=" << module << ",plugin=" << name << "]" ;
			throw(PluginManagerException(buf.str())) ;
		}
	}
	// else
	// ... already registered
}

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
bool
PluginManager::registerPlugin(const std::string& name) throw(SharedLibraryException, PluginManagerException)
{
	bool found = false ;

	if(theNameTransform)
	{
		PluginNameTransform::TransformData data ;
		if(theNameTransform->lookup(name, data))
		{
			found = true ;
			registerPlugin(data.theModule, data.thePluginName) ;
		}
	}
	else
	{
		throw(PluginManagerException("No PluginNameTransform available")) ;
	}

	return(found) ;
}

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
bool
PluginManager::isRegistered(const std::string& module, const std::string& name)
{
	bool registered = false ;
	PluginRecord* prec = getPluginRecord(module, name) ;

	if(prec)
	{
		registered = true ;
	}

	return(registered) ;
}

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
bool
PluginManager::isRegistered(const std::string& name)
{
	bool registered = false ;

	if(theNameTransform)
	{
		PluginNameTransform::TransformData data ;
		if(theNameTransform->lookup(name, data))
		{
			registered = isRegistered(data.theModule, data.thePluginName) ;
		}
	}
	else
	{
		throw(PluginManagerException("No PluginNameTransform available")) ;
	}

	return(registered) ;
}


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
void
PluginManager::loadPlugins(const std::string& module) throw(SharedLibraryException, PluginManagerException)
{
	std::list<PluginInfo> pluginList ;

	// accesses SharedLibrary, may throw
	getAvailablePlugins(module, pluginList) ;

	for(std::list<PluginInfo>::const_iterator citer = pluginList.begin(); citer != pluginList.end(); ++citer)
	{
		loadPlugin(module, citer->getName()) ;
	}
}

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
void
PluginManager::loadPlugin(const std::string& module, const std::string& name) throw(SharedLibraryException, PluginManagerException)
{
	if(!isRegistered(module, name))
	{
		registerPlugin(module, name) ;
	}

	PluginRecord* prec = getPluginRecord(module, name) ;

	if(prec)
	{
		// is the plugin already loaded?
		if(!prec->thePlugin)
		{
			// automatically maintains a pointer to the PluginFactory
			// within the SharedObjectRecord
			PluginFactory* factory = getPluginFactory(module) ;
			if(factory)
			{
				Plugin* plugin = factory->createPlugin(name) ;
				if(plugin)
				{
					prec->thePlugin = plugin ;

					// initial reference for the reference this PluginManager holds
					refPlugin(*plugin) ;
				}
				else
				{
					std::ostringstream buf ;
					buf << "Unexpected error creating Plugin [plugin=" << name << ",module=" << module << "]" ;
					throw(PluginManagerException(buf.str())) ;
				}
			}
			else
			{
				std::ostringstream buf ;
				buf << "Unexpected error accessing PluginFactory [plugin=" << name << ",module=" << module << "]" ;
				throw(PluginManagerException(buf.str())) ;
			}
		}
		// else
		// ... Plugin already loaded
	}
	else
	{
		std::ostringstream buf ;
		buf << "Plugin does not exists [module=" << module << ",plugin=" << name << "]" ;
		throw(PluginManagerException(buf.str())) ;
	}
}

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
bool
PluginManager::loadPlugin(const std::string& name) throw(SharedLibraryException, PluginManagerException)
{
	bool found = false ;

	if(theNameTransform)
	{
		PluginNameTransform::TransformData data ;
		if(theNameTransform->lookup(name, data))
		{
			found = true ;
			loadPlugin(data.theModule, data.thePluginName) ;
		}
	}
	else
	{
		throw(PluginManagerException("No PluginNameTransform available")) ;
	}

	return(found) ;
}


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
bool
PluginManager::isLoaded(const std::string& module, const std::string& name)
{
	bool loaded = false ;

	PluginRecord* prec = getPluginRecord(module, name) ;

	if(prec)
	{
		loaded = prec->thePlugin ;
	}

	return(loaded) ;
}

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
bool
PluginManager::isLoaded(const std::string& name)
{
	bool loaded = false ;

	if(theNameTransform)
	{
		PluginNameTransform::TransformData data ;

		if(theNameTransform->lookup(name, data))
		{
			loaded = isLoaded(data.theModule, data.thePluginName) ;
		}
	}

	return(loaded) ;
}


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
bool
PluginManager::unloadPlugin(const std::string& name) throw(SharedLibraryException, PluginManagerException)
{
	bool found = false ;

	if(theNameTransform)
	{
		PluginNameTransform::TransformData data ;
		if(theNameTransform->lookup(name, data))
		{
			found = true ;
			unloadPlugin(data.theModule, data.thePluginName) ;
		}
	}
	else
	{
		throw(PluginManagerException("No PluginNameTransform available")) ;
	}

	return(found) ;
}

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
bool
PluginManager::unloadPlugin(const std::string& module, const std::string& name) throw(SharedLibraryException, PluginManagerException)
{
	bool ret = false ;

	if(isLoaded(module, name))
	{
		// we must have exactly one reference remaining to the Plugin,
		// the single reference contained within the PluginRecord struct,
		// if this is not the case, we cannot release the Plugin

		PluginRecord* rec = getPluginRecord(module, name) ;
		if(rec)
		{
			if(rec->theRefCount == 1)
			{
				// we need to delete the Plugin using the PluginFactory to ensure
				// we safely delete it
				PluginFactory* factory = getPluginFactory(module) ;

				if(factory)
				{
					ret = true ;

					unrefPlugin(*(rec->thePlugin)) ;
					factory->destroyPlugin(rec->thePlugin) ;
					rec->thePlugin = 0 ;

					// was this Plugin the last from the Shared Object?
					SharedObjectRecord* soRec = getSharedObjectRecord(module) ;
					if(soRec->thePluginCreationCount == 0)
					{
						unloadSharedLibrary(module) ;
					}
				}
				else
				{
					// references remain to the plugin!
					std::ostringstream buf ;
					buf << "Cannot release plugin, cannot access PluginFactory [module=" << module << ",name=" << name << "]" ;
					throw(PluginManagerException(buf.str())) ;
				}
			}
			else
			{
				// references remain to the plugin!
				std::ostringstream buf ;
				buf << "Cannot release plugin while references remain [module=" << module << ",name=" << name << "]" ;
				throw(PluginManagerException(buf.str())) ;
			}
		}
		else
		{
			// shouldn't happen, we check the Plugin is loaded, so the record
			// must exist
			// @note[design] we have no internal locking at this stage, so this
			//               assumes non-multi-threaded

			std::ostringstream buf ;
			buf << "Internal error, cannot access Plugin Record [module=" << module << ",name=" << name << "]" ;
			throw(PluginManagerException(buf.str())) ;
		}
	}

	return(ret) ;
}

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
void
PluginManager::unloadAll() throw(SharedLibraryException, PluginManagerException)
{
	PluginContainer_t::iterator iter = thePlugins.begin() ;
	for(PluginContainer_t::const_iterator citer = thePlugins.begin(); citer != thePlugins.end(); ++citer)
	{
		PluginRecord* prec = citer->second ;
		unloadPlugin(prec->theSharedObjectId, citer->first) ;
	}
}


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
bool
PluginManager::unregisterPlugin(const std::string& name) throw(SharedLibraryException, PluginManagerException)
{
	bool found = false ;

	if(theNameTransform)
	{
		PluginNameTransform::TransformData data ;
		if(theNameTransform->lookup(name, data))
		{
			found = true ;
			unregisterPlugin(data.theModule, data.thePluginName) ;
		}
	}
	else
	{
		throw(PluginManagerException("No PluginNameTransform available")) ;
	}

	return(found) ;
}

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
bool
PluginManager::unregisterPlugin(const std::string& module, const std::string& name) throw(SharedLibraryException, PluginManagerException)
{
	bool ret = false ;

	if(isRegistered(module, name))
	{
		if(isLoaded(module, name))
		{
			// may throw
			unloadPlugin(module, name) ;
		}

		// remove the record from the registered plugins
		PluginRange_t iterRange = thePlugins.equal_range(name) ;

		PluginRecModuleEq pred(module) ;
		PluginContainer_t::iterator iter = std::find_if(iterRange.first, iterRange.second, pred) ;
		if(iter != iterRange.second)
		{
			ret = true ;

			PluginRecord* prec = iter->second ;
			thePlugins.erase(iter) ;

			delete prec ;
			prec = 0 ;
		}
		else
		{
			// shouldn't happen, we check the Plugin is registered, so the record
			// must exist
			// @note[design] we have no internal locking at this stage, so this
			//               assumes non-multi-threaded

			std::ostringstream buf ;
			buf << "Internal error, cannot access Plugin Record [module=" << module << ",name=" << name << "]" ;
			throw(PluginManagerException(buf.str())) ;
		}
	}

	return(ret) ;
}

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
void
PluginManager::unregisterAll() throw(SharedLibraryException, PluginManagerException)
{
	// may throw
	unloadAll() ;

	PluginContainer_t::iterator iter = thePlugins.begin() ;
	while(iter != thePlugins.end())
	{
		PluginContainer_t::iterator next = iter ;
		++next ;

		PluginRecord* prec = iter->second ;
		thePlugins.erase(iter) ;

		delete prec ;
		prec = 0 ;

		iter = next ;
	}
}




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
cutil::Plugin*
PluginManager::getPlugin(const std::string& module, const std::string& name) throw(SharedLibraryException, PluginManagerException)
{
	cutil::Plugin* plugin = 0 ;

	PluginRecord* prec = getPluginRecord(module, name) ;

	if(prec)
	{
		if(!isLoaded(module, name) && theAutoLoadFlag)
		{
			// may throw SharedLibraryException
			loadPlugin(module, name) ;
		}

		if(isLoaded(module, name))
		{
			plugin = prec->thePlugin ;
		}
		else
		{
			std::ostringstream buf ;
			buf << "Plugin is not loaded [module=" << module << ",plugin=" << name << "]" ;
			throw(PluginManagerException(buf.str())) ;
		}
	}
	else
	{
		std::ostringstream buf ;
		buf << "Plugin does not exists [module=" << module << ",plugin=" << name << "]" ;
		throw(PluginManagerException(buf.str())) ;
	}

	return(plugin) ;
}

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
cutil::Plugin*
PluginManager::getPlugin(const std::string& name) throw(SharedLibraryException, PluginManagerException)
{
	Plugin* plugin ;
	if(theNameTransform)
	{
		PluginNameTransform::TransformData data ;
		if(theNameTransform->lookup(name, data))
		{
			plugin = getPlugin(data.theModule, data.thePluginName) ;
		}
		else
		{
			std::ostringstream buf ;
			buf << "No lookup found within PluginNameTransform for specified plugin name [" << name << "]" ;
			throw(PluginManagerException(buf.str())) ;
		}
	}
	else
	{
		throw(PluginManagerException("No PluginNameTransform available")) ;
	}

	return(plugin) ;
}


//-------------------------------------------------------------------------------//
// Reference Counting

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
void
PluginManager::unrefPlugin(const Plugin& plugin)
{
	PluginRecord* prec = getPluginRecord(plugin) ;
	if(prec)
	{
		prec->theRefCount-- ;
		std::cout << plugin.getInfo().getName() << ": Ref Count=" << prec->theRefCount << std::endl ;

		SharedObjectContainer_t::const_iterator citer = theSharedObjects.find(prec->theSharedObjectId) ;
		if(citer != theSharedObjects.end())
		{
			SharedObjectRecord* soRec = citer->second ;
			soRec->thePluginCreationCount-- ;
		}

		// do we automatically unload the Plugin
		if((prec->theRefCount == 1) && (!prec->theRemainLoadedFlag))
		{
			// unloading the Plugin will automatically unload the SharedLibrary, if required
			unloadPlugin(prec->theSharedObjectId, plugin.getInfo().getName()) ;
		}
	}
}

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
void
PluginManager::refPlugin(const Plugin& plugin)
{
	PluginRecord* rec = getPluginRecord(plugin) ;
	if(rec)
	{
		rec->theRefCount++ ;
		std::cout << plugin.getInfo().getName() << ": Ref Count=" << rec->theRefCount << std::endl ;

		SharedObjectContainer_t::const_iterator citer = theSharedObjects.find(rec->theSharedObjectId) ;
		if(citer != theSharedObjects.end())
		{
			SharedObjectRecord* soRec = citer->second ;
			soRec->thePluginCreationCount++ ;
		}
	}
}








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
PluginManager::PluginRecord*
PluginManager::getPluginRecord(const std::string& module, const std::string& name) const
{
	PluginRecord* prec = 0 ;

	ConstPluginRange_t iterRange = thePlugins.equal_range(name) ;

	PluginRecModuleEq pred(module) ;
	PluginContainer_t::const_iterator iter = std::find_if(iterRange.first, iterRange.second, pred) ;
	if(iter != iterRange.second)
	{
		prec = iter->second ;
	}

	return(prec) ;
}

/**
 * Accesses the registered Plugins and returns the PluginRecord for the specified Plugin
 *
 * @param plugin the Pluin for which to access the PluginRecord
 * @return the PluginRecord for the specified Plugin, or 0 if the record cannot be found
 */
PluginManager::PluginRecord*
PluginManager::getPluginRecord(const Plugin& plugin) const
{
	PluginRecord* prec = 0 ;

	ConstPluginRange_t iterRange = thePlugins.equal_range(plugin.getInfo().getName()) ;

	PluginRecPluginEq pred(plugin) ;
	PluginContainer_t::const_iterator iter = std::find_if(iterRange.first, iterRange.second, pred) ;
	if(iter != iterRange.second)
	{
		prec = iter->second ;
	}

	return(prec) ;
}

/**
 * Accesses the registered SharedLibraries and returns the SharedObjectRecord for the specified SharedLibrary
 *
 * @param module the path of the SharedLibrary
 * @return the SharedObjectRecord for the specified SharedLibrary, or 0 if the record cannot be found
 */
PluginManager::SharedObjectRecord*
PluginManager::getSharedObjectRecord(const std::string& module) const
{
	SharedObjectRecord* record = 0 ;

	SharedObjectContainer_t::const_iterator citer = theSharedObjects.find(module) ;
	if(citer != theSharedObjects.end())
	{
		record = citer->second ;
	}

	return(record) ;
}

/**
 * Registers the specified SharedLibrary
 * The SharedLibray is not constructed, loaded, or opened.
 *
 * @param module the path of the SharedLibrary to register
 * @return the SharedObjectRecord for the newly registred SharedLibrary
 */
PluginManager::SharedObjectRecord*
PluginManager::registerSharedLibrary(const std::string& module)
{
	SharedObjectRecord* record = getSharedObjectRecord(module) ;

	if(!record)
	{
		record = new SharedObjectRecord ;
		record->theSharedObject = 0 ;
		record->thePluginCreationCount = 0 ;
		record->thePluginFactory = 0 ;

		theSharedObjects.insert(std::pair<std::string, SharedObjectRecord*>(module, record)) ;
	}

	return(record) ;
}

/**
 * Returns the SharedLibrary as soecified by its module path
 * If the specified SharedLibray has not previously been registerd within this
 * PluginManager, or the instance constructed, it is regustred and create.
 *
 * @param module the SharedLibrary path
 * @return the Shared Library
 * @throw SharedLibraryException if there is an error accessing the SharedLibrary
 */
SharedLibrary*
PluginManager::getSharedLibrary(const std::string& module) throw (SharedLibraryException)
{
	SharedLibrary* lib = 0 ;

	SharedObjectRecord* soRec = getSharedObjectRecord(module) ;
	if(soRec)
	{
		if(soRec->theSharedObject)
		{
			lib = soRec->theSharedObject ;
		}
		else
		{
			lib = new SharedLibrary(module) ;
			soRec->theSharedObject = lib ;
		}
	}
	else
	{
		lib = new SharedLibrary(module) ;
		soRec = registerSharedLibrary(module) ;
		soRec->theSharedObject = lib ;
	}

	return(lib) ;
}

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
void
PluginManager::unloadSharedLibrary(const std::string& module) throw (SharedLibraryException, PluginManagerException)
{
	SharedObjectRecord* soRec = getSharedObjectRecord(module) ;

	if(soRec)
	{
		if(soRec->thePluginCreationCount == 0)
		{
			if(soRec->thePluginFactory)
			{
				releasePluginFactory(*(soRec->theSharedObject), soRec->thePluginFactory) ;
			}

			if(soRec->theSharedObject)
			{
				if(soRec->theSharedObject->isOpen())
				{
					// may throw
					soRec->theSharedObject->close() ;
				}

				delete soRec->theSharedObject ;
				soRec->theSharedObject = 0 ;
			}
		}
		else
		{
			std::ostringstream buf ;
			buf << "Cannot unload SharedLibrary while Plugin rerference remain [module" << module << "]" ;
			throw(PluginManagerException(buf.str())) ;
		}
	}
}

/**
 * Constructs a new PluginFactory instance from the specified SharedLibrary.
 *
 * @param lib the SharedLibrary from which to create the PluginFactory
 * @return the newly constructed PluginFactory
 * @throw SharedLibraryException if there is an error accessing the SharedLibrary
 */
PluginFactory*
PluginManager::createPluginFactory(SharedLibrary& lib) const throw (SharedLibraryException)
{
	PluginFactory* factory = 0 ;

	// get the PluginFactor
	if(!lib.isOpen())
	{
		lib.open() ;
	}

	void* func = lib.getSymbol("getPluginFactory") ;
	if(func)
	{
		createPluginFactoryFunc factoryFunc = (createPluginFactoryFunc)func ;

		factory = (*factoryFunc)() ;
	}

	return(factory) ;
}

/**
 * Releases the specified PluginFactory created from the specified SharedLibrary.
 *
 * @param lib the SharedLibrary from the PluginFactory was created
 * @throw SharedLibraryException if there is an error accessing the SharedLibrary
 */
void
PluginManager::releasePluginFactory(SharedLibrary& lib, PluginFactory* factory) const throw (SharedLibraryException)
{
	if(factory)
	{
		void* func = lib.getSymbol("releasePluginFactory") ;
		if(func)
		{
			releasePluginFactoryFunc factoryFunc = (releasePluginFactoryFunc)func ;

			(*factoryFunc)(factory) ;
		}
	}
}

/**
 * Returns the PluginFactory for the specified SharedLibrary.
 * If no PluginFactory curreny exists, it is created and added to the
 * approproate SharedObjectRecord for future use.
 *
 * @param module the SharedLIbrary from which to obtain the PluginFactory
 * @return the PluginFactory for the specified SharedLIbrary
 * @throw SharedLibraryException if there is an error accessing the SharedLibrary
 */
PluginFactory*
PluginManager::getPluginFactory(const std::string& module) throw (SharedLibraryException)
{
	PluginFactory* factory = 0 ;

	SharedObjectRecord* soRec = getSharedObjectRecord(module) ;
	if(soRec)
	{
		if(soRec->thePluginFactory)
		{
			factory = soRec->thePluginFactory ;
		}
		else
		{
			SharedLibrary* lib = getSharedLibrary(module) ;
			if(lib)
			{
				factory = createPluginFactory(*lib) ;
				soRec->thePluginFactory = factory ;
			}
		}
	}
	else
	{
		// load the SharedLibrary
		SharedLibrary* lib = getSharedLibrary(module) ;
		if(lib)
		{
			factory = createPluginFactory(*lib) ;
			soRec = getSharedObjectRecord(module) ;
			soRec->thePluginFactory = factory ;
		}
	}

	return(factory) ;
}







// Nested Classes


PluginManager::PluginRecModuleEq::PluginRecModuleEq(const std::string& module)
		: theModule(module)
{}

PluginManager::PluginRecModuleEq::result_type
PluginManager::PluginRecModuleEq::operator() (const PluginManager::PluginRecModuleEq::argument_type a) const
{
	return(a.second->theSharedObjectId == theModule) ;
}

PluginManager::PluginRecPluginEq::PluginRecPluginEq(const Plugin& plugin)
		: thePlugin(plugin)
{}

PluginManager::PluginRecPluginEq::result_type
PluginManager::PluginRecPluginEq::operator() (const PluginManager::PluginRecPluginEq::argument_type a) const
{
	return(a.second->thePlugin == &thePlugin) ;
}

PluginManager::PluginInfoNameEq::PluginInfoNameEq(const std::string& name)
		: theName(name)
{}

PluginManager::PluginInfoNameEq::result_type
PluginManager::PluginInfoNameEq::operator() (const PluginManager::PluginInfoNameEq::argument_type& a) const
{
	return(theName == a.getName()) ;
}
