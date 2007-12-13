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

#ifndef _CUTIL_UNITTESTS_ENUMTEST_H_
#define _CUTIL_UNITTESTS_ENUMTEST_H_

#include <cutil/AbstractUnitTest.h>
#include <cutil/Closure.h>
#include <cutil/RefCountPtr.h>

namespace cutil
{
	namespace unit_tests
	{
		class EnumTest : public cutil::AbstractUnitTest
		{
			public:
				EnumTest() ;
				std::vector<cutil::RefCountPtr<const cutil::AbstractTestCase> > getTestCases() ;

				void canIterateOverEnum() ;
				void canGetFirst() ;
				void canGetLast() ;
				void canGetValue() ;
				void canGetByValue() ;
				void hasValue() ;
		} ;
	}
}

#endif /* _CUTIL_UNITTESTS_ENUMTEST_H_ */
