#include <cutil/MapIterator.h>

#include <iostream>
#include <map>

using cutil::MapKeyIterator ;
using cutil::MapValueIterator ;

class FOO
{
	private:
		typedef std::map<int, std::string> Container_t  ;
		Container_t theMap ;

	public:
		FOO() ;

		typedef MapKeyIterator<const Container_t, Container_t::const_iterator> const_iterator_keys ;
		typedef MapValueIterator<const Container_t, Container_t::const_iterator> const_iterator_vals ;

		const_iterator_keys begin_keys() const ;
		const_iterator_keys end_keys() const ;

		const_iterator_vals begin_vals() const ;
		const_iterator_vals end_vals() const ;
};

FOO::FOO()
{
	theMap.insert(std::pair<int, std::string>(2, "two")) ;
	theMap.insert(std::pair<int, std::string>(4, "four")) ;
	theMap.insert(std::pair<int, std::string>(8, "six")) ;
	theMap.insert(std::pair<int, std::string>(16, "eight")) ;
	theMap.insert(std::pair<int, std::string>(32, "thirty-two")) ;
	theMap.insert(std::pair<int, std::string>(64, "sixy-four")) ;
}

FOO::const_iterator_keys FOO::begin_keys() const
{
	Container_t::const_iterator citer = theMap.begin() ;
	const_iterator_keys ret(theMap, citer) ;
	return(ret) ;
}

FOO::const_iterator_keys FOO::end_keys() const
{
	Container_t::const_iterator citer = theMap.end() ;
	const_iterator_keys ret(theMap, citer) ;
	return(ret) ;
}

FOO::const_iterator_vals FOO::begin_vals() const
{
	Container_t::const_iterator citer = theMap.begin() ;
	const_iterator_vals ret(theMap, citer) ;
	return(ret) ;
}

FOO::const_iterator_vals FOO::end_vals() const
{
	Container_t::const_iterator citer = theMap.end() ;
	const_iterator_vals ret(theMap, citer) ;
	return(ret) ;
}

int main()
{
	FOO foo ;

	std::cout << "Keys:\n" ;
	for(FOO::const_iterator_keys citer = foo.begin_keys(); citer != foo.end_keys(); ++citer)
	{
		std::cout << (*citer) << "\n" ;
	}
	std::cout << "\n" << std::endl ;

	std::cout << "Values:\n" ;
	for(FOO::const_iterator_vals citer = foo.begin_vals(); citer != foo.end_vals(); ++citer)
	{
		std::cout << (*citer) << "\n" ;
	}
	std::cout.flush() ;

	return(0) ;
}
