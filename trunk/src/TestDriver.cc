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

#include <cutil/TestDriver.h>

#include <cutil/AbstractTestReporter.h>
#include <cutil/AbstractUnitTest.h>
#include <cutil/Exception.h>
#include <cutil/TestLog.h>
#include <cutil/TestManager.h>

using cutil::AbstractUnitTest ;
using cutil::TestDriver ;
using cutil::TestLog ;
using cutil::TestManager ;

TestDriver::TestDriver()
{
}

void
TestDriver::runAllTests(AbstractTestReporter& reporter)
{
	std::list<AbstractUnitTest*> tests ;
	TestManager::instance().getAllTests(tests) ;

	for(std::list<AbstractUnitTest*>::const_iterator citer = tests.begin(); citer != tests.end(); ++citer)
	{
		// assume test exists.
		AbstractUnitTest* test = *citer ;
		TestLog test_log ;

		reporter.TestStarted(test->getCategory(), test->getName()) ;
		try
		{
			test->run() ;
		}
		catch(Exception& e)
		{
			std::string msg = "Caught Exception : " + e.toString() ;
			test_log.addTestResult(TestResult(test->getName(), TestResult::FAILED_ENUM, msg)) ;
		}
		catch(...)
		{
			std::string msg = "Caught Unknown Exception" ;
			test_log.addTestResult(TestResult(test->getName(), TestResult::FAILED_ENUM, msg)) ;
		}

		reporter.TestCompleted(test->getLog()) ;
	}

	
}

