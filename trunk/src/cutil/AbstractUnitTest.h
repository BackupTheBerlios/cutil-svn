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

#include <cutil/AbstractTestCase.h>
#include <cutil/DefaultTestCase.h>
#include <cutil/Closure.h>
#include <cutil/RefCountPtr.h>

#include <memory>
#include <string>
#include <vector>

namespace cutil
{
	class TestLog ;

	/**
	 * AbstractUnitTest provides a base class for unit tests.
	 * Unit tests instances should derive from this abstract base to perform the actual tests.
	 *
	 * AbstractUnitTest supports two ways of implementing a unit test. An implementation may
	 * either re-implementation the run method, the getTestCases or both.
	 *
	 * By re-implementing run, a unit test can be in complete control of the test, performing
	 * any test action as required. Re-implementing getTestCases allow a unit test to define a
	 * list of test steps to be run which may be executed by the run method (which is its
	 * default implementation) or treated specifically within the overriden run method. 
	 * Providing test cases allows a test to wrap up each test step performing any failure
	 * and exception handling and logging as required.
	 *
	 * Tests should log results, pass or fail to the test log, which can later be queried to
	 * determine the status of the test run.
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
			 * This implementation invokes each test case returned by getTestCases.
			 *
			 * If a unit test implementation is interested in providing a list of test cases only,
			 * this implementation should suffice, otherwise implementations should override this
			 * method to perform appropriate testing.
			 *
			 */
			virtual void run() ;
			
			/**
			 * Returns the collection of AbstractTestCase exposed by this AbstractUnitTest.
			 * This default implementation returns an empty collection
			 *
			 * Unit test implementation should override this method if they wish to return a
			 * list of AbstractTestCase to be run.
			 *
			 * @return default implementation returns an empty collection.
			 */
			virtual std::vector<cutil::RefCountPtr<AbstractTestCase> > getTestCases() ;

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
			 * Executes each AbstractTestCase specified within test_cases
			 *
			 * @param test_cases list of AbstractTestCases to be run.
			 */
			void executeTestCases( std::vector<cutil::RefCountPtr<AbstractTestCase> > test_cases) ;

			/**
			 * Helper method to construct an AbstractTestCase
			 *
			 * @param obj unit test instance on which testMethod is to be called.
			 * @param testMethod function pointer to the test method to be called upon obj
			 * @param step_name user visible name of the test step
			 * @param pass_msg user visible message written to the test log if the test step passes
			 * @param fail_msg user visible message writtin to the test log if the test fails.
			 */
			template<class T>
			cutil::RefCountPtr<cutil::AbstractTestCase>
			makeTestCase(T* obj, void (T::*testMethod)(), std::string step_name, std::string pass_msg, std::string fail_msg) ;

		private:
			/** name of this test */
			std::string m_name ;

			/** category of this test */
			std::string m_category ;

			/** log of test results */
			std::auto_ptr<TestLog> m_log ;

	} ; /* class AbstractUnitTest */


	// template implementation

	template<class T>
	cutil::RefCountPtr<cutil::AbstractTestCase>
	AbstractUnitTest::makeTestCase(T* obj, void (T::*testMethod)(), std::string step_name, std::string pass_msg, std::string fail_msg)
	{
		cutil::RefCountPtr<const cutil::AbstractClosure<void> > test_step(new cutil::Closure0<void, T>(obj, testMethod)) ;
		cutil::RefCountPtr<cutil::AbstractTestCase> test_case(new cutil::DefaultTestCase(test_step, step_name, pass_msg, fail_msg)) ;
		return(test_case) ;
	}

} /* namespace cutil */

#endif /* _CUTIL_ABSTRACTUNITTEST_H_ */
