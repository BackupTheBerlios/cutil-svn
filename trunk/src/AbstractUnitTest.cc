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

#include <cutil/AbstractUnitTest.h>

#include <cutil/Closure.h>
#include <cutil/Exception.h>
#include <cutil/TestLog.h>
#include <cutil/TestManager.h>

#include <iostream>

using cutil::AbstractUnitTest ;
using cutil::AbstractClosure ;
using cutil::AbstractTestCase ;
using cutil::Exception ;
using cutil::TestLog ;
using cutil::TestManager ;

AbstractUnitTest::~AbstractUnitTest()
{
}

AbstractUnitTest::AbstractUnitTest(std::string name, std::string category)
	: m_name(name), m_category(category), m_log(new TestLog())
{
	TestManager::instance().registerTest(*this) ;
}

void
AbstractUnitTest::run()
{
	std::vector<cutil::RefCountPtr<AbstractTestCase> > test_cases = getTestCases() ;
	executeTestCases(test_cases) ;
}

std::vector<cutil::RefCountPtr<AbstractTestCase> >
AbstractUnitTest::getTestCases()
{
	std::vector<cutil::RefCountPtr<AbstractTestCase> > test_cases ;

	// copy on return
	return(test_cases) ;
}

const std::string&
AbstractUnitTest::getName() const
{
	return(m_name) ;
}

const std::string&
AbstractUnitTest::getCategory() const
{
	return(m_category) ;
}

const TestLog&
AbstractUnitTest::getLog() const
{
	return(*m_log) ;
}

void
AbstractUnitTest::executeTestCases(std::vector<cutil::RefCountPtr<AbstractTestCase> > test_cases)
{
	for(std::vector<AbstractTestCase>::size_type i = 0; i < test_cases.size(); i++)
	{
		cutil::RefCountPtr<AbstractTestCase> test_case = test_cases[i] ;
		test_case->run(*m_log) ;
	}
}
