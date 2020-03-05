///////////////////////////////////////////////////////////////////////
// RulesAndActions.cpp - implements new parsing rules and actions    //
// ver 2.7                                                           //
// Language:  C++, Visual Studio 2015 Community                      //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			 //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:		Yilan Li, Syracuse University, (315)382-9302         //
//				yli41@syr.edu									     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////


#include"RulesAndActions.h"
using Table = std::map<std::string, std::vector<std::string>>;


std::string Repository::_namespace = "GlobalNamespace";

//----< test stub >--------------------------------------------

#ifdef TEST_ACTIONSANDRULES

#include <iostream>
#include "RulesAndActions.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExpression/SemiExp.h"

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing ActionsAndRules class\n "
		<< std::string(30, '=') << std::endl;

	try
	{
		std::queue<std::string> resultsQ;
		PreprocToQ ppq(resultsQ);
		PreprocStatement pps;
		pps.addAction(&ppq);

		FunctionDefinition fnd;
		PrettyPrintToQ pprtQ(resultsQ);
		fnd.addAction(&pprtQ);

		Toker toker("../RulesAndActions.h");
		SemiExp se(&toker);
		Parser parser(&se);
		parser.addRule(&pps);
		parser.addRule(&fnd);
		while (se.get())
			parser.parse();
		size_t len = resultsQ.size();
		for (size_t i = 0; i<len; ++i)
		{
			std::cout << "\n  " << resultsQ.front().c_str();
			resultsQ.pop();
		}
		std::cout << "\n\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}
}
#endif
