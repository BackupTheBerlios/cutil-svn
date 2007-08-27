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

#ifndef _CUTIL_ABSTRACTUNITTEST_H_
#define _CUTIL_ABSTRACTUNITTEST_H_

#include <cutil/Closure.h>

#include <memory>
#include <list>
#include <string>

namespace cutil
{
	class TestLog ;

	/**
	 * AbstractUnitTest provides a base class for unit tests.
	 * Unit tests instances should derive from this abstract base, providing a implementation
	 * of the run method to perform the actual tests. Tests should log results, pass or fail to
	 * the test log, which can later be queried to determine the status of the test run.
	 *
	 * Typically, a unit test implementation will will run a series of individual tests or steps for
	 * a particular unit. The runTestStep helper method can be used to automatically run each of these
	 * step, wrapping the test method within a try/catch block and logging as appropriate for the
	 * results of the test. In this setup, a failing test will typically throw an exception, which
	 * will be caught and logged to the test log.
	 *
	 */
	class AbstractUnitTest
	{
		public:
			/**
			 * Virtual destructor, does nothing in this abstract base
			 *
			 */
			virtual ~AbstractUnitTest() ;

			/**
			 * Invoked by a test runner to perform the test.
			 * Subclasses must implement this method to perform some useful testing.
			 *
			 */
			virtual void run() = 0;

			/**
			 * Gets the nams of this test
			 *
			 * @return name of this test
			 */
			const std::string& getName() const ;

			/**
			 * Gets the catrogiry to which this test belongs
			 *
			 * @return category of this test
			 */
			const std::string& getCategory() const ;

			/**
			 * Provides access to the log of events recorded by this test.
			 *
			 * @return this tests log.
			 */
			const TestLog& getLog() const ;

		protected:
			/**
			 * Protected constructor
			 * The combination of name and category must be unique amongts all registered tests.
			 *
			 * @param name the name of this test.
			 * @param category the category containing this test
			 */
			AbstractUnitTest(std::string name, std::string category) ;

			/**
			 * Helper method to run a test step, wrapped as an AbstractClosure<void>, automatically
			 * wrapping the invocation of the AbstractClosure<void> in a try/catch block. If an exception is thrown
			 * during the invocations of the AbstractClosure<void>, the step is deemed to have failed and
			 * a failure will be logged the the test log.
			 *
			 * @param test_step AbstractClosure<void> representing an action to be executed within a try/catch block
			 * @param step_name name of the step which will be written to the test log on success or failure
			 * @param failure_msg message written to the test log if the test step throws an exception.
			 */
			void runTestStep(const AbstractClosure<void>& test_step, std::string step_name = "", std::string failure_msg = "") ;

		private:
			/** name of this test */
			std::string m_name ;

			/** category of this test */
			std::string m_category ;

			/** log of test results */
			std::auto_ptr<TestLog> m_log ;

	} ; /* class AbstractUnitTest */

} /* namespace cutil */

#endif /* _CUTIL_ABSTRACTUNITTEST_H_ */
