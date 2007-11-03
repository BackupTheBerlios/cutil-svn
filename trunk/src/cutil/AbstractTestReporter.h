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

#ifndef _CUTIL_ABSTRACTTESTREPORTER_H_
#define _CUTIL_ABSTRACTTESTREPORTER_H_

#include <string>

namespace cutil
{
	class TestLog ;

	/**
	 * AbstractTestReporter provides a simple mechanism for reporting the status of a unit
	 * test as they are run.
	 * An AbstractTestReporter instance will typically be used with a TestDriver to report on the
	 * current state of a test run.
	 *
	 */
	class AbstractTestReporter
	{
		public:
			/**
			 * Virtual destructor, does nothing in this abstract class
			 *
			 */
			virtual ~AbstractTestReporter() {}

			/**
			 * Indicates that the named test has been started.
			 *
			 * @param category category of the test started
			 * @param name the name of the test stated.
			 */
			virtual void testStarted(const std::string& category, const std::string& name) = 0 ;

			/**
			 * Indicates that the current test has completed.
			 *
			 * @param test_log log of results from the unit test.
			 */
			virtual void testCompleted(const TestLog& test_log) = 0 ;

			/**
			 * Displays a summary after all tests have been run,
			 *
			 * @param test_log merged log of all test results.
			 */
			virtual void displaySummary(const TestLog& test_log) = 0 ;

		protected:
			/**
			 * Protected constructor to avoid isntances of this abstract class
			 *
			 */
			AbstractTestReporter() {} ;

	} ; /* class AbstractTestReporter */

} /* namespace cutil */

#endif /* _CUTIL_ABSTRACTTESTREPORTER_H_ */
