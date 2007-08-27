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

#include <cutil/TestManager.h>

#include <cutil/AbstractUnitTest.h>

using cutil::AbstractUnitTest ;
using cutil::TestManager ;

std::auto_ptr<TestManager> TestManager::s_instance ;

TestManager::TestManager()
{

}

TestManager& TestManager::instance()
{
	if(!s_instance.get())
	{
		s_instance.reset(new TestManager()) ;
	}

	return *(s_instance.get()) ;
}

void
TestManager::registerTest(AbstractUnitTest& test)
{
	m_tests[test.getCategory()][test.getName()] = &test ;
}

const AbstractUnitTest*
TestManager::getTest(const std::string& category, const std::string& name)
{
	const AbstractUnitTest* test = 0 ;

	TestContainer::iterator suite_iter = m_tests.find(category) ;
	if(suite_iter != m_tests.end())
	{
		TestCategory::iterator test_iter = (suite_iter->second).find(name) ;
		if(test_iter != (suite_iter->second).end())
		{
			test = test_iter->second ;
		}
	}

	return test ;
}

std::list<AbstractUnitTest*>&
TestManager::getAllTests(std::list<AbstractUnitTest*>& tests)
{
	for(TestContainer::iterator category_citer = m_tests.begin(); category_citer != m_tests.end(); ++category_citer)
	{
		for(TestCategory::iterator test_iter = (category_citer->second).begin(); test_iter != (category_citer->second).end(); ++test_iter)
		{
			tests.push_back(test_iter->second) ;
		}
	}

	return(tests) ;
}
