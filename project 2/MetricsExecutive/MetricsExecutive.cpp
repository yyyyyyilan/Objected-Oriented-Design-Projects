/////////////////////////////////////////////////////////////////////////////////
// MetricsExecutive.cpp - collecti metrics for all packages                    //
// ver 1.1                                                                     //
// Language:  C++, Visual Studio 2015 Community								   //
// Application: Evaluate specified package, CSE687 - Object Oriented Design    //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			           //
// Author:		Yilan Li, Syracuse University, (315)382-9302				   //
//				yli41@syr.edu												   //
/////////////////////////////////////////////////////////////////////////////////

#include "MetricsExecutive.h"
#include "../Parser/ConfigureParser.h"
#include "../Parser/Parser.h"
#include <iostream>
#include <fstream>
#include <string>

//----< constructor >----------------------------------------------------------
MetricsExecutive::MetricsExecutive(FileMgr fm):fileMgr(fm)
{}

//----< destructor >-----------------------------------------------------------
MetricsExecutive::~MetricsExecutive(){}

//----< display result >-------------------------------------------------------
void MetricsExecutive::displayFileInfo(std::string f)
{
	size_t len = (66 + f.size());
	std::cout << "\n";
	std::cout << "\n" << std::string(len, '*');
	std::cout << "\n" << std::string(20, '*') << "    Processing file   " << f <<
		"    "<< std::string(20, '*');
	std::cout << "\n" << std::string(len, '*');
}

//----< display function info and AST tree structure >-------------------------
void MetricsExecutive::display(MetricsAnalysis<std::string> m)
{
	std::cout << "\n****************\n";
	std::cout << " Function Summary " << "\n";
	std::cout << "******************\n\n";
	m.treeAnalysis();
	std::cout << "\n******************************************\n";
	std::cout << " AST structure [type name startline endline]" << "\n";
	std::cout << "********************************************\n\n";
	m.printTree();
}

//----< function to get repository >-------------------------------------------
void MetricsExecutive::push_back(Repository rp)
{
	repository.push_back(rp);
}

//----< do requirement demo >--------------------------------------------------
void MetricsExecutive::RequirementDemo(std::string p)
{
	RequirementOne();
	std::string line;
	std::ifstream myfile;
	myfile.open(p);
	while(std::getline(myfile, line))
	{
		std::cout << line << "\n";	
	}
	myfile.close();
	std::cout << "\n\n";
}

//----< test function for requirement one >------------------------------------
void MetricsExecutive::RequirementOne()
{
	std::cout <<" Project #2- Code Parser with Abstract Syntax Tree (AST)" << std::endl;
	std::cout <<" Yilan Li  Syracuse University" << std::endl;
	std::cout <<" yli41@syr.edu" << std::endl;
	std::cout <<" Platform: Dell Inspiron 13-7352, Windows 10 Education" << std::endl;
	std::cout <<" Language: C++, Visual Studio 2015 Community\n" << std::endl;
}

//----< test stub >------------------------------------------------------------

//#define TEST_METRICSEXECUTIVE
#ifdef TEST_METRICSEXECUTIVE

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout
			<< "\n  please enter name of file or the pattern of file to process on command line\n\n";
		return 1;
	}
	std::string path = argv[1];
	DataStore ds;
	FileMgr fm(path,ds);
	for (int i = 1; i < argc; i++)
	{
		fm.addPattern(argv[i]);
	}
	fm.search();
	MetricsExecutive metricsExcutive(fm);
	metricsExcutive.RequirementDemo("README.txt");
	for (DataStore::iterator iter = ds.begin(); iter != ds.end(); iter++) 
	{
		std::string fs = *iter;
		metricsExcutive.displayFileInfo(fs);
		MetricsAnalysis<std::string> metricsanalysis;
		metricsExcutive.push_back(metricsanalysis.parse(fs));
		metricsExcutive.display(metricsanalysis);
	}
	return 0;
}
#endif