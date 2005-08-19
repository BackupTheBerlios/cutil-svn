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

#ifndef _CUTIL_PLUGINMANAGEREXCEPTION_H_
#define _CUTIL_PLUGINMANAGEREXCEPTION_H_

#include <cutil/Exception.h>

#include <string>

namespace cutil
{
	/**
	 * PluginManagerException.
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	class PluginManagerException : public Exception
	{
		public:
			/**
			 * Construct a new PluginManagerException with the specified error message.
			 *
			 * @param message the error message
			 */
			PluginManagerException(const std::string& message) ;

			//-------------------------------------------------------------------------------//

		protected:

			//-------------------------------------------------------------------------------//

		private:

	} ; /* class PluginManagerException */

} /* namespace cutil */


#endif /* _CUTIL_PLUGINMANAGEREXCEPTION_H_ */