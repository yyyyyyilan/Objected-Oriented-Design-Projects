#include <iostream>
#include <string>


//----< namespace scope test >---------------------------------------
namespace NamespaceScope
{
	//----< struct scope test >--------------------------------------
	struct StructScope
	{
		std::cout << "This is a struct scope." << std::endl;
	};

	//----< enumeration scope test >---------------------------------
	enum EnumScope { first, second, third };

	//----< class scope test >---------------------------------------
	class ClassScope
	{
		//----< function scope test >---------------------------------
		void FunctionScope1(int i)
		{
			std::cout << "This is a function scope." << std::endl;
		}
		void FunctionScope2();
	};

	//----< function scope test >---------------------------------
	void ClassScope::FunctionScope2()
	{
		std::cout << "This is another function scope." << std::endl;
	}

}

