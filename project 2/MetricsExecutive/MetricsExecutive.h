#ifndef METRICSEXECUTIVE_H
#define METRICSEXECUTIVE_H
/////////////////////////////////////////////////////////////////////////////////
// MetricsExecutive.h - collect metrics for all packages                       //
// ver 1.1                                                                     //
// Language:  C++, Visual Studio 2015 Community                                //
// Application: Evaluate specified package, CSE687 - Object Oriented Design    //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			           //
// Author:		Yilan Li, Syracuse University, (315)382-9302				   //
//				yli41@syr.edu												   //
/////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This module collects metrics for all the packages with names that match a specified
* pattern in a directory tree rooted at a specified path. It's the main entraince of
* the entire project.
*
* Required Files:
* ===============
* Required files:
*		MetricsExecutive.h  MetricsExecutive.cpp  FileMgr.h  FileMgr.cpp  Parser.h  Parser.cpp 
*	    RulesAndActions.h  RulesAndActions.cpp  SemiExp.h  SemiExp.cpp  Tokenizer.h  Tokenizer.cpp  
*       MTree.h  MTree.cpp  MNode.h  MNode.cpp  DataStore.h  DataStore.cpp
*		MetricsAnalysis.h  MetricsAnalysis.cpp  FileSystem.h FileSystem.cpp
*
* Build Process:
* ==============
* - devenv Project_2.sln /debug rebuild 
*
* Maintenance History:
* ====================
* ver 1.1 : 10 Mar 2016 
* - Fixed bug that can't parse multiple files
* ver 1.0 : 3 Mar 2016
* - first release
*/


#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include "../FileMgr/FileMgr.h"
#include "../Analyzer/MetricsAnalysis.h"


using namespace FileSystem;
using namespace Scanner;

/////////////////////////////////////////////////////////
// MetricsExecutive class

class MetricsExecutive
{
public:
	MetricsExecutive(FileMgr fm);
	~MetricsExecutive();
	void displayFileInfo(std::string f);
	void push_back(Repository rp);
	void display(MetricsAnalysis<std::string> m);
	void RequirementDemo(std::string p);
private:
	FileMgr fileMgr;
	std::vector<Repository> repository;
	DataStore ds;
	void RequirementOne();
};

#endif
