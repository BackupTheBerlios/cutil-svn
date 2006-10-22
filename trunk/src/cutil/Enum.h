/*
 * Copyright (C) 2004-2005  Colin Law
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

#ifndef _CUTIL_ENUM_H_
#define _CUTIL_ENUM_H_

#include <functional>
#include <set>
#include <string>

namespace cutil
{
	/**
	 * This class represents an Enumerated Type Constant Object.
	 * The aim is to provide a constant type object template that acts as a base
	 * for other constant types. This provides object based enum type system, which
	 * may be extended to include object specific methods.
	 *
	 * This started as a java technique that I have attempted to port to C++, with
	 * unsatifying results. This latest version is based upon an article in the
	 * May 2003 edition of the C/C++ Users Journal
	 * "When enum Just Isn't Enough: Enumeration Classes for C++" by Yves Meynard
	 * and Stroustrup "the C++ programming language, 3rd Edition" 13.6
	 *
	 * An Enum implementation is required to extend Enum, passing itself as the parameterized
	 * type to Enum, for example (NewEnumType.h);
	 *
	 * <pre>
	 *   class NewEnumType : public cutil::Enum<NewEnumType>
	 * </pre>
	 *
	 * Each Enum implementations must also perform a few steps in order to correctly setup the Enum Type.
	 * The static Enum container used to store each instance of the Enum type, and the static value
	 * must be instanced and initilized for each Enum Type created. For Example (NewEnumType.cc);
	 *
	 * <pre>
	 * template cutil::Enum<NewEnumType>::EnumInstanceList cutil::Enum<NewEnumType>::theEnumInstances ;
	 * template<> int cutil::Enum<NewEnumType>::theNextValue = 0 ;
	 * </pre>
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	template <class T>
	class Enum
	{
		private:
			//---------------------------------------------------------------------------------------//
			// Inner class & Structures

			/**
			 * Predicate for finding the Enum instance with a specific value
			 *
			 */
			class EnumPredicateEqualTo : public std::unary_function<const T*, bool>
			{
				public:
					explicit EnumPredicateEqualTo(int val) : theCompareValue(val) { }

					bool operator() (const T* toCompare)
					{
						return(toCompare->getValue() == theCompareValue) ;
					}

				private:
					const int theCompareValue ;
			} ;

			/**
			 * Comparison functor for the set of instances
			 * This functor returns true if the Enum instance enumA has a value less
			 * than enumB.
			 *
			 */
			class EnumPredicateLess : public std::binary_function<const T*, const T*, bool>
			{
				public:
					EnumPredicateLess() {}

					bool operator() (const T* enumA, const T* enumB)
					{
						return(enumA->getValue() < enumB->getValue()) ;
					}
			} ;

			//---------------------------------------------------------------------------------------//

		protected:
			/** typedef for Enum container */
			typedef std::set<const T*, EnumPredicateLess> EnumInstanceList ;

			//---------------------------------------------------------------------------------------//

		public:
			/** Enum container iterator typedef */
			typedef typename EnumInstanceList::const_iterator const_iterator ;


			//---------------------------------------------------------------------------------------//
			// Member data access

			/**
			 * Returns the ordinal value of thie Enum
			 *
			 * @return the ordinal valu of this Enum
			 */
			int getValue() const { return(theValue) ; }

			/**
			 * Returns the brief description of this Enum object
			 *
			 * @return the brief description of this Enum
			 */
			std::string getDescription() const { return(theDescription) ; }


			//---------------------------------------------------------------------------------------//
			// Enum Object Access

			/**
			 * Returns the Enum object with the specified ordinal value
			 * If this Enum does not contain an object with the specified ordinal
			 * value, 0 is returned
			 *
			 * @return the Enum Object with the specified ordinal value
			 */
			static const T* getByValue(int val)
			{
				const_iterator citer = find_if(theEnumInstances.begin(), theEnumInstances.end(), EnumPredicateEqualTo(val)) ;
				return((citer != theEnumInstances.end()) ? *citer : 0) ;
			}

			/**
			 * Returns whether this enum contains a object with the specified value
			 *
			 * @return true if this Enum contains an object with the specified value
			 *         false otherwise
			 */
			static bool hasValue(int val) { return(getByValue(val) != NULL) ; }

			/**
			 * Returns the number of Enum Objects within this Enum
			 *
			 * @return the number of Enum objects within this Enum
			 */
			static typename EnumInstanceList::size_type getSize() {return(theEnumInstances.size()) ; }

			/**
			 * Returns the first Object within this Enum
			 *
			 * @return the first Object within this enum
			 * @note [design] is this method goind to be required?
			 */
			static const T* getFirst() { return(*theEnumInstances.begin()) ; }

			/**
			 * Returns the last Object within this Enum
			 *
			 * @return the last Object within this Enum
			 * @note [design] is this method goind to be required?
			 */
			static const T* getLast() { return(*theEnumInstances.rbegin()) ; }

			/**
			 * Returns the first ordinal value of this Enum
			 *
			 * @return the first ordinal value of this enum
			 * @note [design] is this method goind to be required?
			 */
			static int getFirstValue() { return((*theEnumInstances.begin())->theValue) ; }

			/**
			 * Returns the last ordinal value of this Enum
			 *
			 * @return the last ordinal value of this Enum
			 * @note [design] is this method goind to be required?
			 */
			static int getLastValue() { return((*theEnumInstances.rbegin())->theValue) ; }

			/**
			 * Returns access to this Enum instances derived class methods
			 * As an Enum is constructued giving a template paramter as the deriving class,
			 * we can provide access to the devided class methods through this method, even
			 * if we maintain on a reference to the base Enum
			 *
			 * @return access to T's methods
			 */
			const T& derived() { return(static_cast<const T&>(*this)) ; }


			//---------------------------------------------------------------------------------------//
			// Iterators

			/**
			 * Returns a constant iterator from the beginning of the Enum
			 *
			 * @return a const iterator from the beginning of the Enum
			 */
			static const_iterator begin() { return(theEnumInstances.begin()) ; }

			/**
			 * Returns a const iterator at the end of the enum
			 * The special location end is a one past the end of the iterator
			 *
			 * @return a const iteraotr at the end of this Enum
			 */
			static const_iterator end() { return(theEnumInstances.end()) ; }



			//---------------------------------------------------------------------------------------//
			// Overloaded Operators

			/**
			 * Returns true if this Enum is equal to the specified Enum.
			 * Equal to in this case is true if the address of this Enum
			 * and the specified Enum are equal, and the value returned
			 * by getValue of this Enum and the specified Enum are equal.
			 *
			 * @param rhs the Enum to compare to this Enum for equality
			 */
			bool operator == (const T& rhs) const
			{
				if((this == &rhs) && (this->getValue() == rhs.getValue()))
				{
					return(true) ;
				}
				else
				{
					return(false) ;
				}
			}

			/**
			 * Returns true if this Enum is not equal to the specified Enum.
			 *
			 * @param rhs the Enum to compare to this Enum for equality
			 */
			bool operator != (const T& rhs) const
			{
				return(!(*this == rhs)) ;
			}



			//---------------------------------------------------------------------------------------//

		protected:
			/**
			 * Construct a new enum with the given description
			 *
			 * @param description a brief description of the enum value
			 */
			explicit Enum(const std::string& description) ;
			
			explicit Enum(const Enum<T>&) {} ;



			//---------------------------------------------------------------------------------------//

		private:

			/** the ordinal value of this Enum */
			int theValue ;

			/** a brief description of this Enum */
			std::string theDescription ;

			/**
			 * Tracks the next ordinal value of the next item we add to the Enum
			 * @note [explanation] this works because each instantiated template
			 *       for a given type has its own copy of static member data
			 *       so the ordinal value is specific to the type used within
			 *       the template (did that make sense?)
			 */
			static int theNextValue ;

			/** the created instances of the Enum for a given type */
			static EnumInstanceList theEnumInstances;

	} ;



	/** define the space for the EnumInstanceList definition */
	// @note [explanation] this is a gcc 3.4 fix. Currently im not convinced/understand either the
	//       syntax or the location. gcc 3.3 did not require this line, and Enum implementations must
	//       also include a line like the following;
	//       template cutil::Enum<EnumType>::EnumInstanceList cutil::Enum<EnumType>::theEnumInstances
	//       where EnumType is the Enum implementation, to instatiate the container
	//       previously gcc 3.3 would allow the single line;
	//       cutil::Enum<EnumType>::EnumInstanceList cutil::Enum<EnumType>::theEnumInstances ;
	//       ... This is likely to change once i get a handle on what is happening
	//
	template<class T> typename cutil::Enum<T>::EnumInstanceList cutil::Enum<T>::theEnumInstances ;



	/**
	 * Construct a new enum with the given description
	 *
	 * @param description a brief description of the enum value
	 */
	template <class T>
	inline Enum<T>::Enum(const std::string& description):
			theValue(Enum::theNextValue++), theDescription(description)
	{
		theEnumInstances.insert(static_cast<T*>(this)) ;
	}

} /* namespace cutil */

#endif /* _CUTIL_ENUM_H_ */
