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

#include "EnumTest.h"

#include <cutil/Assert.h>
#include <cutil/DefaultTestCase.h>
#include <cutil/Enum.h>
#include <string>

using namespace cutil::unit_tests ;

class FooEnum : public cutil::Enum<FooEnum>
{
	public:
		static const FooEnum ENUM_A ;
		static const FooEnum ENUM_B ;
		static const FooEnum ENUM_C ;

		~FooEnum() {} ;
	private:
		FooEnum(std::string description) : cutil::Enum<FooEnum>(description) {} ;
} ;

template cutil::Enum<FooEnum>::EnumInstanceList cutil::Enum<FooEnum>::theEnumInstances ;
template<> int cutil::Enum<FooEnum>::theNextValue = 0 ;

std::string expected_descriptions[] = { "Enum A", "Enum B", "Enum C" } ;

const FooEnum FooEnum::ENUM_A(expected_descriptions[0]) ;
const FooEnum FooEnum::ENUM_B(expected_descriptions[1]) ;
const FooEnum FooEnum::ENUM_C(expected_descriptions[2]) ;

EnumTest::EnumTest() : cutil::AbstractUnitTest("Enum Test", "cutil")
{}

std::vector<cutil::RefCountPtr<const cutil::AbstractTestCase> >
EnumTest::getTestCases()
{
	std::vector<cutil::RefCountPtr<const cutil::AbstractTestCase> > test_cases ;

	test_cases.push_back(makeTestCase<EnumTest>(this, &EnumTest::canIterateOverEnum, "canIterateOverEnum", "", ""));
	test_cases.push_back(makeTestCase<EnumTest>(this, &EnumTest::canGetFirst, "canGetFirst", "", ""));
	test_cases.push_back(makeTestCase<EnumTest>(this, &EnumTest::canGetLast, "canGetLast", "", ""));
	test_cases.push_back(makeTestCase<EnumTest>(this, &EnumTest::canGetValue, "canGetValue", "", ""));
	test_cases.push_back(makeTestCase<EnumTest>(this, &EnumTest::canGetByValue, "canGetByValue", "", ""));
	test_cases.push_back(makeTestCase<EnumTest>(this, &EnumTest::hasValue, "hasValue", "", ""));

	// copy on return
	return(test_cases) ;
}


void
EnumTest::canIterateOverEnum()
{
	int count = 0;
	for(FooEnum::const_iterator citer = FooEnum::begin(); citer != FooEnum::end(); ++citer)
	{
		const FooEnum* foo = *citer ;
		Assert::areEqual<std::string>(expected_descriptions[count], foo->getDescription()) ;

		count++ ;
	}
}

void
EnumTest::canGetFirst()
{
	Assert::areEqual<std::string>(expected_descriptions[0], FooEnum::getFirst()->getDescription()) ;
}

void
EnumTest::canGetLast()
{
	Assert::areEqual<std::string>(expected_descriptions[2], FooEnum::getLast()->getDescription()) ;
}

void
EnumTest::canGetValue()
{
	Assert::areEqual<int>(0, FooEnum::getFirst()->getValue()) ;
	Assert::areEqual<int>(2, FooEnum::getLast()->getValue()) ;
}

void
EnumTest::canGetByValue()
{
	Assert::areEqual<std::string>(expected_descriptions[0], FooEnum::getByValue(0)->getDescription()) ;
	Assert::areEqual<std::string>(expected_descriptions[1], FooEnum::getByValue(1)->getDescription()) ;
	Assert::areEqual<std::string>(expected_descriptions[2], FooEnum::getByValue(2)->getDescription()) ;
}

void
EnumTest::hasValue()
{
	Assert::isTrue(FooEnum::hasValue(0)) ;
	Assert::isFalse(FooEnum::hasValue(10)) ;
}
