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

#include "MapIteratorTest.h"

#include <cutil/Assert.h>
#include <cutil/DefaultTestCase.h>
#include <cutil/MapIterator.h>
#include <cutil/TestLog.h>
#include <cutil/TestResult.h>

#include <map>
#include <string>

using namespace cutil::unit_tests ;

int expected_container_keys[] = {2, 4, 8, 16, 32, 64 } ;
std::string expected_container_values[] = {"two", "four", "eight", "sixteen", "thirty-two", "sixy-four" } ;

class Container
{
	private:
		typedef std::map<int, std::string> Container_t  ;
		Container_t m_map ;

	public:
		Container() ;

		typedef cutil::MapKeyIterator<const Container_t, Container_t::const_iterator> const_iterator_keys ;
		typedef cutil::MapValueIterator<const Container_t, Container_t::const_iterator> const_iterator_vals ;

		const_iterator_keys begin_keys() const ;
		const_iterator_keys end_keys() const ;

		const_iterator_vals begin_vals() const ;
		const_iterator_vals end_vals() const ;
};

Container::Container()
{
	for(int i = 0; i < 6; i++)
	{
		m_map.insert(std::pair<int, std::string>(expected_container_keys[i], expected_container_values[i])) ;
	}
}

Container::const_iterator_keys Container::begin_keys() const
{
	Container_t::const_iterator citer = m_map.begin() ;
	const_iterator_keys ret(m_map, citer) ;
	return(ret) ;
}

Container::const_iterator_keys Container::end_keys() const
{
	Container_t::const_iterator citer = m_map.end() ;
	const_iterator_keys ret(m_map, citer) ;
	return(ret) ;
}

Container::const_iterator_vals Container::begin_vals() const
{
	Container_t::const_iterator citer = m_map.begin() ;
	const_iterator_vals ret(m_map, citer) ;
	return(ret) ;
}

Container::const_iterator_vals Container::end_vals() const
{
	Container_t::const_iterator citer = m_map.end() ;
	const_iterator_vals ret(m_map, citer) ;
	return(ret) ;
}


MapIteratorTest::MapIteratorTest() : cutil::AbstractUnitTest("Map Iterator Test", "cutil")
{}

void
MapIteratorTest::run()
{
	Container test_container ;

	int count = 0 ;
	for(Container::const_iterator_keys citer = test_container.begin_keys(); citer != test_container.end_keys(); ++citer)
	{
		Assert::areEqual<int>(expected_container_keys[count++], (*citer)) ;
	}

	count = 0 ;
	for(Container::const_iterator_vals citer = test_container.begin_vals(); citer != test_container.end_vals(); ++citer)
	{
		Assert::areEqual<std::string>(expected_container_values[count++], (*citer)) ;
	}

	getLog().addTestResult(TestResult(getName(), TestResult::PASSED_ENUM)) ;
}
