#include <cutil/RefCountPtr.h>
#include <iostream>
#include <list>
#include <string>
#include <sstream>

using cutil::RefCountPtr ;

// dummy class to output when it is created and destroyed
class foo
{
	public:
		foo() { std::cout << "[foo] constructor " << i << std::endl ; m_i = i++ ;} ;
		~foo() {  std::cout << "[foo] deconstructor" << m_i << std::endl ; } ;
		std::string bar() { std::ostringstream s ; s << "[foo] bar " << m_i ; return(s.str()) ; } ;
	private:
		int m_i ;
		static int i ;
} ;
int foo::i = 0 ;


int main()
{
	// Construction
	std::cout << "### Construction ###" << std::endl ;
	RefCountPtr<foo> ptr_a(new foo()) ;
	std::cout << ptr_a->bar() << std::endl ;

	// reference counting
	std::cout << "### Reference counting ###" << std::endl ;
	std::cout << "  ptr_a (" << ptr_a->bar() << ") count " << ptr_a.getRefCount() << std::endl ;

	// copy constructor
	RefCountPtr<foo> ptr_b(ptr_a) ;
	std::cout << "  ptr_b (" << ptr_b->bar() << ") count " << ptr_b.getRefCount() << std::endl ;

	// assignment
	RefCountPtr<foo> ptr_c ;
	ptr_c = ptr_a ;
	std::cout << "  ptr_c (" << ptr_c->bar() << ") count " << ptr_c.getRefCount() << std::endl ;



	// clear
	std::cout << "\n### Clear reference ###" << std::endl ;
	ptr_b.clear() ;
	if(ptr_a.hasPtr())
	{
		std::cout << "  ptr_a (" << ptr_a->bar() << ") count " << ptr_a.getRefCount() << std::endl ;
	}
	else
	{
		std::cout << "  ptr_a has no pointer" << std::endl ;
	}

	if(ptr_b.hasPtr())
	{
		std::cout << "  ptr_b (" << ptr_b->bar() << ") count " << ptr_b.getRefCount() << std::endl ;
	}
	else
	{
		std::cout << "  ptr_b has no pointer" << std::endl ;
	}

	if(ptr_c.hasPtr())
	{
		std::cout << "  ptr_c (" << ptr_c->bar() << ") count " << ptr_c.getRefCount() << std::endl ;
	}
	else
	{
		std::cout << "  ptr_c has no pointer" << std::endl ;
	}




	// bind
	std::cout << "\n### bind reference ###" << std::endl ;
	foo* fb = new foo() ;
	ptr_b.bind(fb) ;
	std::cout << "  ptr_a (" << ptr_a->bar() << ") count " << ptr_a.getRefCount() << std::endl ;
	std::cout << "  ptr_b (" << ptr_b->bar() << ") count " << ptr_b.getRefCount() << std::endl ;
	std::cout << "  ptr_c (" << ptr_c->bar() << ") count " << ptr_c.getRefCount() << std::endl ;



	// equality
	std::cout << "\n### equality ###" << std::endl ;
	std::cout << "  ptr_a == ptr_b : " << (ptr_a == ptr_b) << std::endl ;
	std::cout << "  ptr_a == ptr_c : " << (ptr_a == ptr_c) << std::endl ;
	std::cout << "  ptr_b == ptr_c : " << (ptr_b == ptr_c) << std::endl ;


	// falling out of scope
	std::cout << "\n### scoping ###" << std::endl ;
	for(int i = 0; i < 5; i++)
	{
		// create a new RefCountPtr and foo, then simply fall out of scope
		// foo should be cleaned up automatically
		RefCountPtr<foo> rcp(new foo()) ;
		std::cout << "  rcp (" << rcp->bar() << ") count " << rcp.getRefCount() << std::endl ;
	}



	std::cout << "\n### STL Container ###" << std::endl ;
	std::list<RefCountPtr<foo> > l ;

	RefCountPtr<foo> conttest(new foo()) ;
	std::cout << "  conttest (" << conttest->bar() << ") count " << conttest.getRefCount() << std::endl ;

	for(int i = 0; i < 5; i++)
	{
		// adding to the container copies the object
		l.push_back(conttest) ;
		std::cout << "  container size = " << l.size() << " ref count(" << l.back()->bar() << ") = " << l.back().getRefCount() << std::endl ;
	}

	// clear original reference
	conttest.clear() ;
	std::cout << "  container references (" << l.front()->bar() << ") count " << l.front().getRefCount() << std::endl ;

	RefCountPtr<foo> outOfContainer(l.back()) ;
	std::cout << "  outOfContainer (" << outOfContainer->bar() << ") count " << outOfContainer.getRefCount() << std::endl ;

	// clear the container
	l.clear() ;
	std::cout << "  Clearing container" << std::endl ;
	std::cout << "  outOfContainer (" << outOfContainer->bar() << ") count " << outOfContainer.getRefCount() << std::endl ;


	std::cout << "\n### End ###" << std::endl ;
	std::cout << "  remaining RefCountPtr sill fall out of scope and be cleaned up automatically :-) " << std::endl ;

	return(0) ;
}
