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

#ifndef _CUTIL_ABSTRACTTESTCASE_H_
#define _CUTIL_ABSTRACTTESTCASE_H_

#include <cutil/Closure.h>
#include <cutil/RefCountPtr.h>

#include <memory>
#include <string>

namespace cutil
{
	class TestLog ;

	/**
	 * AbstractTestCase provides a wrapping around a unit test, test case or test step.
	 * AbstractTestCase is useful for wrapping a test case in such a way as to
	 * handle the results of a test, for example to wrap the running of a test
	 * step catching any unexpected exception and failing the test with a suitable log message.
	 *
	 * AbstractTestCase implementations should also be capable of wrapping other
	 * AbstractTestCase instances, therefore allowing the building up of complex
	 * AbstractTestCase execution. This may be useful, for example to have one
	 * implementation catch certain common exceptions types, which is wrapped by
	 * an instance catching more specific types.
	 *
	 */
	class AbstractTestCase
	{
		public:
			/**
			 * destructor
			 */
			virtual ~AbstractTestCase() ;

			/**
			 * Executes this AbstractTestCase logging the results as appropriate to log
			 *
			 * @param log the log into which test results are written
			 */
			virtual void run(TestLog& log) const = 0 ;

		protected:
			/**
			 * Constructs a new instance of an AbstractTestCase
			 *
			 * @param test_step the test case/step to be executed
			 * @param step_name user visible name of the test step
			 * @param pass_msg user visible message written to the test log if the test step passes
			 * @param fail_msg user visible message writtin to the test log if the test fails.
			 */
			AbstractTestCase(RefCountPtr<const AbstractClosure<void> > test_step, std::string step_name, std::string pass_msg, std::string fail_msg) ;

			/**
			 * Constructs a new instance of a AbstractTestCase wrapping another AbstractTestCase instance.
			 *
			 * @param test_case anexisting AbstractTestCase to be wrapped by this new instance.
			 */
			AbstractTestCase(RefCountPtr<const AbstractTestCase> test_case) ;

			/**
			 * Gets the test step to be executed by this AbstractTestCase
			 *
			 * @return test step to be executed
			 */
			RefCountPtr<const AbstractClosure<void> > getTestStep() const ;

			/**
			 * Gets AbstractTestCase this AbstractTestCase wraps. If this AbstractTestCase does not
			 * wrap another, the returned RefCountPtr will not have a value.
			 *
			 * @return the AbstractTestCase wrapped by this AbstractTestCase
			 */
			RefCountPtr<const AbstractTestCase> getWrappedTestCase() const ;

			/**
			 * Indicates if this AbstractTestCase wraps another, returning true if so, false otherwise.
			 *
			 * @return true if this AbstractTestCase wraps another, false otherwise.
			 */
			bool isNested() const ;

			/**
			 * Gets the name of this test step
			 *
			 * @return the name of this test step
			 */
			const std::string& getStepName() const ;

			/**
			 * Gets the message to be written to the test log on succesful pass of the test step
			 *
			 * @return message to be written to the log on test pass
			 */
			const std::string& getPassMessage() const ;

			/**
			 * Gets the message to be written to the test log on failure of the test step
			 *
			 * @return message to be written to the log on test step failure.
			 */
			const std::string& getFailureMessage() const ;

		private:
			/** test step to be exewcuted */
			const RefCountPtr<const AbstractClosure<void> > m_test_step ;

			/** test case to be wrapped by this AbstractTestCase */
			const RefCountPtr<const AbstractTestCase> m_wrapped_test_case ;

			/** user visible name of the test step */
			std::string m_step_name ;

			/** user visible message written to the test log if the test step passes */
			std::string m_pass_msg ;

			/** user visible message writtin to the test log if the test fails */
			std::string m_fail_msg ;

	} ; /* class AbstractTestCase */

} /* namespace cutil */

#endif /* _CUTIL_ABSTRACTTESTCASE_H_ */
