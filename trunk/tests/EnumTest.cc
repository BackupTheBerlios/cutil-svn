#include <cutil/Enum.h>

#include <iostream>
#include <string>

using cutil::Enum ;

class Foo : public cutil::Enum<Foo>
{
	public:
		static const Foo TEST1 ;
		static const Foo TEST2 ;

		~Foo() {} ;
	private:
		Foo(std::string description) : cutil::Enum<Foo>(description) {} ;
} ;

template cutil::Enum<Foo>::EnumInstanceList cutil::Enum<Foo>::theEnumInstances ;
template<> int cutil::Enum<Foo>::theNextValue = 0 ;

const Foo Foo::TEST1("Test Enum 1") ;
const Foo Foo::TEST2("Test Enum 2") ;

int main()
{
	// Construction
	std::cout << "### Construction ###" << std::endl ;

	std::cout << Foo::TEST1.getDescription() << std::endl ;
	
	for(Foo::const_iterator citer = Foo::begin(); citer != Foo::end(); ++citer)
	{
		const Foo* foo = *citer ;
		std::cout << foo->getDescription() << std::endl ;
	}
	
	std::cout << "--------------" << std::endl ;
	std::cout << Foo::getFirst()->getDescription() << std::endl ;
	std::cout << Foo::getLast()->getDescription() << std::endl ;
	
	std::cout << "--------------" << std::endl ;
	std::cout << Foo::getFirst()->getValue() << std::endl ;
	std::cout << Foo::getLast()->getValue() << std::endl ;
	
	std::cout << "--------------" << std::endl ;
	std::cout << Foo::getByValue(0)->getDescription() << std::endl ;

	return(0) ;
}
