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


#ifndef _CUTIL_NAMEDPIPE_
#define _CUTIL_NAMEDPIPE_

#include <cutil/NamedPipeException.h>

#include <string>

namespace cutil
{
	/**
	 * A Named Pipe, or FIFO for interprocess communication.
	 * This class proves a mechanism for interprocess communication in both a blocking, and for real-time
	 * applications, non blocking.
	 * This class is a simple wrapper around common low level named pipe operations.
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class NamedPipe
	{
		public:

			/** access modes for the NamedPipe */
			enum AccessModeEnum { READ_ONLY_ENUM, WRITE_ONLY_ENUM, READ_WRITE_ENUM } ;

			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * Creates a NamedPipe and leaves it in an uncreatd state.
			 * The Constructed NamedPipe will initially be set blocking, with a mode of READ_ONLY_ENUM.
			 * A filesystem path set should be set, then create, and open called before this NamedPipe is in a useable state
			 *
			 */
			NamedPipe() ;

			/**
			 * Creates a NamedPipe to use the specified path.
			 * The NamedPipe is created but not opened until open is called upon this NamedPipe
			 *
			 * @param path the filesystem path to create the FIFO on
			 * @param mode the access mode of this NamedPipe
			 * @param blocking true if this NamedPipe is blocking, false for non-blocking
			 * @throw namedPipeException if the specified path exists, but is not the correct type, or if an error
			 *         is encountered creating a new FIFO on the file system.
			 */
			NamedPipe(const std::string& path, AccessModeEnum mode, bool blocking) throw(NamedPipeException) ;

			/**
			 * Destructor.
			 *
			 */
			virtual ~NamedPipe() ;


			//-------------------------------------------------------------------------------//
			//


			//-------------------------------------------------------------------------------//
			// Named Pipe Setup Operations

			/**
			 * Attempts to create the NamedPipe using the set path.
			 * If path already exists, and is the correct type, no further action will be taken.
			 * if the path exists, but is not the correct type, an Exception will be thrown.
			 * If the path does not exist, it will be created as a FIFO on the filesystem.
			 * If this NamedPipe has already had create called and return seccesfully, this method takes
			 * no furthe actions.
			 *
			 * @throws NamedPipeException if set path exists, but is not the correct type, or if an error
			 *         is encountered creating a new FIFO on the file system.
			 */
			void create() throw(NamedPipeException) ;

			/**
			 * Attempts to open this NamedPipe
			 * Care should be taken to setup this NamedPipe as required before calling open, for instance
			 * if non-blocking, the read end of the NamedPipe must be opened prior to the write end. If
			 * non-blocking, a call to open will normally block until both the read and write end are opened.
			 * If this namedPipe is already open, no action is taken.
			 *
			 * @throws NamedPipeException if an error occurs opening this NamedPipe
			 */
			void open() throw(NamedPipeException) ;

			/**
			 * Closes this NamedPipe
			 *
			 * @throws NamedPipeException if an error occurs during the closing of this NamedPipe
			 */
			void close() throw(NamedPipeException) ;

			/**
			 * Deletes this NamedPipe path fromthe filesystem
			 * This method is intended as a clean up operation during the destruction of this NamedPipe
			 *
			 * @throws NamedPipeException if an error occurs during the unlink process
			 */
			void unlink() throw(NamedPipeException) ;

			//-------------------------------------------------------------------------------//
			// Named Pipe Read/Write Operations

			/**
			 * Writes the specified char to the NamedPipe.
			 * This method does not throw any exception and is intended as a real-time call in a non-blocking state.
			 * The return value indicates the status of the underlying write call, on failure
			 * errcode will be set to the error code of the underlying call. see write(2) for
			 * definitions of the return value and error codes
			 *
			 * @param writeByte the byte to write
			 * @param errCode set to errno of the underlying call on error
			 * @return the return status of the underlying call
			 */
			int write(const char& writeByte, int& errCode) throw() ;

			/**
			 * Writes the specified char to the NamedPipe
			 *
			 * @param writeByte the byte to write
			 * @throw NamedPipeException on error
			 */
			void write(const char& writeByte) throw(NamedPipeException) ;


			/**
			 * Attempts to read a byte from this NamedPipe.
			 * The read byte will be set within the readByte parameter.
			 * This method does not throw any Exception and is intended as a real-time call in
			 * a non-blocking state (although can be used anywhere)
			 * If this NamedPipe is in a blocking state, this method will block until a byte
			 * is available for reading.
			 * The return value indicates the return satus of the underlying call, on failure
			 * errcode will be set to the error code of the underlying call, see read(2) for
			 * definitions of the return value and error codes.
			 *
			 * @param readByte the parameter to set with the read byte
			 * @param errCode set to the errno of the underlying call
			 * @return the return status of the underlying call
			 */
			int read(char& readByte, int& errCode) throw() ;

			/**
			 * Attempts to read a byte from this NamedPipe.
			 * The read byte will be set within the readByte parameter.
			 * This method will block until data a byte is available for reading on this
			 * NamedPipe, unless in a non-blocking state. If in non-blocking mode, a read
			 * when no data is available will not throw an exception, however false will be returned.
			 *
			 * @param readByte the parameter to set with the read byte
			 * @return true if a byte was read, false otherwise
			 * @throw NamedPipeException on read error
			 */
			bool read(char& readByte) throw(NamedPipeException) ;
			
			/**
			 * Determines if data is available for reading from this NamedPipe
			 *
			 * @param timeout timeout for waiting for data in milliseconds
			 * @return true if data is available for reading, false otherwise
			 * @throw NamedPipeException if and error occurs or the FIFO has been hung up.
			 */
			bool dataAvailable(int timeout) const throw(NamedPipeException) ;

			//-------------------------------------------------------------------------------//
			// General Accessors/Murators

			/**
			 * Sets the path of this NamedPipe
			 * This method can only be called on a NamedPipe that has not already had create,
			 * or open called upon it, doing wo will cause an Exception.
			 *
			 * @param path filoesystem path of this NamedPipe
			 * @throws NamedPipeException if called upon a created, or opened NamedPipe
			 */
			void setPath(const std::string& path) ;

			/**
			 * Returns the filesystem path of this NamedPipe
			 * This method returns only the set filesystem path, this path may not have been created
			 * and may not be a valid filesystem path for this NamedPipe. Use isCreated to detemine
			 * if the path is indeed valid and created.
			 *
			 * @return the filesystem path of this NamedPipe
			 */
			std::string getPath() const ;

			/**
			 * Sets the blocking state of this NamedPipe.
			 * This method can only be called upon an unopened NamedPipe, doing so on an open NamedPipe
			 * will result in an Exception being thrown
			 *
			 * @param blockState the block state of this NamedPipe
			 * @throws NamedPipeException if the this NamedPipe is already open
			 */
			void setBlockState(bool blockState) ;

			/**
			 * Returns the blocking state of this NamedPipe.
			 * The blocking state indicate if this namedPipe is set to open in a blocking or non-blocking manner.
			 * This method may be called on an open, or non open NamedPipe to determine what state it is, or
			 * will be in
			 *
			 * @return the blocking state of this NamedPipe
			 */
			bool getBlockState() const ;

			/**
			 * Sets the access mode of this NamedPipe
			 * This method can only be called upon as unopened namedPipe, doing so on an open NamedPipe
			 * will result in an Exception being thrown
			 *
			 * @param mode the access mode of this NamedPipe
			 * @throws NamedPipeException if called on an open namedPipe
			 */
			void setMode(AccessModeEnum mode) ;

			/**
			 * Returns the access mode of this NamedPipe
			 *
			 * @return the access mode of this NamedPipe
			 */
			AccessModeEnum getMode() const ;


			/**
			 * Returns whether this NamedPipe has been created.
			 * A created NamedPipe has had create called and return successfully
			 *
			 * @return true if this NamedPipe has been created, false otherwise
			 */
			bool isCreated() const ;

			/**
			 * Returns whether this NamedPipe has been opened.
			 * 
			 * @return true if this NamedPipe has been opened, false otherwise
			 */
			bool isOpen() const ;

			//-------------------------------------------------------------------------------//

		protected:


			//-------------------------------------------------------------------------------//

		private:

			/** the filesysten path of this NamedPipe */
			std::string theFileSystemPath ;

			/** the File Descriptor of the opened named pipe */
			int theFd ;

			/** indicates if this NamedPipe has been created */
			bool theCreatedFlag ;

			/** indicates the blocking state of this NamedPipe */
			bool theBlockStateFlag ;
			
			/** Inidicates the access mode of this NamedPipe */
			AccessModeEnum theAccessMode ;

	} ; /* class NamedPipe */

} /* namespace cutil */


#endif /* _CUTIL_NAMEDPIPE_ */

