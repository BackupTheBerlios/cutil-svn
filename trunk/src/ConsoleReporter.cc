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

#include <cutil/ConsoleReporter.h>

#include <cutil/TestLog.h>
#include <cutil/TestResult.h>

#include <iostream>
#include <iomanip>
#include <list>
#include <string>

using cutil::ConsoleReporter ;
using cutil::TestLog ;
using cutil::TestResult ;

ConsoleReporter::ConsoleReporter()
{
}

ConsoleReporter::~ConsoleReporter()
{
}

void
ConsoleReporter::testStarted(const std::string& category, const std::string& name)
{
	std::cout << "****\n" ;
	std::cout << "** Category : " << category << "\n" ;
	std::cout << "** Test : " << name << std::endl ;
	std::cout << "**\n" ;
}

void
ConsoleReporter::testCompleted(const TestLog& test_log)
{
	std::list<TestResult> failed_tests ;
	std::list<TestResult> passed_tests ;

	test_log.getFailures(failed_tests) ;
	test_log.getPasses(passed_tests) ;

	for(std::list<TestResult>::const_iterator citer = failed_tests.begin(); citer != failed_tests.end(); ++citer)
	{
		std::cout << "** " << std::setw(4) << ":(" << std::setw(12) << "Failed! : " << (*citer).getName() ;
		if(!(*citer).getMessage().empty())
		{
			std::cout << " : " << (*citer).getMessage() ;
		}
		std::cout << "\n" ;
	}

	for(std::list<TestResult>::const_iterator citer = passed_tests.begin(); citer != passed_tests.end(); ++citer)
	{
		std::cout << "** " << std::setw(4) << std::left << ":)" << std::setw(12) << std::right << "Passed! : " << (*citer).getName() ;
		if(!(*citer).getMessage().empty())
		{
			std::cout << " : " << (*citer).getMessage() ;
		}
		std::cout << "\n" ;
	}

	// display summary for this test.
	size_t total_tests = passed_tests.size() + failed_tests.size() ;

	std::cout << "**\n" ;
	std::cout << "** " ;
	if(failed_tests.empty())
	{
		std::cout << "Test Completed : " ;
		std::cout << passed_tests.size() << "/" << total_tests << " tests passed\n" ;
	}
	else
	{
		std::cout << "Test Failed : " ;
		std::cout << passed_tests.size() << "/" << total_tests << " tests passed" ;
		std::cout << ", " << failed_tests.size() ;
		if(failed_tests.size() > 1)
		{
			std::cout << " test cases failed\n" ;
		}
		else
		{
			std::cout << " test case failed\n" ;
		}
	}
	std::cout << "****\n" << std::endl ;
}

void
ConsoleReporter::displaySummary(const TestLog& test_log)
{
	std::list<TestResult> failed_tests ;
	std::list<TestResult> passed_tests ;

	test_log.getFailures(failed_tests) ;
	test_log.getPasses(passed_tests) ;
	size_t total_tests = passed_tests.size() + failed_tests.size() ;

	std::cout << "\n" ;
	std::cout << "**********************\n" ;
	std::cout << "  :: Test Summary ::  \n" ;
	std::cout << "**********************\n\n" ;
	if(passed_tests.size() != total_tests)
	{
		std::cout << " :( Test run Failed!\n\n";
	}
	else
	{
		std::cout << " :) Test run Passed!\n\n";
	}

	std::cout << "Total Tests  : " << total_tests << "\n";
	std::cout << "Tests Passed : " << passed_tests.size() << "\n" ;
	std::cout << "Tests Failed : " << failed_tests.size() << "\n" ;

	std::cout << std::endl ;
}
