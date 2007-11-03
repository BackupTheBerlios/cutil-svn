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

#ifndef _CUTIL_TESTLOG_H_
#define _CUTIL_TESTLOG_H_

#include <cutil/TestResult.h>

#include <list>
#include <string>

namespace cutil
{
	/**
	 * TestLog provides a log of individual unit test or test step results
	 *
	 */
	class TestLog
	{
		public:
			/**
			 * Constructs a new instance of a TestLog
			 *
			 */
			TestLog() ;

			/**
			 * Adds a passing test result to this TestLog
			 *
			 * @param name the name of the passing test or test step
			 * @param msg optional message string to add to the test result
			 */
			void addTestPass(const std::string& name, std::string msg = "") ;

			/**
			 * Adds a failing test result to this TestLog
			 *
			 * @param name the name of the failing test or test step
			 * @param failure_msg optional message to add to the test result
			 */
			void addTestFailure(const std::string& name, std::string failure_msg = "") ;

			/**
			 * Adds the specified TestResult to this TestLog
			 *
			 * @param test_result the TestResult to be added
			 */
			void addTestResult(const TestResult& test_result) ;

			/**
			 * Gets a list of all passing tests reported within this TestLog.
			 * The list is sorted by the test name
			 * 
			 * @param results populated with all passing test results.
			 * @return results parameter populated with the passing test results.
			 */
			std::list<TestResult>& getPasses(std::list<TestResult>& results) const ;

			/**
			 * Gets a list of all failing tests reported within this TestLog.
			 * The list is sorted by the test name
			 *
			 * @param results populated with all failing test results.
			 * @return results parameter populated with the failing test results.
			 */
			std::list<TestResult> getFailures(std::list<TestResult>& results) const ;

			/**
			 * merges the contents of other with this TestLog
			 *
			 * @param other the TestLog to merge with this TestLog
			 */
			void merge(const TestLog& other) ;

		private:
			/** list of test results */
			std::list<TestResult> m_results ;


			/**
			 * Function Class to sort TestResult objects by the name of the test.
			 *
			 */
			class TestResultNameComparator
			{
				public:
					bool operator() (const TestResult& x, const TestResult& y) const ;
			} ; /* class TestResultNameComparator */

	} ; /* class TestLog */

} /* namespace cutil */

#endif /* _CUTIL_TESTLOG_H_ */
