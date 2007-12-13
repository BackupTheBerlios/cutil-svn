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

#include "RefCountPtrTest.h"

#include <cutil/DefaultTestCase.h>
#include <cutil/Assert.h>
#include <list>
#include <memory>
#include <string>

using namespace cutil::unit_tests ;

class ConstructionLog
{
	public:
		ConstructionLog()
		: m_constructor_count(0), m_destructor_count(0)
		{
		}
				
		void consructorCalled()
		{         
			m_constructor_count++ ;
		}
		
		void destructorCalled()
		{
			m_destructor_count++ ;
		}
		
		int getConstructorCount() const
		{
			return m_constructor_count ;
		}
		
		int getDestructorCount() const
		{
			return m_destructor_count ;
		}
		
	private:
		int m_constructor_count ;
		int m_destructor_count ;
} ;

class Foo
{
	public:
		Foo(ConstructionLog& log)
		: m_log(log)
		{
			m_log.consructorCalled() ;
		}
		
		~Foo()
		{
			m_log.destructorCalled() ;
		}

	private:
		ConstructionLog& m_log ;
} ;
			

RefCountPtrTest::RefCountPtrTest() : cutil::AbstractUnitTest("RefCountPtr Test", "cutil")
{}

std::vector<cutil::RefCountPtr<const cutil::AbstractTestCase> >
RefCountPtrTest::getTestCases()
{
	std::vector<cutil::RefCountPtr<const cutil::AbstractTestCase> > test_cases ;

	test_cases.push_back(makeTestCase<RefCountPtrTest>(this, &RefCountPtrTest::pointsToNullOnConstruction, "pointsToNullOnConstruction", "", ""));
	test_cases.push_back(makeTestCase<RefCountPtrTest>(this, &RefCountPtrTest::refCountIncreasesWhenAssigned, "refCountIncreasesWhenAssigned", "", ""));
	test_cases.push_back(makeTestCase<RefCountPtrTest>(this, &RefCountPtrTest::refCountIncreasesWhenCopyConstructorCalled, "refCountIncreasesWhenCopyConstructorCalled", "", ""));
	test_cases.push_back(makeTestCase<RefCountPtrTest>(this, &RefCountPtrTest::refCountDecreasesWhenOutOfScope, "refCountDecreasesWhenOutOfScope", "", ""));
	test_cases.push_back(makeTestCase<RefCountPtrTest>(this, &RefCountPtrTest::assignmentPointsToSameInstance, "assignmentPointsToSameInstance", "", ""));
	test_cases.push_back(makeTestCase<RefCountPtrTest>(this, &RefCountPtrTest::destructorCalledWhenLastReferenceCleared, "destructorCalledWhenLastReferenceCleared", "", ""));
	test_cases.push_back(makeTestCase<RefCountPtrTest>(this, &RefCountPtrTest::destructorCalledWhenLastReferenceFallsOutOfScope, "destructorCalledWhenLastReferenceFallsOutOfScope", "", ""));
	test_cases.push_back(makeTestCase<RefCountPtrTest>(this, &RefCountPtrTest::copyConstructorPointsToSameInstance, "copyConstructorPointsToSameInstance", "", ""));
	test_cases.push_back(makeTestCase<RefCountPtrTest>(this, &RefCountPtrTest::canBeCleared, "canBeCleared", "", ""));
	test_cases.push_back(makeTestCase<RefCountPtrTest>(this, &RefCountPtrTest::canBindNewPointer, "canBindNewPointer", "", ""));
	test_cases.push_back(makeTestCase<RefCountPtrTest>(this, &RefCountPtrTest::canCompareForEquality, "canCompareForEquality", "", ""));
	test_cases.push_back(makeTestCase<RefCountPtrTest>(this, &RefCountPtrTest::canBePlacedInStlContainer, "canBePlacedInStlContainer", "", ""));
	
	// copy on return
	return(test_cases) ;
}

void
RefCountPtrTest::pointsToNullOnConstruction()
{
	cutil::RefCountPtr<int> ptr ;
	Assert::isFalse(ptr.hasPtr()) ;
	Assert::isNull(ptr.getPtr()) ;
	Assert::areEqual<unsigned int>(0, ptr.getRefCount()) ;
}

void
RefCountPtrTest::refCountIncreasesWhenAssigned()
{
	cutil::RefCountPtr<int> ptr_a(new int(42)) ;
	Assert::areEqual<unsigned int>(1, ptr_a.getRefCount()) ;

	cutil::RefCountPtr<int> ptr_b = ptr_a ;
	Assert::areEqual<unsigned int>(2, ptr_a.getRefCount()) ;
	Assert::areEqual<unsigned int>(2, ptr_b.getRefCount()) ;
}

void
RefCountPtrTest::refCountIncreasesWhenCopyConstructorCalled()
{
	cutil::RefCountPtr<int> ptr_a(new int(42)) ;
	Assert::areEqual<unsigned int>(1, ptr_a.getRefCount()) ;

	cutil::RefCountPtr<int> ptr_b(ptr_a) ;
	Assert::areEqual<unsigned int>(2, ptr_a.getRefCount()) ;
	Assert::areEqual<unsigned int>(2, ptr_b.getRefCount()) ;
}

void
RefCountPtrTest::refCountDecreasesWhenOutOfScope()
{
	cutil::RefCountPtr<int> ptr_a(new int(42)) ;
	Assert::areEqual<unsigned int>(1, ptr_a.getRefCount()) ;

	{
		cutil::RefCountPtr<int> ptr_b = ptr_a ;
		Assert::areEqual<unsigned int>(2, ptr_a.getRefCount()) ;
	}
	
	Assert::areEqual<unsigned int>(1, ptr_a.getRefCount()) ;
}

void
RefCountPtrTest::destructorCalledWhenLastReferenceCleared()
{
	ConstructionLog log ;
	cutil::RefCountPtr<Foo> ptr(new Foo(log)) ;
	Assert::areEqual<unsigned int>(1, ptr.getRefCount()) ;

	Assert::areEqual<int>(0, log.getDestructorCount()) ;
	ptr.clear() ;
	Assert::areEqual<int>(1, log.getDestructorCount()) ;
}

void
RefCountPtrTest::destructorCalledWhenLastReferenceFallsOutOfScope()
{
	ConstructionLog log ;
	{
		cutil::RefCountPtr<Foo> ptr(new Foo(log)) ;
		Assert::areEqual<unsigned int>(1, ptr.getRefCount()) ;

		Assert::areEqual<int>(0, log.getDestructorCount()) ;
	}
	Assert::areEqual<int>(1, log.getDestructorCount()) ;
}

void
RefCountPtrTest::assignmentPointsToSameInstance() 
{
	cutil::RefCountPtr<int> ptr_a(new int(42)) ;
	cutil::RefCountPtr<int> ptr_b = ptr_a ;
	Assert::areEqual<int*>(ptr_a.getPtr(), ptr_b.getPtr()) ;
}

void
RefCountPtrTest::copyConstructorPointsToSameInstance()
{
	cutil::RefCountPtr<int> ptr_a(new int(42)) ;
	cutil::RefCountPtr<int> ptr_b(ptr_a) ;
	Assert::areEqual<int*>(ptr_a.getPtr(), ptr_b.getPtr()) ;
}

void
RefCountPtrTest::canBeCleared() 
{
	cutil::RefCountPtr<int> ptr_a(new int(42)) ;
	cutil::RefCountPtr<int> ptr_b = ptr_a ;

	Assert::areEqual<unsigned int>(2, ptr_a.getRefCount()) ;
	ptr_a.clear() ;

	Assert::isFalse(ptr_a.hasPtr()) ;
	Assert::isNull(ptr_a.getPtr()) ;
	Assert::areEqual<unsigned int>(0, ptr_a.getRefCount()) ;

	Assert::isTrue(ptr_b.hasPtr()) ;
	Assert::isNotNull(ptr_b.getPtr()) ;
	Assert::areEqual<unsigned int>(1, ptr_b.getRefCount()) ;
}

void
RefCountPtrTest::canBindNewPointer()
{
	ConstructionLog log_a ;
	ConstructionLog log_b ;
	cutil::RefCountPtr<Foo> ptr(new Foo(log_a)) ;
	Assert::areEqual<int>(0, log_a.getDestructorCount()) ;
	
	ptr.bind(new Foo(log_b)) ;
	Assert::areEqual<int>(1, log_a.getDestructorCount()) ;
	Assert::areEqual<int>(1, ptr.getRefCount()) ;
}

void
RefCountPtrTest::canCompareForEquality() 
{
	cutil::RefCountPtr<int> ptr_a(new int(42)) ;
	cutil::RefCountPtr<int> ptr_b = ptr_a ;
	Assert::isTrue(ptr_a == ptr_b) ;
}

void
RefCountPtrTest::canBePlacedInStlContainer()
{
	std::list<cutil::RefCountPtr<int> > container ;

	cutil::RefCountPtr<int> ptr_a(new int(42)) ;
	Assert::areEqual<unsigned int>(1, ptr_a.getRefCount()) ;

	// place into container 5 times, expect ref count to increase by 5
	for(int i = 0; i < 5; i++)
	{
		container.push_back(ptr_a) ;
	}
	Assert::areEqual<unsigned int>(6, ptr_a.getRefCount()) ;
	
	// all items point to the same instance
	for(std::list<cutil::RefCountPtr<int> >::const_iterator citer = container.begin(); citer != container.end(); ++citer)
	{
		Assert::isTrue(ptr_a == *citer) ;
	}

	// clear the original ptr, expect ref count to be 0
	ptr_a.clear() ;
	Assert::areEqual<unsigned int>(0, ptr_a.getRefCount()) ;
	
	// create new ptr from item in container, expect ref count to increase by 1
	cutil::RefCountPtr<int> ptr_b(container.back()) ;
	Assert::areEqual<unsigned int>(6, ptr_b.getRefCount()) ;

	// clear the container, expect to be left with the single ref.
	container.clear() ;
	Assert::areEqual<unsigned int>(1, ptr_b.getRefCount()) ;
}
