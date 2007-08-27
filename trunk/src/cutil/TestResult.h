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

#ifndef _CUTIL_TESTRESULT_H_
#define _CUTIL_TESTRESULT_H_

#include <string>

namespace cutil
{
	/**
	 * TestResult is used to record the status of an individual unit test or test step
	 *
	 */
	class TestResult
	{
		public:
			/**
			 * enumeration of possible test statuses
			 *
			 */
			enum TestStatus
			{
			    PASSED_ENUM,
			    FAILED_ENUM,
			    POSTPONED_ENUM
			} ;

			/**
			 * Constructs a new TestResult with the specified TestStatus
			 *
			 * @param name the name of the test of test step for which this TestStatus associated
			 * @param status indicates the status of the test or test step
			 * @param msg optional message for detailing the results
			 */
			TestResult(const std::string& name, TestStatus status, std::string msg = "") ;

			/**
			 * Gets the name of the test or test step this TestResult applies to
			 *
			 * @return the name of the test or test step for this TestResult
			 */
			const std::string& getName() const ;

			/**
			 * Gets the TestStatus of this TestResult
			 *
			 * @return the TestStatus of this TestResult
			 */
			TestStatus getStatus() const ;

			/**
			 * Gets the optional message which may have been added to document the aspects of the test or
			 * test step being run
			 *
			 * @return optional descriptive message for this TestResult.
			 */
			const std::string& getMessage() const ;

		private:
			/** name of the test or test step */
			std::string m_name ;

			/** status of the test or test step */
			TestStatus m_status ;

			/** an optional message which for detailing the outcomes of the test or test step */
			std::string m_msg ;

	} ; /* class TestLog */

} /* namespace cutil */

#endif /* _CUTIL_TESTRESULT_H_ */
