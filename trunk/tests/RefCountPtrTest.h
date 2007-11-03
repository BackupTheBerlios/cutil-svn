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

#ifndef _CUTIL_UNITTESTS_REFCOUNTPTRTEST_H_
#define _CUTIL_UNITTESTS_REFCOUNTPTRTEST_H_

#include <cutil/AbstractUnitTest.h>
#include <cutil/Closure.h>
#include <cutil/RefCountPtr.h>

namespace cutil
{
	namespace unit_tests
	{
		class RefCountPtrTest : public cutil::AbstractUnitTest
		{
			public:
				RefCountPtrTest() ;
				std::vector<cutil::RefCountPtr<cutil::AbstractTestCase> > getTestCases() ;

				void pointsToNullOnConstruction() ;
				void refCountIncreasesWhenAssigned() ;
				void refCountIncreasesWhenCopyConstructorCalled() ;
				void refCountDecreasesWhenOutOfScope() ;
				void assignmentPointsToSameInstance() ;
				void copyConstructorPointsToSameInstance() ;
				void destructorCalledWhenLastReferenceCleared() ;
				void destructorCalledWhenLastReferenceFallsOutOfScope() ;
				void canBeCleared() ;
				void canBindNewPointer() ;
				void canCompareForEquality() ;
				void canBePlacedInStlContainer() ;
		} ;
	}
}

#endif /* _CUTIL_UNITTESTS_REFCOUNTPTRTEST_H_ */
