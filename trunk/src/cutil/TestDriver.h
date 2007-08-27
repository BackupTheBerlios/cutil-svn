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

#ifndef _CUTIL_TESTDRIVER_H_
#define _CUTIL_TESTDRIVER_H_

#include <string>

namespace cutil
{
	class AbstractTestReporter ;

	/**
	 * TestDriver provides a mechanism to drive the running of unit tests.
	 * TestDriver iterates over tests managed by the singleton TestManager, running each test.
	 *
	 */
	class TestDriver
	{
		public:
			/**
			 * Constructs a new instance of a TestDriver
			 *
			 */
			TestDriver() ;

			/**
			 * Runs all tests managed by the TestManager singleton, logging progress to the specified
			 * AbstractTestReporter
			 *
			 * @param reporter AbstractTestReporter used to log the progress of the test run
			 */
			void runAllTests(AbstractTestReporter& reporter) ;

	} ; /* class TestDriver */

} /* namespace cutil */

#endif /* _CUTIL_TESTDRIVER_H_ */
