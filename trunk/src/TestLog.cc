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

#include <cutil/TestLog.h>

#include <cutil/TestResult.h>

#include <algorithm>
#include <list>

using cutil::TestLog ;
using cutil::TestResult ;

TestLog::TestLog()
{

}

void
TestLog::addTestPass(const std::string& name, std::string msg)
{
	m_results.push_back(TestResult(name, TestResult::PASSED_ENUM, msg)) ;
}

void
TestLog::addTestFailure(const std::string& name, std::string failure_msg)
{
	m_results.push_back(TestResult(name, TestResult::FAILED_ENUM, failure_msg)) ;
}

void
TestLog::addTestResult(const TestResult& test_result)
{
	m_results.push_back(test_result) ;
}

std::list<TestResult>&
TestLog::getPasses(std::list<TestResult>& results) const
{
	for(std::list<TestResult>::const_iterator citer = m_results.begin(); citer != m_results.end(); ++citer)
	{
		if((*citer).getStatus() == TestResult::PASSED_ENUM)
		{
			results.push_back(*citer) ;
		}
	}

	results.sort(TestResultNameComparator()) ;

	return(results) ;
}

std::list<TestResult>
TestLog::getFailures(std::list<TestResult>& results) const
{
	for(std::list<TestResult>::const_iterator citer = m_results.begin(); citer != m_results.end(); ++citer)
	{
		if((*citer).getStatus() == TestResult::FAILED_ENUM)
		{
			results.push_back(*citer) ;
		}
	}

	results.sort(TestResultNameComparator()) ;

	return(results) ;
}

void
TestLog::merge(const TestLog& other)
{
	for(std::list<TestResult>::const_iterator citer = other.m_results.begin(); citer != other.m_results.end(); ++citer)
	{
		m_results.push_back(*citer) ;
	}
}

bool
TestLog::TestResultNameComparator::operator() (const TestResult& x, const TestResult& y) const
{
	return(x.getName() < y.getName()) ;
}
