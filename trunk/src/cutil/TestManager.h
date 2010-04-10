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

#ifndef _CUTIL_TESTMANAGER_H_
#define _CUTIL_TESTMANAGER_H_

#include <list>
#include <map>
#include <memory>
#include <string>

namespace cutil
{
	class AbstractUnitTest ;

	/**
	 * TestManager is a singleton instance manager maintaining a list of all AbstractUnitTest created.
	 *
	 */
	class TestManager
	{
		public:
			/**
			 * returns access to the singleton instance of the TestManager
			 *
			 */
			static TestManager& instance() ;

			/**
			 * Registers the specified AbstractUnitTest with this TestManager.
			 * AbstractUnitTest implementation typically do not need to perform this registration.
			 * The test will be registered automatically by the AbstractUnitTest base class.
			 *
			 * @param test the test to be registered.
			 */
			void registerTest(AbstractUnitTest& test) ;

			/**
			 * Gets the specified unit test
			 *
			 * @param category the category to which the unit test belongs
			 * @param name the name of the unit test
			 */
			const AbstractUnitTest* getTest(const std::string& category, const std::string& name) ;

			/**
			 * populates the specified list with all unit tests registered with this TestManager
			 *
			 * @param tests list to be populated with all unit tests
			 * @return tests parameter populated with all unit tests
			 */
			std::list<AbstractUnitTest*>& getAllTests(std::list<AbstractUnitTest*>& tests) ;

		private:
			/**
			 * Private constructor to avoid instances
			 *
			 */
			TestManager() ;

			/** singleton TestManager instance */
			static std::auto_ptr<TestManager> s_instance ;

			typedef std::map<std::string, AbstractUnitTest*> TestCategory ;
			typedef std::map<std::string, TestCategory> TestContainer ;

			/** contains all registered unit tests */
			TestContainer m_tests ;

	} ; /* class TestManager */

} /* namespace cutil */

#endif /* _CUTIL_TESTMANAGER_H_ */
