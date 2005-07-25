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

#include <cutil/FilePath.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <cerrno>


using cutil::FilePath ;

const char FilePath::PATH_SEPARATOR = '/' ;

//-------------------------------------------------------------------------------//
// Constructor / Desctructor

/**
 * Creates a new FilePath from the specified path
 *
 * @param path the file path
 */
FilePath::FilePath(const std::string& path)
{
	theFilePath = path ;
	cleanupFilePath(theFilePath) ;
}

/**
 * Creates a new FilePath from the specified parent path and the specified child leaf path
 *
 * @param parent the parent directory path
 * @param leaf the file path
 */
FilePath::FilePath(const std::string& parent, const std::string& path)
{
	theFilePath = std::string(parent) ;
	cleanupFilePath(theFilePath) ;

	appendPath(path) ;
}

/**
 * Creates a new FilePath from the specified parent FilePath and the specified child leaf path
 *
 * @param parent the parent FilePath representing the parent directory
 * @param leaf the file path
 */
FilePath::FilePath(const FilePath& parent, const std::string& path)
{
	theFilePath = std::string(parent.getPath()) ;
	cleanupFilePath(theFilePath) ;

	appendPath(path) ;
}

/**
 * Copy constructor
 *
 * @param file the FilePath object to copy
 */
FilePath::FilePath(const FilePath& file)
{
	theFilePath = file.theFilePath ;
	cleanupFilePath(theFilePath) ;
}



/**
 * Destructor.
 *
 */
FilePath::~FilePath()
{
	// nothing to do
}



//-------------------------------------------------------------------------------//
// File Operations

/**
 * Creates and empty file within the filesystem as specified by the path of this FilePath.
 * An Exception will be thrown if the FilePath is empty, the file already exists, or any
 * of the required parent directory components do not exist.
 *
 * @throw Exception if a system error occured during file creation
 */
void
FilePath::createFile() throw(Exception)
{
	if(isEmpty())
	{
		throw(Exception(std::string("Exception creating file: cannot create empty file path"))) ;
	}

	if(exists())
	{
		throw(Exception(std::string("Exception creating file: file already exists"))) ;
	}

	FilePath parent = getParentFile() ;
	if(!parent.exists())
	{
		throw(Exception(std::string("Exception creating file: parent directory must exist"))) ;
	}

	if(::creat(getPath().c_str(), S_IRUSR | S_IWUSR | S_IRGRP) == -1)
	{
		throw(Exception(std::string("Exception creating file [creat]:").append(::strerror(errno)))) ;
	}
}

/**
 * Creates the directory as specified by the path of this FilePath if it does not already exist.
 * If parents is set true, the parent path components will be created if required, otherwise
 * only the leaf directory is created and the parent components must already exist.
 * An Exception will be thrown if the FilePath is empty, the directory already exists, or any
 * of the required parent directory components do not exist.
 *
 * @param parent set true to create required parent parent directories.
 * @throw Exception if a system error occured during file creation
 */
void
FilePath::createDir(bool parent) throw(Exception)
{
	if(isEmpty())
	{
		throw(Exception(std::string("Exception creating directory: cannot create empty file path"))) ;
	}

	if(exists())
	{
		throw(Exception(std::string("Exception creating directory: directory already exists"))) ;
	}

	if(parent)
	{
		FilePath f = getParentFile() ;
		if(!f.exists())
		{
			f.createDir(parent) ;
		}
	}

	if(::mkdir(getPath().c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP) != 0)
	{
		throw(Exception(std::string("Exception creating directory [mkdir]:").append(::strerror(errno)))) ;
	}
}

/**
 * Deletes the File represented by this FilePath object
 * If the specified file does not exist, no further action is taken, and false is returned.
 * If this FilePath represents a directory, the directory must not contain any files
 * before it can be deleted, otherwise an exception is thrown.
 *
 * @return true if the file or directory was sucessfuly deleted.
 * @throw Exception if a system error occured during file creation
 */
bool
FilePath::deleteFile() throw(Exception)
{
	bool removed = false ;

	if(isEmpty())
	{
		throw(Exception(std::string("Exception removing file path: cannot remove empty file path"))) ;
	}

	if(isDirectory())
	{
		// @todo we only need the count of file items here, not the names
		std::list<std::string> dummy_list ;
		getFiles(dummy_list) ;

		if(dummy_list.empty())
		{
			if(::rmdir(getPath().c_str()) != 0)
			{
				throw(Exception(std::string("Exception removing directory [rmdir]:").append(::strerror(errno)))) ;
			}
			else
			{
				removed = true ;
			}
		}
		else
		{
			throw(Exception(std::string("Exception removing directory: directory not empty"))) ;
		}

	}
	else
	{
		if(::unlink(getPath().c_str()) != 0)
		{
			throw(Exception(std::string("Exception removing file [unlink]:").append(::strerror(errno)))) ;
		}
		else
		{
			removed = true ;
		}
	}

	return(removed) ;
}

/**
 * Renames the file or directory represented by this FilePath object to the specified name
 * If the file does not exist, it cannot be renamed and an Exception is thrown.
 * An Exception is thrown if the filepath of this FilePath is empty, or newfile is empty,
 * or newfile already exists.
 *
 * @param newFile the new filepath to rename to
 * @throw Exception if a system error occured during renaming
 */
void
FilePath::rename(const std::string& newFile) throw(Exception)
{
	if(isEmpty())
	{
		throw(Exception(std::string("Exception renaming file: empty file path"))) ;
	}

	FilePath f(newFile);
	if(f.isEmpty())
	{
		throw(Exception(std::string("Exception renaming file: destination file path is empty"))) ;
	}

	if(f.exists())
	{
		throw(Exception(std::string("Exception renaming file: destination file already exists"))) ;
	}

	if(::rename(getPath().c_str(), f.getPath().c_str()) != 0)
	{
		throw(Exception(std::string("Exception removing file [unlink]:").append(::strerror(errno)))) ;
	}
}

/**
 * Returns true if the file or directory represented by this FilePath object exists.
 *
 * @return true if the file or directory represented by this FilePath object exists,
 *         otherwise false
 * @throw Exception if a system error occured
 */
bool
FilePath::exists() const throw(Exception)
{
	bool exists = false ;

	if(::access(getPath().c_str(), F_OK) == 0)
	{
		exists = true ;
	}
	else
	{
		if(errno != ENOENT)
		{
			throw(Exception(std::string("Exception accessing filepath [access]:").append(::strerror(errno)))) ;
		}
		// else
		// ... catch the error and return as non existance
	}

	return(exists) ;
}



//-------------------------------------------------------------------------------//
// Filepath Component Accessors / Mutators

/**
 * Returns a string representation of the absolute path of this FilePath.
 * The absolute path is the complete path of this FilePath resolving any relative
 * path components
 *
 * @return string representation of the absolute path of this FilePath
 */
//std::string
//FilePath::getAbsolutePath() const
//{
//}

/**
 * Returns a new FilePath representing the absolute path of this FilePath.
 * The absolute path is the complete path of this FilePath resolving any relative
 * path components
 *
 * @return a new FilePath representation of the absolute path of this FilePath
 */
//File
//FilePath::getAbsoluteFile() const
//{
//	File f(getAbsolutePath()) ;
//	return(f) ;
//}

/**
 * Returns the leaf, file name or directory name represented by this FilePath object.
 * The leaf or file name is the last component of the FilePath specification
 *
 * @return the leaf, file name or directory name represented by this FilePath
 */
std::string
FilePath::getLeaf() const
{
	std::string leaf ;

	std::string filePath = getPath() ;
	std::string::size_type pos = filePath.rfind(FilePath::PATH_SEPARATOR, filePath.size() -1) ;

	if(pos != std::string::npos)
	{
		leaf = filePath.substr(pos +1, filePath.size() - (pos +1)) ;
	}

	return(leaf) ;
}

/**
 * Returns the parent path preceeding leading to the leaf, file or directory name.
 * The parent path is the filepath spcification represented by this FilePath, without the
 * leaf, file of directory name.
 *
 * @return the parent path of this FilePath
 */
std::string
FilePath::getParent() const
{
	std::string parent ;

	std::string filePath = getPath() ;
	std::string::size_type pos = filePath.rfind(FilePath::PATH_SEPARATOR, filePath.size() -1) ;

	if(pos != std::string::npos)
	{
		parent = filePath.substr(0, pos) ;
	}

	return(parent) ;
}

/**
 * Returns the parent path preceeding leading to the leaf, file or directory name as a FilePath.
 * The parent path is the filepath spcification represented by this FilePath, without the
 * leaf, file of directory name.
 *
 * @return the parent path of this FilePath
 */
FilePath
FilePath::getParentFile() const
{
	FilePath f(getParent()) ;

	// copy on return
	return(f) ;
}

/**
 * Returns a string representation of the filepath of this FilePath object
 *
 * @return the filepath of this FilePath represented as a string
 */
std::string
FilePath::getPath() const
{
	return(theFilePath) ;
}

/**
 * Sets the filepath of the file represented by this FilePath object
 *
 * @param filePath the new filepath
 */
void
FilePath::setPath(const std::string& filepath)
{
	theFilePath = filepath ;
	cleanupFilePath(theFilePath) ;
}

/**
 * Appends the specified filepath to this FilePath object
 *
 * @param filePath the filepath to append
 */
void
FilePath::appendPath(const std::string& filepath)
{
	appendPathSeparator(theFilePath) ;

	std::string apath(filepath) ;
	trimWhiteSpace(apath, true, true) ;

	theFilePath.append(apath) ;
	cleanupFilePath(theFilePath) ;
}


/**
 * Returns whether this FilePath has an empty filepath
 * Empty file paths cannot point to a valid file in the filesyetem, and are
 * generally invalid for many operations.
 *
 * @return true if the filepath of this FilePath is emoty, false otherwise
 */
bool
FilePath::isEmpty() const
{
	return(theFilePath.empty()) ;
}



//------------------------
// File property accessors

/**
 * Retuns the file type of the file represented by this FilePath object
 * The file type is one of the defined Filetype values.
 * If !exists(), this method will throw an exception.
 *
 * @return the FileType of the file represented by this FilePath object
 * @throw Exception if the file represented by this FileObject does not exist
 * @throw Exception if a system error occured
 */
FilePath::FileType
FilePath::getFileType() const throw(Exception)
{
	FilePath::FileType fileType = UNKNOWN_TYPE_ENUM;

	struct stat buf ;
	if(::stat(getPath().c_str(), &buf) == 0)
	{
		if(S_ISREG(buf.st_mode))
		{
			fileType = REGULAR_FILE_ENUM ;
		}
		else if(S_ISDIR(buf.st_mode))
		{
			fileType = DIRECTORY_ENUM ;
		}
		else if(S_ISFIFO(buf.st_mode))
		{
			fileType = FIFO_ENUM ;
		}
		else if(S_ISSOCK(buf.st_mode))
		{
			fileType = SOCKET_ENUM ;
		}
		else if(S_ISLNK(buf.st_mode))
		{
			fileType = SYM_LINK_ENUM ;
		}
		else if(S_ISCHR(buf.st_mode))
		{
			fileType = CHAR_DEV_ENUM ;
		}
		else if(S_ISBLK(buf.st_mode))
		{
			fileType = BLOCK_DEV_ENUM ;
		}
		else
		{
			fileType = UNKNOWN_TYPE_ENUM ;
		}
	}
	else
	{
		throw(Exception(std::string("Exception accesing file type [stat]:").append(::strerror(errno)))) ;
	}

	return(fileType) ;
}

/**
 * Returns true if the file represented by this FilePath exists and represents a directory
 * This is a convenience method equivelant to exists() && getFileType() == DIRECTORY_ENUM.
 *
 * @return true if this File exists and represents a directory, false otherwise
 * @throw Exception if a system error occured
 */
bool
FilePath::isDirectory() const throw(Exception)
{
	bool regFile = false;

	struct stat buf ;
	if(::stat(getPath().c_str(), &buf) == 0)
	{
		regFile = S_ISDIR(buf.st_mode) ;
	}
	else
	{
		throw(Exception(std::string("Exception accesing file type [stat]:").append(::strerror(errno)))) ;
	}

	return(regFile) ;
}

/**
 * Returns true if the file represented by this FilePath exists and is a regular file
 * This is a convenience method equivelant to exists() && getFileType() == REGULAR_FILE_ENUM.
 *
 * @return true if the file representaed by this FilePath object exists and is a regular file,
 *         false otherwise
 * @throw Exception if a system error occured
 */
bool
FilePath::isRegularFile() const throw(Exception)
{
	bool regFile = false;

	struct stat buf ;
	if(::stat(getPath().c_str(), &buf) == 0)
	{
		regFile = S_ISREG(buf.st_mode) ;
	}
	else
	{
		throw(Exception(std::string("Exception accesing file type [stat]:").append(::strerror(errno)))) ;
	}

	return(regFile) ;
}

/**
 * Returns whether the file representaed by tis FilePath exists and is a hidden file.
 * Under linux, a hidden, or dot file is a file which begins with a dot.
 *
 * @return true if this File represented by this FilePath exists and is a hidden file, false otherwise
 * @throw Exception if a system error occured
 */
bool
FilePath::isHidden() const throw(Exception)
{
	bool hidden = false ;

	std::string s = getLeaf() ;

	// reomve white space from the front
	trimWhiteSpace(s, true, false) ;

	if((!s.empty()) && (s[0] == '.'))
	{
		hidden = true ;
	}

	return(hidden) ;
}

/**
 * Returns true if the file representaed by this FilePath is readable by the current user/application
 *
 * @return true if the file represented by this FilePath is readable by the current user/application,
 *         false otherwise
 * @throw Exception if a system error occured
 */
bool
FilePath::isReadable() const throw(Exception)
{
	bool readable = false ;

	if(::access(getPath().c_str(), R_OK) == 0)
	{
		readable = true ;
	}
	else
	{
		if(errno != EACCES && errno != EROFS)
		{
			throw(Exception(std::string("Exception accessing permission [access]:").append(::strerror(errno)))) ;
		}
		// else
		// ... catch the error and return as non-writable
	}
	
	return(readable) ;
}

/**
 * Returns true if the file representaed by this FilePath is writable by the current user/application
 *
 * @return true if the file represented by this FilePath is writable by the current user/application,
 *         false otherwise
 * @throw Exception if a system error occured
 */
bool
FilePath::isWritable() const throw(Exception)
{
	bool writable = false;

	if(::access(getPath().c_str(), W_OK) == 0)
	{
		writable = true ;
	}
	else
	{
		if(errno != EACCES && errno != EROFS)
		{
			throw(Exception(std::string("Exception accessing permission [access]:").append(::strerror(errno)))) ;
		}
		// else
		// ... catch the error and return as non-writable
	}

	return(writable) ;
}

/**
 * Returns whether this FilePath contains an absolute Filepath.
 * An absolute filepath is a complete filesystem path containing a root element and
 * no relative path components
 *
 * @return true if this FilePath contains an absolute filepath, false otherwise
 */
bool
FilePath::isAbsolute() const
{
	bool absolute = false ;

	std::string s = getPath() ;

	if(!isEmpty() && s[0] == FilePath::PATH_SEPARATOR)
	{
		absolute = true ;
	}

	return(absolute) ;
}

/**
 * Returns the size of the file represented by this FilePath object.
 * The file reprensented by this FilePath object must exist and must be a regular file.
 *
 * @return the size in byted of the file
 * @throw Exception if a system error occured
 */
unsigned long
FilePath::getFileSize() const throw(Exception)
{
	unsigned long size = 0 ;

	struct stat buf ;
	if(::stat(theFilePath.c_str(), &buf) == 0)
	{
		size = buf.st_size ;
	}
	else
	{
		throw(Exception(std::string("Exception obtaining file size [stat]:").append(::strerror(errno)))) ;
	}
	
	return(size) ;
}




//-------------------------
// Directory content access

/**
 * Popultes the specified list with the contents of the directory represented by this FilePath
 * If this FilePath does not represent a directory, no action is taken and filelist is unchanged
 *
 * @param fileList populated with the contenst of the directory represented by this FilePath
 * @return the fileList parameter populated
 * @throw Exception if a system error occured
 */
std::list<std::string>&
FilePath::getFiles(std::list<std::string>& fileList) const throw(Exception)
{
	DIR* dir = 0 ;

	if((dir = ::opendir(theFilePath.c_str())) != 0)
	{
		struct dirent* item;

		// @todo how do we tell if we get an error here, or simply reach the end?
		while((item = ::readdir(dir)) != 0)
		{
			if((std::string(item->d_name) != ".") && (std::string(item->d_name) != ".."))
			{
				std::string filepath = getPath() ;
				appendPathSeparator(filepath) ;
				filepath.append(std::string(item->d_name)) ;
				fileList.push_back(filepath) ;
			}
		}
	}
	else
	{
		throw(Exception(std::string("Exception accessing directory [opendir]:").append(::strerror(errno)))) ;
	}

	return(fileList) ;
}

/**
 * Popultes the specified list with the contents of the directory represented by this FilePath
 * If this FilePath does not represent a directory, no action is taken and filelist is unchanged
 *
 * @param fileList populated with the contenst of the directory represented by this File
 * @return the fileList parameter populated
 * @throw Exception if a system error occured
 */
std::list<FilePath>&
FilePath::getFiles(std::list<FilePath>& fileList) const throw(Exception)
{
	std::list<std::string> stringList;

	getFiles(stringList);
	for(std::list<std::string>::const_iterator citer = stringList.begin(); citer != stringList.end(); ++citer)
	{
		fileList.push_back(FilePath(*citer)) ;
	}

	return(fileList) ;
}


//-------------------------------------------------------------------------------//

/**
 * Returns a new FilePath Object representing the current working directory
 *
 * @return a new FilePath Object representing the current working directory
 * @throw Exception if a system error occured
 */
FilePath
FilePath::currentWorkingDir() throw (Exception)
{
	char buf[PATH_MAX +1] ;
	memset(&buf, '\0', PATH_MAX) ;

	if(::getcwd(buf, PATH_MAX) == 0)
	{
		throw(Exception(std::string("Exception accessing current working directory [getcwd]:").append(::strerror(errno)))) ;
	}

	// copied on return
	return(FilePath(std::string(buf))) ;
}





//-------------------------------------------------------------------------------//

/**
 * Trims white space from the ends of the spceified string
 * 
 * @param s the string to trim whitespace from
 * @param front set true to trim whitespace from the front of s
 * @param end set true to trim whitespace from the end of s
 * @return copy of string s with whitespce trimmed
 */
std::string&
FilePath::trimWhiteSpace(std::string& s, bool front, bool end) const
{
	if(front)
	{
		std::string::size_type pos = 0 ;

		// reomve white space from the front
		while((pos < s.length()) && (isspace(s[pos])))
		{
			pos++ ;
		}

		s.erase(0, pos) ;
	}

	if(end)
	{
		while((!s.empty()) && (isspace(s[s.length() -1])))
		{
			s.erase(s.length() -1, 1) ;
		}
	}

	return(s) ;
}

/**
 * Trim the path separator from the specified file path string
 * 
 * @param filepath the filepath string to trim path separator from
 * @param front set true to trim the path separator from the front of filepath
 * @param end set true to trim the path separator from the end of filepath
 * @return copy of filepath with the path separators trimmed
 */
std::string&
FilePath::trimPathSeparator(std::string& s, bool front, bool end) const
{
	if(front)
	{
		std::string::size_type pos = 0 ;

		// reomve white space from the front
		while((pos < s.length()) && (s[pos] == FilePath::PATH_SEPARATOR))
		{
			pos++ ;
		}

		s.erase(0, pos) ;
	}

	if(end)
	{
		while((!s.empty()) && (s[s.length() -1] == FilePath::PATH_SEPARATOR))
		{
			s.erase(s.length() -1, 1) ;
		}
	}

	return(s) ;
}

/**
 * Appends a path separator to the end pof the specified filepath.
 * A path separator is not added if the filepath already ends with a path separator
 *
 * @param filepath the filepath to append the path separator to
 * @return filepath with path separator appended
 */
std::string&
FilePath::appendPathSeparator(std::string& filepath) const
{
	if(theFilePath[filepath.length() -1] != FilePath::PATH_SEPARATOR)
	{
		filepath.append(std::string(1, FilePath::PATH_SEPARATOR)) ;
	}

	return(filepath) ;
}

/**
 * Formats the specified filepath string.
 * Formatting includes;
 *   removal of whitespace from the beginning and end of the filepath
 *   removal of any trailing path separators
 *   removal of multiple path separators where only one is required
 *
 * @param filepath the filepath string to format
 * @return filepath formatted
 */
std::string&
FilePath::cleanupFilePath(std::string& filepath)
{
	trimWhiteSpace(filepath, true, true) ;
	trimPathSeparator(filepath, false, true);

	//
	// remove multiple path separators
	//

	bool done = false ;
	std::string::size_type pos = 0 ;

	while(!done)
	{
		pos = filepath.find(FilePath::PATH_SEPARATOR, pos) ;
		if(pos == std::string::npos)
		{
			done = true ;
		}
		else
		{
			std::string::size_type count = 0 ;
			pos++;

			while(((pos + count) < filepath.length()) && (filepath[pos + count] == FilePath::PATH_SEPARATOR))
			{
				count++;
			}
			filepath.erase(pos, count) ;
		}
	}

	return(filepath) ;
}

