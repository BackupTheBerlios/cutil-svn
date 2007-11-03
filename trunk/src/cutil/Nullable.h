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
 * $Id$
 */

#ifndef _CUTIL_NULLABLE_H_
#define _CUTIL_NULLABLE_H_

namespace cutil
{
	/**
	 * Nullable provides a template class wrapping a value type which may optionally be set 
	 * to null.
	 *
	 * For example, Nullable<bool> may represent three values, true, false and null.
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	template <class T>
	class Nullable
	{
		public:
			/**
			 * Constructs a new instance of a Nullable<T> which intially has no value.
			 *
			 */
			Nullable() ;

			/**
			 * Constructs a new instance of a Nullable<T> and assigns it the specified value
			 *
			 * @param vaule the value to be assigned to the new instance.
			 */
			Nullable(T value) ;

			//---------------------------------------------------------------------------------------//
			// Accessors / Mutators

			/**
			 * Returns true if this Nullable holds a non null value.
			 *
			 * @return true if this Nullable holds a non null value.
			 */
			bool hasValue() const ;

			/**
			 * Returns the value held by this Nullable<T>. It is invalid to access the value of a null
			 * Nullable<T>, i.e. if hasValue returns false, and will result in a cutil::Exception
			 * being thrown.
			 *
			 * @return the value held by this Nullable<T>
			 * @throws Exception if hasValue returns false.
			 */
			T getValue() const throw(Exception) ;

			/**
			 * Clears this Nullable<T>, setting its value to null.
			 *
			 */
			void clear() ;

			/**
			 * Sets the value held by this Nullable<T> to the specified value.
			 *
			 * @param value new value to be held by this Nullable<T>.
			 */
			void setValue(T value) ;

			//---------------------------------------------------------------------------------------//
			// operator overloading

			/**
			 * Assignment operator assigns the specified value to this Nullable<T>.
			 *
			 * @param value value to be assigned to this Nullable<T>
			 */
			Nullable<T>& operator=(T value) ;

		protected:

		private:
			/** the value held by this Nullable<T> */
			T m_value ;
			bool m_is_null ;
	} ;


	//---------------------------------------------------------------------------------------//



	//---------------------------------------------------------------------------------------//
	// Implementation
	
	template <class T>
	Nullable<T>::Nullable()
	{
		this->m_is_null = true ;
	}

	template <class T>
	Nullable<T>::Nullable(T value)
	{
		this->m_value = value ;
		this->m_is_null = false ;
	}

	//---------------------------------------------------------------------------------------//
	// Accessors / Mutators

	template <class T>
	bool Nullable<T>::hasValue() const
	{
		return !this->m_is_null ;
	}

	template <class T>
	T Nullable<T>::getValue() const throw(Exception)
	{
		if(this->m_is_null)
		{
			throw Exception("Nullable has null value");
		}

		return this->m_value ;
	}

	template <class T>
	void Nullable<T>::clear()
	{
		this->m_is_null = true ;
	}

	template <class T>
	void Nullable<T>::setValue(T value)
	{
		this->m_value = value ;
		this->m_is_null = false ;
	}

	//---------------------------------------------------------------------------------------//
	// operator overloading

	template <class T>
	Nullable<T>& Nullable<T>::operator=(T value)
	{
		setValue(value) ;

		return(*this) ;
	}


} /* namespace cutil */

#endif /* _CUTIL_NULLABLE_H_ */
