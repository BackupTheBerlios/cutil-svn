/*
 * Copyright (C) 2007  Colin Law
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
 *
 * $Id$
 */

#ifndef _CUTIL_CONSOLEREPORTER_H_
#define _CUTIL_CONSOLEREPORTER_H_

#include <cutil/AbstractTestReporter.h>

namespace cutil
{
	class TestLog ;

	/**
	 * ConsoleReporter provides a AbstractTestReporter implementation for reporting unit test
	 * progress to the console.
	 *
	 */
	class ConsoleReporter : public AbstractTestReporter
	{
		public:
			/**
			 * Constructs a new instance of an ConsolReporter
			 *
			 */
			ConsoleReporter() ;

			/**
			 * Indicates that the named test has been started.
			 *
			 * @param category category of the test started
			 * @param name the name of the test stated.
			 */
			virtual void TestStarted(const std::string& category, const std::string& name) ;

			/**
			 * Indicates that the current test has completed.
			 *
			 * @param test_log log of results from the unit test.
			 */
			virtual void TestCompleted(const TestLog& test_log) ;

	} ; /* class TestReporter */

} /* namespace cutil */

#endif /* _CUTIL_CONSOLEREPORTER_H_ */
