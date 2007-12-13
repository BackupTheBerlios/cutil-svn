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

#include "NullableTest.h"

#include <cutil/AbstractUnitTest.h>
#include <cutil/Assert.h>
#include <cutil/Nullable.h>
#include <cutil/RefCountPtr.h>

#include <memory>
#include <string>

using namespace cutil::unit_tests ;

NullableTest::NullableTest() : cutil::AbstractUnitTest("Nullable Test", "cutil")
{
}

void
NullableTest::defaultValueIsNull()
{
	cutil::Nullable<bool> nullable ;
	cutil::Assert::isFalse(nullable.hasValue()) ;
}

void
NullableTest::hasValue()
{
	cutil::Nullable<bool> nullable ;
	cutil::Assert::isFalse(nullable.hasValue()) ;
	
	nullable.setValue(true) ;
	cutil::Assert::isTrue(nullable.hasValue()) ;
}

void
NullableTest::valueCanBeSet()
{
	cutil::Nullable<bool> nullable(true) ;
	cutil::Assert::areEqual(true, nullable.getValue()) ;
	
	nullable.setValue(false) ;
	cutil::Assert::areEqual(false, nullable.getValue()) ;
}

void
NullableTest::valueCanBeCleared()
{
	cutil::Nullable<bool> nullable(true) ;
	cutil::Assert::isTrue(nullable.hasValue()) ;

	nullable.clear() ;
	cutil::Assert::isFalse(nullable.hasValue()) ;
}

std::vector<cutil::RefCountPtr<const cutil::AbstractTestCase> >
NullableTest::getTestCases()
{
	std::vector<cutil::RefCountPtr<const cutil::AbstractTestCase> > test_cases ;

	test_cases.push_back(makeTestCase<NullableTest>(this, &NullableTest::defaultValueIsNull, "defaultValueIsNull", "", ""));
	test_cases.push_back(makeTestCase<NullableTest>(this, &NullableTest::hasValue, "hasValue", "", ""));
	test_cases.push_back(makeTestCase<NullableTest>(this, &NullableTest::valueCanBeSet, "valueCanBeSet", "", ""));
	test_cases.push_back(makeTestCase<NullableTest>(this, &NullableTest::valueCanBeCleared, "valueCanBeCleared", "", ""));

	// copy on return
	return(test_cases) ;
}
