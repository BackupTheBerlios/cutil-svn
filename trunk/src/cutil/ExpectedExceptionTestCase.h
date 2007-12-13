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

#ifndef _CUTIL_EXPECTEDEXCEPTIONTESTCASE_H_
#define _CUTIL_EXPECTEDEXCEPTIONTESTCASE_H_

#include <cutil/AbstractTestCase.h>

#include <cutil/Closure.h>
#include <cutil/DefaultTestCase.h>
#include <cutil/Exception.h>
#include <cutil/TestLog.h>

#include <string>

namespace cutil
{
	/**
	 * ExpectedExceptionTestCase provides an AbstractTestCase implementation which catches
	 * a specified exception type, reporting success if the exception was thrown and caught,
	 * reportingand failure otherwise. ExpectedExceptionTestCase is suitable for wrapping within
	 * a DefaultTestCase to catch all other exceptions.
	 *
	 */
	template <class T>
	class ExpectedExceptionTestCase : public AbstractTestCase
	{
		public:
			/**
			 * Constructs a new instance of an ExpectedExceptionTestCase. 
			 *
			 * @param test_step the test case/step to be executed
			 * @param step_name user visible name of the test step
			 * @param pass_msg user visible message written to the test log if the test step passes
			 * @param fail_msg user visible message writtin to the test log if the test fails.
			 */
			ExpectedExceptionTestCase(RefCountPtr<const AbstractClosure<void> > test_step, std::string step_name, std::string pass_msg, std::string fail_msg) ;

			/**
			 * Constructs a new instance of a ExpectedExceptionTestCase wrapping another AbstractTestCase instance.
			 *
			 * @param test_case an existing AbstractTestCase to be wrapped by this new instance.
			 */
			ExpectedExceptionTestCase(RefCountPtr<const AbstractTestCase> test_case) ;

			/**
			 * Executes this ExpectedExceptionTestCase logging the results as appropriate to log
			 *
			 * @param log the log into which test results are written
			 */
			virtual void run(TestLog& log) const ;

	} ; /* class DefaultTestCase */

	template <class T>
	ExpectedExceptionTestCase<T>::ExpectedExceptionTestCase(RefCountPtr<const AbstractClosure<void> > test_step, std::string step_name, std::string pass_msg, std::string fail_msg)
		: AbstractTestCase(test_step, step_name, pass_msg, fail_msg)
	{

	}

	template <class T>
	ExpectedExceptionTestCase<T>::ExpectedExceptionTestCase(RefCountPtr<const AbstractTestCase> test_case)
		: AbstractTestCase(test_case)
	{

	}

	template <class T>
	void
	ExpectedExceptionTestCase<T>::run(TestLog& log) const
	{
		try
		{
			if(isNested())
			{
				RefCountPtr<const AbstractTestCase> wrapped = getWrappedTestCase() ;
				wrapped->run(log) ;
			}
			else
			{
				// invoke the test step closure
				RefCountPtr<const AbstractClosure<void> > test_step = getTestStep() ;
				(*test_step.getPtr())() ;

				// hmm, didn't get the exception we expected, log as fail.
				std::string msg = "Expected Exception not thrown : " + getFailureMessage()  ;
				log.addTestResult(TestResult(getStepName(), TestResult::FAILED_ENUM, msg)) ;
			}
		}
		catch(T& expected_exception)
		{
			// got expected expected exception, log as pass
			log.addTestResult(TestResult(getStepName(), TestResult::PASSED_ENUM, getPassMessage())) ;
		}
	}

} /* namespace cutil */

#endif /* _CUTIL_EXPECTEDEXCEPTIONTESTCASE_H_ */
