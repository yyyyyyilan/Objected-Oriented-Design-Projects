#include <iostream>
#include <string>

using namespace Scanner;
using namespace Utilities;

typedef unsigned long ulong;

std::function<void(const std::string&)> Title =
[](auto src) { StringHelper::Title(src, '='); };

namespace RequirementSix
{
	struct TEST
	{
		std::string _name;
		std::string _type;
	};

	class Parser
	{
		void addRule(IRule* pRule);
		bool next();
		bool parse();
		bool addAction(IAction *pAction);
	};

	class IBuilder
	{
	public:
		virtual ~IBuilder() {}
		virtual Parser* Build() = 0;
	};

	class IAction
	{
	public:
		virtual ~IAction() {}
		virtual void doAction(ITokCollection*& pTc) = 0;
	};

	int main(int argc, char* argv[])
	{
		std::cout << "Test requirement six";
	}
}