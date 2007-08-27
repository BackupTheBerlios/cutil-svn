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

void
ConsoleReporter::TestStarted(const std::string& category, const std::string& name)
{
	std::cout << "****\n" ;
	std::cout << "** Category : " << category << "\n" ;
	std::cout << "** Test : " << name << std::endl ;
	std::cout << "**\n" ;
}

void
ConsoleReporter::TestCompleted(const TestLog& test_log)
{
	std::list<TestResult> failed_tests;
	std::list<TestResult> passed_tests;

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

	std::cout << "**\n" ;
	std::cout << "** Test Completed\n" ;
	std::cout << "****\n" << std::endl ;
}
