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

#ifndef _CUTIL_CLOSURE_H_
#define _CUTIL_CLOSURE_H_

namespace cutil
{
	/**
	 * Abstract Base class for a Closure Function Object.
	 * A closure object is a function object that takes a object and pointer to a member function
	 * to call upon the specified object. It also takes any required paramaters to the member function.
	 * The Closure may then be called with the predefined parameter values at a later stage.
	 *
	 * the T_ret template parameter specified the return type of the operator()
	 *
	 * The AbstractClosure base exists to enable containers of pointers to closure objects
	 * to be maintained (assuming the same return type), This allowd the correct version of
	 * operator() to be called at run time as required. 
	 *
	 * @author Colin Law [claw@mail.berlios.de]
	 * @version $Rev$
	 */
	template <class T_ret>
	class AbstractClosure
	{
		public:
			/**
			 * Destructor
			 */
			virtual ~AbstractClosure() {} ;

			/**
			 * Calls this Closure.
			 */
			virtual T_ret operator() () const = 0 ;

		protected:
			/**
			 * Disallow instances of this Abstract class
			 */
			AbstractClosure() {} ;

		private:
	};


	/**
	 * Closure object providing a call to a member frunction of a specified object.
	 * This closure takes a pointer to an instance of an object, and a member function
	 * of that object. When this Closure operaor() is called, the specified member function
	 * is called upon the specified instnace of the object, returning type template type
	 * parameter T_ret
	 *
	 * The template parameter T_ret specifies the return type of the member function to call.
	 * The template parameter T_obj specifies the Object type on which the member function is to be called
	 */
	template <class T_ret, class T_obj>
	class Closure0 : public AbstractClosure<T_ret>
	{
		public:
			/**
			 * Constructs a new Closure0 to call the specified member function of the specified object
			 *
			 * @param obj the object on which to call the member function
			 * @param memberFunction the member function to call upon obj
			 */
			Closure0(T_obj* obj, T_ret (T_obj::*memberFunction)()) ;

			/**
			 * Calls this Closure.
			 * This operator invokes the member function specified within the constructor on the specified
			 * object
			 */
			virtual T_ret operator() () const ;

		protected:

		private:
			/** object to call member function on */
			T_obj* theObject ;

			/** member function */
			T_ret (T_obj::*theMemberFunction)() ;
	};

	/**
	 * Closure object providing a call to a member frunction of a specified object.
	 * This closure takes a pointer to an instance of an object, a member function
	 * of that object, ana single parameter to the member function. When this Closures
	 * operaor() is called, the specified member function is called upon the specified
	 * instnace of the object, passing the specified parameter argument, returning the
	 * template type parameter T_ret
	 *
	 * The template parameter T_ret specifies the return type of the member function to call.
	 * The template parameter T_obj specifies the Object type on which the member function is to be called
	 * The template parameter T_arg1 specifies the parameter argument type to the member function.
	 */
	template <class T_ret, class T_obj, class T_arg1>
	class Closure1 : public AbstractClosure<T_ret>
	{
		public:
			/**
			 * Constructs a new Closure1 to call the specified member function of the specified object
			 * The specified arg0 is passed as a parameter to the member function
			 *
			 * @param obj the object on which to call the member function
			 * @param arg0 parameter to the member function
			 * @param memberFunction the member function to call upon obj
			 */
			Closure1(T_obj* obj, T_arg1 arg1, T_ret (T_obj::*memberFunction)(T_arg1)) ;

			/**
			 * Calls this Closure.
			 * This operator invokes the member function specified within the constructor on the specified
			 * object, passing the single argument value as a parameter to the member function
			 */
			virtual T_ret operator() () const ;

		protected:

		private:
			/** object to call member function on */
			T_obj* theObject ;

			/** data to pass to the member function */
			T_arg1 theArg1 ;

			/** member function */
			T_ret (T_obj::*theMemberFunction)(T_arg1) ;
	} ;

	/**
	 * Closure object providing a call to a member frunction of a specified object.
	 * This closure takes a pointer to an instance of an object, a member function
	 * of that object, and two parameters to the member function. When this Closures
	 * operaor() is called, the specified member function is called upon the specified
	 * instnace of the object, passing the specified parameter arguments, returning the
	 * template type parameter T_ret
	 *
	 * The template parameter T_ret specifies the return type of the member function to call.
	 * The template parameter T_obj specifies the Object type on which the member function is to be called
	 * The template parameter T_arg1 specifies the first parameter argument type to the member function.
	 * The template parameter T_arg1 specifies the second parameter argument type to the member function.
	 */
	template <class T_ret, class T_obj, class T_arg1, class T_arg2>
	class Closure2 : public AbstractClosure<T_ret>
	{
		public:
			/**
			 * Constructs a new Closure1 to call the specified member function of the specified object
			 * The specified arg0 is passed as a parameter to the member function
			 *
			 * @param obj the object on which to call the member function
			 * @param arg1 parameter to the member function
			 * @param arg2 parameter to the member function
			 * @param memberFunction the member function to call upon obj
			 */
			Closure2(T_obj* obj, T_arg1 arg1, T_arg2 arg2, T_ret (T_obj::*memberFunction)(T_arg1, T_arg2)) ;

			/**
			 * Calls this Closure.
			 * This operator invokes the member function specified within the constructor on the specified
			 * object, passing the single argument value as a parameter to the member function
			 */
			virtual T_ret operator() () const ;

		protected:

		private:
			/** object to call member function on */
			T_obj* theObject ;

			/** first argument to the member function */
			T_arg1 theArg1 ;

			/** second argument to the member function */
			T_arg2 theArg2 ;

			/** member function */
			T_ret (T_obj::*theMemberFunction)(T_arg1, T_arg2) ;
	} ;

	/**
	 * Closure object providing a call to a member frunction of a specified object.
	 * This closure takes a pointer to an instance of an object, a member function
	 * of that object, and three parameters to the member function. When this Closures
	 * operaor() is called, the specified member function is called upon the specified
	 * instnace of the object, passing the specified parameter arguments, returning the
	 * template type parameter T_ret
	 *
	 * The template parameter T_ret specifies the return type of the member function to call.
	 * The template parameter T_obj specifies the Object type on which the member function is to be called
	 * The template parameter T_arg1 specifies the first parameter argument type to the member function.
	 * The template parameter T_arg2 specifies the second parameter argument type to the member function.
	 * The template parameter T_arg3 specifies the third parameter argument type to the member function.
	 */
	template <class T_ret, class T_obj, class T_arg1, class T_arg2, class T_arg3>
	class Closure3 : public AbstractClosure<T_ret>
	{
		public:
			/**
			 * Constructs a new Closure1 to call the specified member function of the specified object
			 * The specified arg0 is passed as a parameter to the member function
			 *
			 * @param obj the object on which to call the member function
			 * @param arg1 parameter to the member function
			 * @param arg2 parameter to the member function
			 * @param arg3 parameter to the member function
			 * @param memberFunction the member function to call upon obj
			 */
			Closure3(T_obj* obj, T_arg1 arg1, T_arg2 arg2 , T_arg3 arg3, T_ret (T_obj::*memberFunction)(T_arg1, T_arg2, T_arg3)) ;

			/**
			 * Calls this Closure.
			 * This operator invokes the member function specified within the constructor on the specified
			 * object, passing the single argument value as a parameter to the member function
			 */
			virtual T_ret operator() () const ;

		protected:

		private:
			/** object to call member function on */
			T_obj* theObject ;

			/** first argument to the member function */
			T_arg1 theArg1 ;

			/** second argument to the member function */
			T_arg2 theArg2 ;

			/** third argument to the member function */
			T_arg3 theArg3 ;

			/** member function */
			T_ret (T_obj::*theMemberFunction)(T_arg1, T_arg2, T_arg3) ;
	} ;


	//---------------------------------------------------------------------------------------//



	//---------------------------------------------------------------------------------------//
	// Implementation

	// Closure0

	template <class T_ret, class T_obj>
	Closure0<T_ret, T_obj>::Closure0(T_obj* obj, T_ret (T_obj::*memberFunction)())
	{
		theObject = obj ;
		theMemberFunction = memberFunction ;
	}

	template <class T_ret, class T_obj>
	T_ret Closure0<T_ret, T_obj>::operator() () const 
	{
		return((*theObject.*theMemberFunction)()) ;
	}




	// Closure1

	template <class T_ret, class T_obj, class T_arg1>
	Closure1<T_ret, T_obj, T_arg1>::Closure1(T_obj* obj, T_arg1 arg1, T_ret (T_obj::*memberFunction)(T_arg1))
	{
		theObject = obj ;
		theArg1 = arg1 ;
		theMemberFunction = memberFunction ;
	}

	template <class T_ret, class T_obj, class T_arg1>
	T_ret Closure1<T_ret, T_obj, T_arg1>::operator() () const 
	{
		return((*theObject.*theMemberFunction)(theArg1)) ;
	}



	// Closure2

	template <class T_ret, class T_obj, class T_arg1, class T_arg2>
	Closure2<T_ret, T_obj, T_arg1, T_arg2>::Closure2(T_obj* obj, T_arg1 arg1, T_arg2 arg2, T_ret (T_obj::*memberFunction)(T_arg1, T_arg2))
	{
		theObject = obj ;
		theArg1 = arg1 ;
		theArg2 = arg2 ;
		theMemberFunction = memberFunction ;
	}

	template <class T_ret, class T_obj, class T_arg1, class T_arg2>
	T_ret Closure2<T_ret, T_obj, T_arg1, T_arg2>::operator() () const 
	{
		return((*theObject.*theMemberFunction)(theArg1, theArg2)) ;
	}



	// Closure3

	template <class T_ret, class T_obj, class T_arg1, class T_arg2, class T_arg3>
	Closure3<T_ret, T_obj, T_arg1, T_arg2, T_arg3>::Closure3(T_obj* obj, T_arg1 arg1, T_arg2 arg2, T_arg3 arg3, T_ret (T_obj::*memberFunction)(T_arg1, T_arg2, T_arg3))
	{
		theObject = obj ;
		theArg1 = arg1 ;
		theArg2 = arg2 ;
		theArg3 = arg3 ;
		theMemberFunction = memberFunction ;
	}

	template <class T_ret, class T_obj, class T_arg1, class T_arg2, class T_arg3>
	T_ret Closure3<T_ret, T_obj, T_arg1, T_arg2, T_arg3>::operator() () const 
	{
		return((*theObject.*theMemberFunction)(theArg1, theArg2, theArg3)) ;
	}

} /* namespace cutil */

#endif /* _CUTIL_CLOSURE_H_ */
