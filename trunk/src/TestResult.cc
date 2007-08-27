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

#include <cutil/TestResult.h>

using cutil::TestResult ;

TestResult::TestResult(const std::string& name, TestStatus status, std::string msg)
		: m_name(name), m_status(status), m_msg(msg)
{
}

const std::string&
TestResult::getName() const
{
	return(m_name) ;
}

TestResult::TestStatus
TestResult::getStatus() const
{
	return(m_status) ;
}

const std::string&
TestResult::getMessage() const
{
	return(m_msg) ;
}
