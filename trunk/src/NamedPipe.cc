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

#include <cutil/NamedPipe.h>

#include <cerrno>
#include <string>
#include <sstream>

#include <sys/poll.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>

using cutil::NamedPipe ;
using cutil::NamedPipeException ;


//-------------------------------------------------------------------------------//
// Constructor / Desctructor

/**
 * Creates a NamedPipe and leaves it in an uncreatd state.
 * The Constructed NamedPipe will initially be set blocking, with a mode of READ_ONLY_ENUM.
 * A filesystem path set should be set, then create, and open called before this NamedPipe is in a useable state
 *
 */
NamedPipe::NamedPipe()
{
	m_file_system_path = std::string("") ;
	m_fd = -1 ;
	m_created = false ;
	m_access_mode = READ_ONLY_ENUM ;
	m_blocking = false ;
}

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
NamedPipe::NamedPipe(const std::string& path, AccessModeEnum mode, bool blocking) throw(NamedPipeException)
{
	m_file_system_path = path ;
	m_fd = -1 ;
	m_created = false ;
	m_access_mode = mode ;
	m_blocking = blocking ;

	// throws NamedPipeException
	create() ;
}

/**
 * Destructor.
 *
 */
NamedPipe::~NamedPipe()
{
	try
	{
		close() ;
		unlink() ;
	}
	catch(NamedPipeException& npe)
	{
		// @note [todo] need to log this!
	}
}


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
void
NamedPipe::create() throw(NamedPipeException)
{
	if(!m_created)
	{
		// first does the fifo already exists?
		if(!m_file_system_path.empty())
		{
			struct stat statbuf ;
			if(::stat(m_file_system_path.c_str(), &statbuf) == 0)
			{
				// yes it does, check its really a fifo
				if(S_ISFIFO(statbuf.st_mode))
				{
					// yes it is
					m_created = true ;
				}
				else
				{
					m_created = false ;
					throw(NamedPipeException(std::string("File exists, ").append(m_file_system_path).append(", but it isn't a FIFO"))) ;
				}
			}
			else
			{
				// error returned from stat, hopefully this means that the spedified path simply doesn't exist
				if(errno == ENOENT)
				{
					if(::mkfifo(m_file_system_path.c_str(), 0666) == 0)
					{
						// created ok
						m_created = true ;
					}
					else
					{
						m_created = false ;
						throw(NamedPipeException(std::string("Exception in create [mkfifo]: Cannot create NamedPipe, ").append(m_file_system_path))) ;
					}
				}
				else
				{
					throw(NamedPipeException(std::string("Exception in create [stat]:").append(::strerror(errno)))) ;
				}
			}
		}
		else
		{
			m_created = false ;
			throw(NamedPipeException(std::string("Cannot create NamedPipe, pathname is empty"))) ;
		}
	}
}

/**
 * Attempts to open this NamedPipe
 * Care should be taken to setup this NamedPipe as required before calling open, for instance
 * if non-blocking, the read end of the NamedPipe must be opened prior to the write end. If
 * non-blocking, a call to open will normally block until both the read and write end are opened.
 * If this namedPipe is already open, no action is taken.
 *
 * @throws NamedPipeException if an error occurs opening this NamedPipe
 */
void
NamedPipe::open() throw(NamedPipeException)
{
	if(!isOpen())
	{
		int flags ;
		switch(m_access_mode)
		{
			case NamedPipe::READ_ONLY_ENUM:
			{
				flags = O_RDONLY ;
				break ;
			}
			case NamedPipe::WRITE_ONLY_ENUM:
			{
				flags = O_WRONLY ;
				break ;
			}
			case NamedPipe::READ_WRITE_ENUM:
			{
				flags = O_RDWR ;
			}
			default:
			{
				flags = O_RDONLY ;
			}
		}

		if(m_blocking)
		{
			flags = flags|O_NONBLOCK ;
		}

		m_fd = ::open(m_file_system_path.c_str(), flags) ;

		if(m_fd < 0)
		{
			throw(NamedPipeException(std::string("Exception in open [open]:").append(::strerror(errno)))) ;
		}
	}
}

/**
 * Closes this NamedPipe
 *
 * @throws NamedPipeException if an error occurs during the closing of this NamedPipe
 */
void
NamedPipe::close() throw(NamedPipeException)
{
	if(isOpen())
	{
		if(::close(m_fd) == 0)
		{
			m_fd = -1 ;
		}
		else
		{
			throw(NamedPipeException(std::string("Exception in close [close]:").append(::strerror(errno)))) ;
		}
	}
}

/**
 * Deletes this NamedPipe path fromthe filesystem
 * This method is intended as a clean up operation during the destruction of this NamedPipe
 *
 * @throws NamedPipeException if an error occurs during the unlink process
 */
void
NamedPipe::unlink() throw(NamedPipeException)
{
	if(::unlink(m_file_system_path.c_str()) != 0)
	{
		throw(NamedPipeException(std::string("Exception in unlink [unlink]:").append(::strerror(errno)))) ;
	}
}

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
void
NamedPipe::setPath(const std::string& path)
{
	if(isCreated() || isOpen())
	{
		throw(NamedPipeException("Cannot set file system path on a created NamedPipe")) ;
	}
	else
	{
		m_file_system_path = path ;
	}
}

/**
 * Returns the filesystem path of this NamedPipe
 * This method returns only the set filesystem path, this path may not have been created
 * and may not be a valid filesystem path for this NamedPipe. Use isCreated to detemine
 * if the path is indeed valid and created.
 *
 * @return the filesystem path of this NamedPipe
 */
std::string
NamedPipe::getPath() const
{
	return(m_file_system_path) ;
}

/**
 * Sets the blocking state of this NamedPipe.
 * This method can only be called upon an unopened NamedPipe, doing so on an open NamedPipe
 * will result in an Exception being thrown
 *
 * @param blockState the block state of this NamedPipe
 * @throws NamedPipeException if the this NamedPipe is already open
 */
void
NamedPipe::setBlockState(bool blockState)
{
	if(isOpen())
	{
		throw(NamedPipeException("Cannot set Block State on an open NamedPipe")) ;
	}
	else
	{
		m_blocking = blockState ;
	}
}

/**
 * Returns the blocking state of this NamedPipe.
 * The blocking state indicate if this namedPipe is set to open in a blocking or non-blocking manner.
 * This method may be called on an open, or non open NamedPipe to determine what state it is, or
 * will be in
 *
 * @return the blocking state of this NamedPipe
 */
bool
NamedPipe::getBlockState() const
{
	return(m_blocking) ;
}

/**
 * Sets the access mode of this NamedPipe
 * This method can only be called upon as unopened namedPipe, doing so on an open NamedPipe
 * will result in an Exception being thrown
 *
 * @param mode the access mode of this NamedPipe
 * @throws NamedPipeException if called on an open namedPipe
 */
void
NamedPipe::setMode(AccessModeEnum mode)
{
	if(isOpen())
	{
		throw(NamedPipeException("Cannot set Mode on an open NamedPipe")) ;
	}
	else
	{
		m_access_mode = mode ;
	}
}

/**
 * Returns the access mode of this NamedPipe
 *
 * @return the access mode of this NamedPipe
 */
NamedPipe::AccessModeEnum
NamedPipe::getMode() const
{
	return(m_access_mode) ;
}


/**
 * Returns whether this NamedPipe has been created.
 * A created NamedPipe has had create called and return successfully
 *
 * @return true if this NamedPipe has been created, false otherwise
 */
bool
NamedPipe::isCreated() const
{
	return(m_created) ;
}

/**
 * Returns whether this NamedPipe has been opened.
 * 
 * @return true if this NamedPipe has been opened, false otherwise
 */
bool
NamedPipe::isOpen() const
{
	return(m_fd == -1 ? false : true) ;
}


//-------------------------------------------------------------------------------//
// AbstractInputStream

bool
NamedPipe::isDataAvailable(long usec) const throw(Exception)
{
	bool ret = false ;

	if(isOpen())
	{
		::pollfd pfd[1] ;
		pfd[0].fd = m_fd ;
		pfd[0].events = POLLIN|POLLERR|POLLHUP|POLLNVAL ;

		int pollret = poll(pfd, 1, usec) ;

		if(pollret == -1)
		{
			throw(NamedPipeException(std::string("Exception in dataAvailable [poll]:").append(::strerror(errno)))) ;
		}
		else if(pollret)
		{
			if(pfd[0].revents == POLLIN)
			{
				ret = true ;
			}
			if(pfd[0].revents == POLLERR)
			{
				ret = false ;
				throw(NamedPipeException(std::string("An error occured on the NamedPipe"))) ;
			}
			if(pfd[0].revents == POLLHUP)
			{
				ret = false ;
				throw(NamedPipeException(std::string("Hung Up"))) ;
			}
			if(pfd[0].revents == POLLNVAL)
			{
				ret = false ;
				throw(NamedPipeException(std::string("Invalid request, NamedPipe not open"))) ;
			}
		}
		else
		{
			// no data to read
			ret = false ;
		}
	}

	return(ret) ;
}

ssize_t
NamedPipe::read(void* buf, size_t length) const throw(Exception)
{
	ssize_t retcode = ::read(m_fd, buf, length) ;

	if(retcode < 0)
	{
		throw(NamedPipeException(std::string("Exception in read [read]:").append(::strerror(errno)))) ;
	}

	return(retcode) ;
}

ssize_t
NamedPipe::read(void* buf, size_t length, int& err_code) const throw()
{
	ssize_t retcode = ::read(m_fd, buf, length) ;

	if(retcode < 0)
	{
		err_code = errno ;
	}

	return(retcode) ;
}

ssize_t
NamedPipe::read(char& read_byte) throw(Exception)
{
	return(read(&read_byte, 1)) ;
}

ssize_t
NamedPipe::read(char& read_byte, int& err_code) throw()
{
	return(read(&read_byte, 1, err_code)) ;
}

//-------------------------------------------------------------------------------//
// AbstractOutputStream

ssize_t
NamedPipe::write(const void* data, size_t size) throw(Exception)
{
	ssize_t retcode = ::write(m_fd, data, size) ;
	if(retcode < 0)
	{
		throw(Exception(std::string("Exception in write [write]: ").append(::strerror(errno)))) ;
	}

	return(retcode) ;
}

ssize_t
NamedPipe::write(const void* data, size_t size, int& err_code) throw()
{
	ssize_t written = ::write(m_fd, data, size) ;

	if(written < 0)
	{
		err_code = errno ;
	}

	return(written) ;
}

ssize_t
NamedPipe::write(const char& write_byte) throw(Exception)
{
	return(write(&write_byte, 1)) ;
}

ssize_t
NamedPipe::write(const char& write_byte, int& err_code) throw()
{
	return(write(&write_byte, 1, err_code)) ;
}
