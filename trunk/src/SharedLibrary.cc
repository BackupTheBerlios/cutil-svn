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

#include <cutil/SharedLibrary.h>

#include <cutil/SharedLibraryException.h>

#include <iostream>
#include <string>
#include <sstream>

#include <dlfcn.h>

using cutil::SharedLibrary ;

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
SharedLibrary::SharedLibrary(const std::string& name)
		: theModuleName(name)
{
	theOpenFlag = false ;
	theHandle = 0 ;
	theResolveMode = RESOLVE_NOW_ENUM ;
	theGlobalFlag = true ;
}

/**
 * Destructor
 * Closes this SharedLibrary if open.
 *
 */
SharedLibrary::~SharedLibrary()
{
	if(isOpen())
	{
		try
		{
			close() ;
		}
		catch(SharedLibraryException& e)
		{
			std::cerr << "Caught Exception closing SharedLibrary " << theModuleName << "\n" ;
			std::cerr << e.toString() << std::endl ;
		}
	}
}


//-------------------------------------------------------------------------------//
// Accessors / Mutators

/**
 * Returns the module name of this SharedLibrary
 *
 * @return the module name of this SharedLibrary
 */
const std::string&
SharedLibrary::getModuleName() const
{
	return(theModuleName) ;
}

/**
 * Sets the Module name of this SharedLibrary
 * The module name may only be set on an unopened SharedLibrary, attempting to
 * call setModuleName on an open SharedLibrary will result in a SharedLibraryException
 * being thrown.
 *
 * @param name the module name
 * @throw SharedLibraryException if this SharedLibrary is open.
 */
void
SharedLibrary::setModuleName(const std::string& name) throw(SharedLibraryException)
{
	if(isOpen())
	{
		throw(SharedLibraryException(std::string("setModuleName called on an open SharedLibrary"))) ;
	}
	else
	{
		theModuleName = name ;
	}
}

/**
 * Returns the open state of this SharedLibrary
 *
 * @return true if this SharedLibrary has been opened, false otherwise
 */
bool
SharedLibrary::isOpen() const
{
	return(theOpenFlag) ;
}

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
void
SharedLibrary::setResolveMode(SharedLibrary::ResolveMode mode) throw(SharedLibraryException)
{
	if(isOpen())
	{
		throw(SharedLibraryException(std::string("setResolveMode called on an open SharedLibrary"))) ;
	}
	else
	{
		theResolveMode = mode ;
	}
}

/**
 * Returns the current symbol resolution mode
 *
 * @see setResolveMode
 * @return the current symbol resolution mode
 */
SharedLibrary::ResolveMode
SharedLibrary::getResolveMode() const
{
	return(theResolveMode) ;
}

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
void
SharedLibrary::setGlobalSymbol(bool global) throw(SharedLibraryException)
{
	if(isOpen())
	{
		throw(SharedLibraryException(std::string("setGlobalSymbol called on an open SharedLibrary"))) ;
	}
	else
	{
		theGlobalFlag = global ;
	}
}

/**
 * Returns whether symbols loaded from this SharedLibrary are availble for subsequently loaded objects.
 *
 * @return true if loaded symbols are global, false if local.
 */
bool
SharedLibrary::getGlobalSymbol() const
{
	return(theGlobalFlag) ;
}


//-------------------------------------------------------------------------------//
// SharedLibray Operations

/**
 * Loads the dynamic library of this SharedLibrary
 *
 * @throw SharedLibraryException if the module does not exist, or if there is an
 *        error opening the module.
 */
void
SharedLibrary::open() throw(SharedLibraryException)
{
	if(!isOpen())
	{
		int flags = 0 ;
		switch(theResolveMode)
		{
				case RESOLVE_LAZY_ENUM:
				{
					flags = RTLD_LAZY ;
					break ;
				}
				case RESOLVE_NOW_ENUM:
				{
					flags = RTLD_NOW ;
					break ;
				}
		}

		if(theGlobalFlag)
		{
			flags |= RTLD_LAZY ;
		}

		void* handle = dlopen(theModuleName.c_str(), flags) ;

		if(handle)
		{
			theHandle = handle ;
			theOpenFlag = true ;
		}
		else
		{
			const char* errmsg = dlerror() ;

			std::ostringstream buf ;
			buf << "Exception opening SharedLibrary " << theModuleName ;
			buf << (errmsg ? errmsg : "No error message given") ;
			throw(SharedLibraryException(buf.str())) ;
		}
	}
}

/**
 * Closes this SharedLibrary
 *
 * @throw SharedLibraryException if there is an error closing the module.
 */
void
SharedLibrary::close() throw(SharedLibraryException)
{
	if(isOpen())
	{
		if(dlclose(theHandle) == 0)
		{
			theHandle = 0 ;
			theOpenFlag = false ;
		}
		else
		{
			const char* errmsg = dlerror() ;

			std::ostringstream buf ;
			buf << "Exception closing SharedLibrary " << theModuleName ;
			buf << (errmsg ? errmsg : "No error message given") ;
			throw(SharedLibraryException(buf.str())) ;
		}
	}
}

/**
 * Returns the address of the specified symbol within this SharedLibrary
 * The SharedLibrary must have been loaded by a call to open before calling
 * this method.
 *
 * @param symbol the symbl name to get
 * @throw SharedLibraryException if this SharedLibrary is not open, the symbol
 *        does not exist, or there is an error locating the symbol
 */
void*
SharedLibrary::getSymbol(const std::string& symbol) throw(SharedLibraryException)
{
	void* sym = 0 ;

	if(isOpen())
	{
		// clear any previous error message
		const char* errmsg =  dlerror() ;

		sym = dlsym(theHandle, symbol.c_str()) ;

		// test is there was an error
		errmsg = dlerror() ;
		if(errmsg)
		{
			// got an error
			sym = 0 ;
			std::ostringstream buf ;
			buf << "Exception during symbol lookup [" << theModuleName  << ", " << symbol << "] " << errmsg ;
			throw(SharedLibraryException(buf.str())) ;
		}
	}
	else
	{
		throw(SharedLibraryException("Attempt to getSymbol on an unopen SharedLibrary")) ;
	}

	return(sym) ;
}
