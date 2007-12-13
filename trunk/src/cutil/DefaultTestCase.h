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

#ifndef _CUTIL_DEFAULTTESTCASE_H_
#define _CUTIL_DEFAULTTESTCASE_H_

#include <cutil/AbstractTestCase.h>

#include <cutil/Closure.h>

#include <string>

namespace cutil
{
	class TestLog ;

	/**
	 * DefaultTestCase provides an AbstractTestCase implementation which catches
	 * and reports cutil::Exception's, It also catches all other exceptions (...)
	 * reporting an enexpected exception was raised,
	 *
	 */
	class DefaultTestCase : public AbstractTestCase
	{
		public:
			/**
			 * Constructs a new instance of a DefaultTestCase. 
			 *
			 * @param test_step the test case/step to be executed
			 * @param step_name user visible name of the test step
			 * @param pass_msg user visible message written to the test log if the test step passes
			 * @param fail_msg user visible message writtin to the test log if the test fails.
			 */
			DefaultTestCase(RefCountPtr<const AbstractClosure<void> > test_step, std::string step_name, std::string pass_msg, std::string fail_msg) ;

			/**
			 * Constructs a new instance of a DefaultTestCase wrapping another AbstractTestCase instance.
			 *
			 * @param test_case an existing AbstractTestCase to be wrapped by this new instance.
			 */
			DefaultTestCase(RefCountPtr<const AbstractTestCase> test_case) ;

			/**
			 * Executes this AbstractTestCase logging the results as appropriate to log
			 *
			 * @param log the log into which test results are written
			 */
			virtual void run(TestLog& log) const ;

	} ; /* class DefaultTestCase */

} /* namespace cutil */

#endif /* _CUTIL_DEFAULTTESTCASE_H_ */
