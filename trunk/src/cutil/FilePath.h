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


#ifndef _CUTIL_FILEPATH_
#define _CUTIL_FILEPATH_

#include <cutil/Exception.h>

#include <list>
#include <string>

namespace cutil
{
	/**
	 * Representation of a file or directory path.
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class FilePath
	{
		public:
			//-------------------------------------------------------------------------------//
			// Constructor / Desctructor

			/**
			 * Creates a new FilePath from the specified path
			 *
			 * @param path the file path
			 */
			FilePath(const std::string& path) ;

			/**
			 * Creates a new FilePath from the specified parent path and the specified child leaf path
			 *
			 * @param parent the parent directory path
			 * @param leaf the file path
			 */
			FilePath(const std::string& parent, const std::string& path) ;

			/**
			 * Creates a new FilePath from the specified parent FilePath and the specified child leaf path
			 *
			 * @param parent the parent FilePath representing the parent directory
			 * @param leaf the file path
			 */
			FilePath(const FilePath& parent, const std::string& path) ;

			/**
			 * Copy constructor
			 *
			 * @param file the FilePath object to copy
			 */
			FilePath(const FilePath& file) ;



			/**
			 * Destructor.
			 *
			 */
			virtual ~FilePath() ;



			//-------------------------------------------------------------------------------//
			// FilePath Operations

			/**
			 * Creates and empty file within the filesystem as specified by the path of this FilePath.
			 * An Exception will be thrown if the FilePath is empty, the file already exists, or any
			 * of the required parent directory components do not exist.
			 *
			 * @throw Exception if a system error occured during file creation
			 */
			void createFile() throw(Exception) ;

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
			void createDir(bool parent) throw(Exception) ;

			/**
			 * Deletes the File represented by this FilePath object
			 * If the specified file does not exist, no further action is taken, and false is returned.
			 * If this FilePath represents a directory, the directory must not contain any files
			 * before it can be deleted, otherwise an exception is thrown.
			 *
			 * @return true if the file or directory was sucessfuly deleted.
			 * @throw Exception if a system error occured during file creation
			 */
			bool deleteFile() throw(Exception) ;

			/**
			 * Renames the file or directory represented by this FilePath object to the specified name
			 * If the file does not exist, it cannot be renamed and an Exception is thrown.
			 * An Exception is thrown if the filepath of this FilePath is empty, or newfile is empty,
			 * or newfile already exists.
			 *
			 * @param newFile the new filepath to rename to
			 * @throw Exception if a system error occured during renaming
			 */
			void rename(const std::string& newFile) throw(Exception) ;

			/**
			 * Returns true if the file or directory represented by this FilePath object exists.
			 *
			 * @return true if the file or directory represented by this FilePath object exists,
			 *         otherwise false
			 * @throw Exception if a system error occured
			 */
			bool exists() const throw(Exception) ;



			//-------------------------------------------------------------------------------//
			// FilePath Component Accessors / Mutators

			/**
			 * Returns a string representation of the absolute path of this FilePath.
			 * The absolute path is the complete path of this FilePath resolving any relative
			 * path components
			 *
			 * @return string representation of the absolute path of this FilePath
			 */
			//std::string getAbsolutePath() const ;

			/**
			 * Returns a new FilePath representing the absolute path of this FilePath.
			 * The absolute path is the complete path of this FilePath resolving any relative
			 * path components
			 *
			 * @return a new FilePath representation of the absolute path of this FilePath
			 */
			//FilePath getAbsoluteFile() const ;

			/**
			 * Returns the leaf, file name or directory name represented by this FilePath object.
			 * The leaf or file name is the last component of the FilePath specification
			 *
			 * @return the leaf, file name or directory name represented by this FilePath
			 */
			std::string getLeaf() const ;

			/**
			 * Returns the parent path preceeding leading to the leaf, file or directory name.
			 * The parent path is the filepath spcification represented by this FilePath, without the
			 * leaf, file of directory name.
			 *
			 * @return the parent path of this FilePath
			 */
			std::string getParent() const ;

			/**
			 * Returns the parent path preceeding leading to the leaf, file or directory name as a FilePath.
			 * The parent path is the filepath spcification represented by this FilePath, without the
			 * leaf, file of directory name.
			 *
			 * @return the parent path of this FilePath
			 */
			FilePath getParentFile() const ;

			/**
			 * Returns a string representation of the filepath of this FilePath object
			 *
			 * @return the filepath of this FilePath represented as a string
			 */
			std::string getPath() const ;

			/**
			 * Sets the filepath of the file represented by this FilePath object
			 *
			 * @param filePath the new filepath
			 */
			void setPath(const std::string& filePath) ;

			/**
			 * Appends the specified filepath to this FilePath object
			 *
			 * @param filePath the filepath to append
			 */
			void appendPath(const std::string& filePath) ;

			/**
			 * Returns whether this FilePath has an empty filepath
			 * Empty file paths cannot point to a valid file in the filesyetem, and are
			 * generally invalid for many operations.
			 *
			 * @return true if the filepath of this FilePath is emoty, false otherwise
			 */
			bool isEmpty() const ;



			//------------------------
			// File property accessors

			/** Enumeration of possible FilePath types */
			enum FileType { DIRECTORY_ENUM, REGULAR_FILE_ENUM, FIFO_ENUM, SOCKET_ENUM, SYM_LINK_ENUM, CHAR_DEV_ENUM, BLOCK_DEV_ENUM, UNKNOWN_TYPE_ENUM } ;

			/**
			 * Retuns the file type of the file represented by this FilePath object
			 * The file type is one of the defined Filetype values.
			 * If !exists(), this method will throw an exception.
			 *
			 * @return the FileType of the file represented by this FilePath object
			 * @throw Exception if the file represented by this FileObject does not exist
			 * @throw Exception if a system error occured
			 */
			FileType getFileType() const throw(Exception) ;

			/**
			 * Returns true if the file represented by this FilePath exists and represents a directory
			 * This is a convenience method equivelant to exists() && getFileType() == DIRECTORY_ENUM.
			 *
			 * @return true if this File exists and represents a directory, false otherwise
			 * @throw Exception if a system error occured
			 */
			bool isDirectory() const throw(Exception) ;

			/**
			 * Returns true if the file represented by this FilePath exists and is a regular file
			 * This is a convenience method equivelant to exists() && getFileType() == REGULAR_FILE_ENUM.
			 *
			 * @return true if the file representaed by this FilePath object exists and is a regular file,
			 *         false otherwise
			 * @throw Exception if a system error occured
			 */
			bool isRegularFile() const throw(Exception) ;

			/**
			 * Returns whether the file representaed by tis FilePath exists and is a hidden file.
			 * Under linux, a hidden, or dot file is a file which begins with a dot.
			 *
			 * @return true if this File represented by this FilePath exists and is a hidden file, false otherwise
			 * @throw Exception if a system error occured
			 */
			bool isHidden() const throw(Exception) ;

			/**
			 * Returns true if the file representaed by this FilePath is readable by the current user/application
			 *
			 * @return true if the file represented by this FilePath is readable by the current user/application,
			 *         false otherwise
			 * @throw Exception if a system error occured
			 */
			bool isReadable() const throw(Exception) ;

			/**
			 * Returns true if the file representaed by this FilePath is writable by the current user/application
			 *
			 * @return true if the file represented by this FilePath is writable by the current user/application,
			 *         false otherwise
			 * @throw Exception if a system error occured
			 */
			bool isWritable() const throw(Exception) ;

			/**
			 * Returns whether this FilePath contains an absolute Filepath.
			 * An absolute filepath is a complete filesystem path containing a root element and
			 * no relative path components
			 *
			 * @return true if this FilePath contains an absolute filepath, false otherwise
			 */
			bool isAbsolute() const ;

			/**
			 * Returns the size of the file represented by this FilePath object.
			 * The file reprensented by this FilePath object must exist and must be a regular file.
			 *
			 * @return the size in byted of the file
			 * @throw Exception if a system error occured
			 */
			unsigned long getFileSize() const throw(Exception) ;




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
			std::list<std::string>& getFiles(std::list<std::string>& fileList) const throw(Exception) ;

			/**
			 * Popultes the specified list with the contents of the directory represented by this FilePath
			 * If this FilePath does not represent a directory, no action is taken and filelist is unchanged
			 *
			 * @param fileList populated with the contenst of the directory represented by this File
			 * @return the fileList parameter populated
			 * @throw Exception if a system error occured
			 */
			std::list<FilePath>& getFiles(std::list<FilePath>& fileList) const throw(Exception) ;



			//-------------------------------------------------------------------------------//

			/**
			 * Returns a new FilePath Object representing the current working directory
			 *
			 * @return a new FilePath Object representing the current working directory
			 * @throw Exception if a system error occured
			 */
			static FilePath currentWorkingDir() throw (Exception) ;

			//-------------------------------------------------------------------------------//

			/** Path separator character */
			static const char PATH_SEPARATOR ;

			//-------------------------------------------------------------------------------//

		protected:

			//-------------------------------------------------------------------------------//

		private:

			/**
			 * Trims white space from the ends of the spceified string
			 * 
			 * @param s the string to trim whitespace from
			 * @param front set true to trim whitespace from the front of s
			 * @param end set true to trim whitespace from the end of s
			 * @return string s with whitespce trimmed
			 */
			std::string& trimWhiteSpace(std::string& s, bool front, bool end) const ;

			/**
			 * Trim the path separator from the specified file path string
			 * 
			 * @param filepath the filepath string to trim path separator from
			 * @param front set true to trim the path separator from the front of filepath
			 * @param end set true to trim the path separator from the end of filepath
			 * @return filepath with the path separators trimmed
			 */
			std::string& trimPathSeparator(std::string& s, bool front, bool end) const ;

			/**
			 * Appends a path separator to the end pof the specified filepath.
			 * A path separator is not added if the filepath already ends with a path separator
			 *
			 * @param filePath the filepath to append the path separator to
			 * @return filePath with path separator appended
			 */
			std::string& appendPathSeparator(std::string& filePath) const ;

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
			std::string& cleanupFilePath(std::string& filepath) ;


			//-------------------------------------------------------------------------------//
			// Members

			/** the file path of this File */
			std::string theFilePath ;


	} ; /* class File */

} /* namespace cutil */


#endif /* _CUTIL_FILEPATH_ */

