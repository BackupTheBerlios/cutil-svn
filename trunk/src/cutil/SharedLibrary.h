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

#ifndef _CUTIL_SHAREDLIBRARY_
#define _CUTIL_SHAREDLIBRARY_

#include <cutil/SharedLibraryException.h>

#include <string>

namespace cutil
{
	/**
	 * Representation of a dynamically loadable module.
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class SharedLibrary
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * Constructs a new SharedLibrary for the specified modules name.
			 * The SharedLibrary is not opened until the user call open.
			 * The default Resolve mode is set to RESOLVE_NOW_ENUM, and global symbols is
			 * set true.
			 *
			 * @param name the name of dynamically loadable module
			 */
			SharedLibrary(const std::string& name) ;

			/**
			 * Destructor
			 * Closes this SharedLibrary if open.
			 *
			 */
			~SharedLibrary() ;


			//-------------------------------------------------------------------------------//
			// Accessors / Mutators

			/**
			 * Returns the module name of this SharedLibrary
			 *
			 * @return the module name of this SharedLibrary
			 */
			const std::string& getModuleName() const ;

			/**
			 * Sets the Module name of this SharedLibrary
			 * The module name may only be set on an unopened SharedLibrary, attempting to
			 * call setModuleName on an open SharedLibrary will result in a SharedLibraryException
			 * being thrown.
			 *
			 * @param name the module name
			 * @throw SharedLibraryException if this SharedLibrary is open.
			 */
			void setModuleName(const std::string& name) throw(SharedLibraryException) ;

			/**
			 * Returns the open state of this SharedLibrary
			 *
			 * @return true if this SharedLibrary has been opened, false otherwise
			 */
			bool isOpen() const ;

			/** Symbol resolution modes */
			enum ResolveMode { RESOLVE_LAZY_ENUM, RESOLVE_NOW_ENUM } ;

			/**
			 * Sets the symbol resolution mode of this Shared Library.
			 * With the selution mode set to RESOLVE_LAZY_ENUM (RTLD_LAZY
			 * on UNIX systems), undefined symbols are resolved when required.
			 * If the resolution mode is RESOLVE_NOW_ENUM (RTLD_NOW on UNIX
			 * systems) all undefined symbols are resolved immediatly, i.e. before
			 * the call to open returns. With the resolution mode set to RESOLVE_NOW_ENUM,
			 * a SharedLibraryException is thrown is any symbol cannot be resolved.
			 * The resolution mode is effective only during the opening of a shared library.
			 * To avoid confusion, changes to the resolution mode after open has been called 
			 * are not allowed, attempting to do so results in a SharedLibraryException
			 *
			 * @param mode the symbol resolution mode
			 * @throw SharedLibraryException if an attemp is made to set the resolution mode
			 *        on an open SharedLibrary
			 */
			void setResolveMode(ResolveMode mode) throw(SharedLibraryException) ;

			/**
			 * Returns the current symbol resolution mode
			 *
			 * @see setResolveMode
			 * @return the current symbol resolution mode
			 */
			ResolveMode getResolveMode() const ;

			/**
			 * Sets whether symbols loaded from this SharedLibrary are made globally.
			 * Symbols made globally available are availble for subsequently loaded
			 * dynamic libraries. Set true to make loaded symbols global, or false
			 * in order to make loaded symbols local only.
			 * This flag is effective only during the opening of a shared library.
			 * To avoid confusion, changes to the global flag after open has been called 
			 * are not allowed, attempting to do so results in a SharedLibraryException 
			 *
			 * @param global set true to set loaded symbols global, false for local.
			 * @throw SharedLibraryException if an attemp is made to change the global flag
			 *        on an open SharedLibrary
			 */
			void setGlobalSymbol(bool global) throw(SharedLibraryException) ;

			/**
			 * Returns whether symbols loaded from this SharedLibrary are availble for subsequently loaded objects.
			 *
			 * @return true if loaded symbols are global, false if local.
			 */
			bool getGlobalSymbol() const ;


			//-------------------------------------------------------------------------------//
			// SharedLibray Operations

			/**
			 * Loads the dynamic library of this SharedLibrary
			 *
			 * @throw SharedLibraryException if the module does not exist, or if there is an
			 *        error opening the module.
			 */
			void open() throw(SharedLibraryException) ;

			/**
			 * Closes this SharedLibrary
			 *
			 * @throw SharedLibraryException if there is an error closing the module.
			 */
			void close() throw(SharedLibraryException) ;

			/**
			 * Returns the address of the specified symbol within this SharedLibrary
			 * The SharedLibrary must have been loaded by a call to open before calling
			 * this method.
			 *
			 * @param symbol the symbl name to get
			 * @throw SharedLibraryException if this SharedLibrary is not open, the symbol
			 *        does not exist, or there is an error locating the symbol
			 */
			void* getSymbol(const std::string& symbol) throw(SharedLibraryException) ;


		protected:

			//-------------------------------------------------------------------------------//

		private:

			/** the dynamic module name */
			std::string theModuleName ;

			/** the handle to the loaded dynamic module */
			void* theHandle ;

			/** indicates if this SharedLibray is open */
			bool theOpenFlag ;

			/** the symbol resolution mode */
			ResolveMode theResolveMode ;

			/** indicates if loaded symbols are available globally (true), or local (false) */
			bool theGlobalFlag ;


	} ; /* class SharedLibrary */

} /* namespace cutil */

#endif /* _CUTIL_SHAREDLIBRARY_ */
