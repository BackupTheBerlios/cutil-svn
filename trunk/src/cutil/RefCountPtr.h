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

#ifndef _CUTIL_REFCOUNTPTR_H_
#define _CUTIL_REFCOUNTPTR_H_

namespace cutil
{
	/**
	 * RefCountPtr is a reference counting smart pointer.
	 * Instance of RefCountPtr are created to manage the lifetime of a dynamically allocated object.
	 * Once the last instance of a RefCountPtr for a particular object is release, the managed
	 * object will be deleted.
	 *
	 * RefCountPtr implements a valid copy constructor and assignment operator and is therefore suitable for
	 * use within STL containers
	 *
	 * A RefCountPtr should be created in the following manner to avoid accidental references to the plain pointer
	 *     RefCountPtr<T> ptr(new T) ;
	 *
	 * @note [design] it is possible to abuse a RefCountPtr by creating a new instance
	 *       using an already managed object. We therefore have two RefCountPtrs with
	 *       different use counts although they are considered equal (by pointing to the same
	 *       managed object). This is simply considered an abuse.
	 * 
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	template <class T>
	class RefCountPtr
	{
		public:
			/**
			 * Construct a RefCountPtr which initially points to 0 and has a refence count of 0
			 *
			 */
			RefCountPtr() : thePtr(0), theRefCount(0) { checkType() ; }

			/**
			 * Constructs a new RefCountptr to manage the object pointed at by pointer.
			 * This RefCountPtr will have an intial reference count of 1
			 *
			 * @param pointer a pointer to the object this RefCountptr is to manage
			 */
			RefCountPtr(T* pointer) : thePtr(pointer), theRefCount(new unsigned int(1)) { checkType() ; }

			/**
			 * Constructs a new RefCountPtr by copying the specified RefCountPtr
			 * This RefCountPtr will point to the obejct manage by copy and the reference
			 * count will be incremented by one
			 *
			 * @param copy the RefCounPtr to copy
			 */
			RefCountPtr(const RefCountPtr<T>& copy) ;

			/**
			 * Destructor
			 */
			~RefCountPtr() ;

			//---------------------------------------------------------------------------------------//
			// Accessors / Mutators

			/**
			 * Returns trus if this RefCountPtr points to a vild object, ie not 0.
			 *
			 * @return true if this RefCountPtr points to a vlid object, false otherwise
			 */
			bool hasPtr() const ;

			/**
			 * Returns a pointer to the managed object
			 *
			 * @return a pointer to the managed object
			 */
			T* getPtr() const ;

			/**
			 * Returns the number of reference to the managed object
			 * If this RefCountPtr does not point to a valid object, 0 is returned
			 *
			 * @return the number of references to the managed object
			 */
			unsigned int getRefCount() const ;

			/**
			 * Unreferences this RefCountPtr from the managed object.
			 * This RefCountPtr will no longer point to a valid object and this
			 * RefCountPtr will have a reference count of zero.
			 *
			 * If this RefCountPtr was the last remaining reference to the managed object,
			 * the managed object is deleted, otherwise the refrence count is decremented by
			 * one
			 *
			 */
			void clear() ;

			/**
			 * Binds this RefCountPtr to object pointer at by pointer.
			 * This RefCountPointer assumes management of the specified object.
			 *
			 * If this RefCountPtr previously held a reference to a managed object, the
			 * reference count is first cleared.
			 *
			 * If this RefCountPtr was the last remaining reference to the managed object,
			 * the managed object is deleted, otherwise the refrence count is decremented by
			 * one
			 *
			 * @param pointer a pointer to the object to manage
			 */
			void bind(T* pointer) ;

			//---------------------------------------------------------------------------------------//
			// operator overloading

			/**
			 * Assignment operator assigns rhs to this RefCountPtr
			 *
			 * If this RefCountPtr previously held a reference to a managed object, the
			 * reference count is first cleared.
			 *
			 * If this RefCountPtr was the last remaining reference to the managed object,
			 * the managed object is deleted, otherwise the refrence count is decremented by
			 * one
			 *
			 * @param rhs the RefCountPtr to assign to this
			 */
			RefCountPtr& operator=(const RefCountPtr<T>& rhs) ;

			/**
			 * Returns a pointer to the managed object
			 *
			 * @return a pointer to the managed object
			 */ 
			T* operator->() const ;

			/**
			 * Compares this and the specified RefCountPtr for equality.
			 * Two RefCountPtrs are equal if they both point to the same managed object.
			 *
			 * @param rhs the RefCountPtr to compare to this for equlity
			 */
			bool operator==(const RefCountPtr<T>& rhs) const ;

			/**
			 * Compares this and the specified RefCountPtr for inequality.
			 * Two RefCountPtrs are not equal if they point to different managed objects.
			 *
			 * @param rhs the RefCountPtr to compare to this for inequlity
			 */
			bool operator!=(const RefCountPtr<T>& rhs) const ;



		protected:

			//---------------------------------------------------------------------------------------//

		private:
			/**
			 * Decrements the reference count of the managed object by one
			 *
			 * If the new reference count reaches zero the managed object is deleted.
			 */
			void decrementRefCount() ;

			/**
			 * Increments the reference count of the managed object by one
			 *
			 */
			void incrementRefCount() ;

			/**
			 * Complete type paramter test
			 * This method checks that we can get the sizeof the parameterized type, and will
			 * fail compilation if we cannot. This is vital for a the RefCountPtr to correctly
			 * delete the type.
			 *
			 * It is possible to construct a RefCountPtr for an incomplete type, and therefore
			 * when the last reference to the object of the incomplete type is released, the
			 * RefCountPtr will attempt to delete the object. This is allowed according to the
			 * C++ standard, however is certainly isn't correct.
			 * GCC (verified on gcc3.4 only) will warn about deleting an incomplete type if
			 * warnings are enabled. however it is possible ignore warnings, or worse, disable
			 * so this would ordinarily go unnoticed.
			 *
			 * So, to avoid creating RefCountPtr for incomplete types we force each constructor to
			 * call this method, If the type is incomplete, sizeof() will fail and therefore
			 * compilation will also fail.
			 *
			 */
			void checkType() { int dummy = 0 ; dummy = sizeof(T) ; } 

			/** pointer to the object we are managing */
			T* thePtr ;

			/** reference count to the managed pointer */
			unsigned int* theRefCount ;


			//---------------------------------------------------------------------------------------//
	} ;


	//---------------------------------------------------------------------------------------//



	//---------------------------------------------------------------------------------------//
	// Implementation


	template <class T>
	RefCountPtr<T>::RefCountPtr(const RefCountPtr<T>& copy)
	{
		this->thePtr = copy.thePtr ;
		this->theRefCount = copy.theRefCount ;

		incrementRefCount() ;
	}

	template <class T>
	RefCountPtr<T>::~RefCountPtr()
	{
		decrementRefCount() ;
	}

	//---------------------------------------------------------------------------------------//
	// Accessors / Mutators

	template <class T>
	bool RefCountPtr<T>::hasPtr() const
	{
		return(thePtr != 0) ;
	}

	template <class T>
	T* RefCountPtr<T>::getPtr() const
	{
		return(thePtr) ;
	}

	template <class T>
	unsigned int RefCountPtr<T>::getRefCount() const
	{
		if(theRefCount)
		{
			return(*theRefCount) ;
		}
		else
		{
			return(0) ;
		}
	}

	template <class T>
	void RefCountPtr<T>::clear()
	{
		decrementRefCount() ;

		this->thePtr = 0 ;
		this->theRefCount = 0 ;
	}

	template <class T>
	void RefCountPtr<T>::bind(T* pointer)
	{
		if(pointer != this->thePtr)
		{
			decrementRefCount() ;

			this->thePtr = pointer ;
			theRefCount = new unsigned int(1) ;
		}
	}

	//---------------------------------------------------------------------------------------//
	// operator overloading

	template <class T>
	RefCountPtr<T>& RefCountPtr<T>::operator=(const RefCountPtr<T>& rhs)
	{
		if(this->thePtr == rhs.thePtr)
		{
			return(*this) ;
		}
		else
		{
			decrementRefCount() ;

			thePtr = rhs.thePtr ;
			theRefCount = rhs.theRefCount ;

			incrementRefCount() ;

			return(*this) ;
		}
	}

	template <class T>
	T* RefCountPtr<T>::operator->() const
	{
		return(thePtr) ;
	}

	template <class T>
	bool RefCountPtr<T>::operator==(const RefCountPtr<T>& rhs) const
	{
		return(this->thePtr == rhs.thePtr) ;
	}

	template <class T>
	bool RefCountPtr<T>::operator!=(const RefCountPtr<T>& rhs) const
	{
		return(this->thePtr != rhs.thePtr) ;
	}

	//---------------------------------------------------------------------------------------//


	template <class T>
	void RefCountPtr<T>::decrementRefCount()
	{
		if(theRefCount)
		{
			if(--(*theRefCount) == 0)
			{
				delete thePtr ;
				delete theRefCount ;
			}
		}
	}

	template <class T>
	void RefCountPtr<T>::incrementRefCount()
	{
		if(theRefCount)
		{
			(*theRefCount)++ ;
		}
	}


} /* namespace cutil */

#endif /* _CUTIL_REFCOUNTPTR_H_ */
