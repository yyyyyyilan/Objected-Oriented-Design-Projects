/////////////////////////////////////////////////////////////////////////////////
// MetricsAnalysis.cpp - evaluate and display AST package                      //
// ver 1.1                                                                     //
// Language:  C++, Visual Studio 2015 Community                                //
// Application: Evaluate specified package, CSE687 - Object Oriented Design    //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			           //
// Author:		Yilan Li, Syracuse University, (315)382-9302				   //
//				yli41@syr.edu												   //
/////////////////////////////////////////////////////////////////////////////////

#include "MetricsAnalysis.h"
#include "../Parser/ConfigureParser.h"
#include "../Parser/Parser.h"
#include "../Utilities/ParserUtilities.h"
#include "../AbstractSyntaxTree/Utilities.h"
#include <iostream>
#include <string>


//#define TEST_METRICSANALYSIS
#ifdef TEST_METRICSANALYSIS


using MNodeStr = MNode < std::string >;
using sPtr = std::shared_ptr < MNodeStr >;
using namespace Scanner;

//----< test stub >--------------------------------------------

#include "../FileMgr/FileSystem.h"

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing MetricsAnalysis class\n ";
	
	if (argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}

	for (int i = 1; i < argc; ++i)
	{
		std::cout << "\n  Processing file " << argv[i];
		std::cout << "\n  " << std::string(16 + strlen(argv[i]), '-');

		//get AST to analysis
		MetricsAnalysis<std::string> metricAnalysis;
		Repository repository = metricAnalysis.parse(argv[i]);
		std::cout << "*****************\n";
		std::cout << "Function Summary" << "\n";
		std::cout << "*****************\n\n";
		metricAnalysis.treeAnalysis();
		std::cout << "*****************\n";
		std::cout << "AST structure" << "\n";
		std::cout << "*****************\n\n";
		metricAnalysis.printTree();
	}
}

#endif
