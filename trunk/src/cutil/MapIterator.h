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

#ifndef _CUTIL_MAP_ITERATOR_H_
#define _CUTIL_MAP_ITERATOR_H_

#include <iterator>

namespace cutil
{
	// pre-declare each the template friend function before the template class definition
	// to inform the compiler that the functions are themselves templates
	template<class T_Cont, class T_Iter> class MapIterator;
	template<class T_Cont, class T_Iter> bool operator== (const MapIterator<T_Cont, T_Iter>& lhs, const MapIterator<T_Cont, T_Iter>& rhs);
	template<class T_Cont, class T_Iter> bool operator!= (const MapIterator<T_Cont, T_Iter>& lhs, const MapIterator<T_Cont, T_Iter>& rhs);

	/**
	 * MapIterator provides a convience class to iterator over a map container
	 * Currently two sub-classes exist, MapValueIterator and MapKeyIterator to
	 * iterator over the values and keys within a map.
	 *
	 * A typical use of such a class is to provide a public const_iterator over either the
	 * values or keys held internally within a std::map, therefore keeping the exact nature of
	 * the private container, private.
	 *
	 * The T_Cont template parameter is the container, this should be a std::map - see todo
	 * The T_Iter template parameter is the iterator type over the container type.
	 *
	 * @todo avoid passing the map as the template paramter somehow.
	 */
	template<class T_Cont, class T_Iter = typename T_Cont::iterator>
	class MapIterator : public std::iterator_traits<T_Iter>
	{
		public:
			/**
			 * Returns true if both the iterators specified point to the same location over the same container
			 *
			 * @param lhs the first iterator
			 * @param rhs the second iterator
			 * @return true if both iterators point to the same location and container
			 *         false otherwise
			 */
			friend bool operator== <> (const MapIterator<T_Cont, T_Iter>& lhs, const MapIterator<T_Cont, T_Iter>& rhs);
			
			/**
			 * Returns true if the iterators specified do NOT point to the same location within the same container.
			 * This will always return false if the iterators are from different containers.
			 *
			 * @param lhs the first iterator
			 * @param rhs the second iterato
			 * @return true if both iterators specified do not point to the same location within the same container,
			 *         false if they do
			 */
			friend bool operator!= <> (const MapIterator<T_Cont, T_Iter>& lhs, const MapIterator<T_Cont, T_Iter>& rhs);

			/**
			 * Increment this iterator to point a the next location
			 *
			 * @return an iterator pointing the the next location
			 */
			MapIterator& operator++();

		protected:
			/**
			 * Protected Constructor - disallow instances
			 *
			 * @param cont the container to iterator over
			 * @param iter an iterator over the specified container
			 */
			MapIterator(T_Cont& cont, T_Iter iter);

			/**
			 * Returns the container this Iterator is iterating over
			 *
			 * @return the container this iterator is iteraoting over
			 */
			T_Cont* GetCont() const ;

			/**
			 * Returns a reference to the private iterator this class is acting for
			 *
			 * @return reference to the std iterator
			 */
			const T_Iter& GetIter() const ;


		private:
			/** the container we are iteraing over */
			T_Cont* theContainer;

			/** private iterator */
			T_Iter theCurrIter;

	};


	//---------------------------------------------------------------------------------------//


	/**
	 * MapKeyIterator provides an iteraotr over the keys within a map
	 * This class may be used to iterate over the keys contained within a map,
	 * hiding from a user the values associated with the key. This avoids exposing
	 * the key value pair of the map to the user.
	 *
	 */
	template<class T_Cont, class T_Iter = typename T_Cont::iteratorr>
	class MapKeyIterator : public MapIterator<T_Cont, T_Iter>
	{

		public:
			/**
			 * Constructs a new MapKeyIterator to iterate over the keys contained within the container cont.
			 *
			 * @param cont the container to iterator over
			 * @param iter a standard iterator over the specified container
			 */
			MapKeyIterator(T_Cont& cont, T_Iter iter) ;

			/**
			 * Returns the key into the container at the position of this iterator
			 *
			 * @return the key into the container at the position of this iterator
			 */
			typename T_Cont::key_type operator* () const ;
	};


	//---------------------------------------------------------------------------------------//


	/**
	 * MapValueIterator provides an iteraotr over the values within a map
	 * This class may be used to iterate over the values contained within a map,
	 * hiding from a user the keys into that map. This avoids exposing the key value
	 * pair of the map to the user.
	 *
	 */
	template<class T_Cont, class T_Iter = typename T_Cont::iterator>
	class MapValueIterator : public MapIterator<T_Cont, T_Iter>
	{

		public:
			/**
			 * Constructs a new MapValueIterator to iterate over the values contained within the container cont.
			 *
			 * @param cont the container to iterator over
			 * @param iter a standard iterator over the specified container
			 */
			MapValueIterator(T_Cont& cont, T_Iter iter) ;

			/**
			 * Returns the value contained within the container at the position of this iterator
			 *
			 * @return the value contained at the position of this iterator
			 */
			typename T_Cont::mapped_type operator* () const;
	};






	//---------------------------------------------------------------------------------------//



	//---------------------------------------------------------------------------------------//
	// Implementation

	// MapIterator

	template<class T_Cont, class T_Iter>
	bool operator== (const MapIterator<T_Cont, T_Iter>& lhs, const MapIterator<T_Cont, T_Iter>& rhs)
	{
		return(lhs.theContainer == rhs.theContainer && lhs.theCurrIter == rhs.theCurrIter);
	}

	template<class T_Cont, class T_Iter>
	bool operator!= (const MapIterator<T_Cont, T_Iter>& lhs, const MapIterator<T_Cont, T_Iter>& rhs)
	{
		return(lhs.theContainer != rhs.theContainer || lhs.theCurrIter != rhs.theCurrIter);
	}

	template<class T_Cont, class T_Iter>
	MapIterator<T_Cont, T_Iter>& MapIterator<T_Cont, T_Iter>::operator++()
	{
		++theCurrIter;
		return(*this);
	}

	template<class T_Cont, class T_Iter>
	MapIterator<T_Cont, T_Iter>::MapIterator(T_Cont& cont, T_Iter iter) : theContainer(&cont), theCurrIter(iter)
	{}

	template<class T_Cont, class T_Iter>
	const T_Iter& MapIterator<T_Cont, T_Iter>::GetIter() const
	{
		return(theCurrIter);
	}

	template<class T_Cont, class T_Iter>
	T_Cont* MapIterator<T_Cont, T_Iter>::GetCont() const
	{
		return(theContainer);
	}



	//---------------------------------------------------------------------------------------//
	// MapKeyIterator

	template<class T_Cont, class T_Iter>
	MapKeyIterator<T_Cont, T_Iter>::MapKeyIterator(T_Cont& cont, T_Iter iter) : MapIterator<T_Cont, T_Iter>(cont, iter)
	{}

	template<class T_Cont, class T_Iter>
	typename T_Cont::key_type MapKeyIterator<T_Cont, T_Iter>::operator* () const
	{
		// we need to make acess to the base class dependant here using this->GetIter()
		return((*(this->GetIter())).first);
	}



	//---------------------------------------------------------------------------------------//
	// MapValueIterator

	template<class T_Cont, class T_Iter>
	MapValueIterator<T_Cont, T_Iter>::MapValueIterator(T_Cont& cont, T_Iter iter) : MapIterator<T_Cont, T_Iter>(cont, iter)
	{}

	template<class T_Cont, class T_Iter>
	typename T_Cont::mapped_type MapValueIterator<T_Cont, T_Iter>::operator* () const
	{
		return((*(MapValueIterator<T_Cont, T_Iter>::GetIter())).second);
	}

} /* namespace cutil */

#endif /* _CUTIL_MAP_ITERATOR_H_ */
