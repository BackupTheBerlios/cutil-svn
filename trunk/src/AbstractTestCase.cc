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

#include <cutil/AbstractTestCase.h>

#include <cutil/Closure.h>

#include <string>

using cutil::AbstractTestCase ;
using cutil::AbstractClosure ;
using cutil::RefCountPtr ;

AbstractTestCase::~AbstractTestCase()
{
	
}

AbstractTestCase::AbstractTestCase(RefCountPtr<const AbstractClosure<void> > test_step, std::string step_name, std::string pass_msg, std::string fail_msg)
	: m_test_step(test_step), m_step_name(step_name), m_pass_msg(pass_msg), m_fail_msg(fail_msg)
{
	
}

AbstractTestCase::AbstractTestCase(RefCountPtr<const AbstractTestCase> test_case)
	: m_wrapped_test_case(test_case)
{
	
}

RefCountPtr<const AbstractClosure<void> >
AbstractTestCase::getTestStep() const
{
	return(m_test_step) ;
}

RefCountPtr<const AbstractTestCase>
AbstractTestCase::getWrappedTestCase() const
{
	return(m_wrapped_test_case) ;
}

bool
AbstractTestCase::isNested() const
{
	return(m_wrapped_test_case.hasPtr()) ;
}

const std::string&
AbstractTestCase::getStepName() const
{
	return(m_step_name) ;
}

const std::string&
AbstractTestCase::getPassMessage() const 
{
	return(m_pass_msg) ;
}

const std::string&
AbstractTestCase::getFailureMessage() const
{
	return(m_fail_msg) ;
}
