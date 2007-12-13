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

#include <cutil/DefaultTestCase.h>

#include <cutil/Exception.h>
#include <cutil/TestLog.h>

using cutil::DefaultTestCase ;
using cutil::Exception ;

DefaultTestCase::DefaultTestCase(RefCountPtr<const AbstractClosure<void> > test_step, std::string step_name, std::string pass_msg, std::string fail_msg)
	: AbstractTestCase(test_step, step_name, pass_msg, fail_msg)
{
	
}

DefaultTestCase::DefaultTestCase(RefCountPtr<const AbstractTestCase> test_case)
	: AbstractTestCase(test_case)
{
	
}

void
DefaultTestCase::run(TestLog& log) const
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

			// if we didn't get an exception, the test must has passed.
			log.addTestResult(TestResult(getStepName(), TestResult::PASSED_ENUM, getPassMessage())) ;
		}
	}
	catch(Exception& e)
	{
		std::string msg = getFailureMessage() + " : Caught Exception : " + e.toString() ;
		log.addTestResult(TestResult(getStepName(), TestResult::FAILED_ENUM, msg)) ;
	}
	catch(...)
	{
		std::string msg = getFailureMessage() + " : Caught Unknown Exception" ;
		log.addTestResult(TestResult(getStepName(), TestResult::FAILED_ENUM, msg)) ;
	}
}
