#ifndef PARALLELDEPENDENCYEXECUTIVE_H
#define PARALLELDEPENDENCYEXECUTIVE_H
/////////////////////////////////////////////////////////////////////////////////
// ParallelDependencyExecutive.cpp - conducts dependency analysis              //
// ver 1.0                                                                     //
// Application: Parallel Dependency Analysis                                   //
//              CSE687 - Object Oriented Design                                //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			           //
// Author:		Yilan Li, Syracuse University, (315)382-9302				   //
//				yli41@syr.edu												   //
/////////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module defines a ParallelDependencyExecutive package. It collects all of the files
* matching one or more patterns in a directory tree rooted at a specified path. On that 
* collection it conducts a dependency analysis and then displays the analysis results.
*
* Public Interface:
* =================
* ParallelDependencyExecutive();								// create ParallelDependencyExecutive instance
* void RequirementTwoandThree(std::string p);					// requirement two&three demo
* void RequirementOne();										// requirement one demo
* void RequirementFour();										// requirement four demo
* void RequirementFive();										// requirement five demo
* void RequirementSix();										// requirement six demo
* void RequirementSeven();										// requirement seven demo
* void RequirementEightandNine(int argc, char* argv[]);			// requirement eight&nine demo
*
* Build Process:
* ==============
* Required files
* - ParallelDependencyExecutive.h ParallelDependencyExecutive.cpp
*   ParallelDependencyAnalysis.h ParallelDependencyAnalysis.cpp Task.h Task.cpp 
*   TypeAnalysis.h TypeAnalysis.cpp DependencyAnalysis.h DependencyAnalysis.cpp 
*   FileMgr.h FileMgr.cpp FileSystem.h FileSystem.cpp DataStore.h DataStore.cpp ThreadPool.h 
*   BlockingQueue.h BlockingQueue.cpp Parser.h Parser.cpp ConfigureParser.h ThreadPool.cpp
*   ConfigureParser.cpp ScopeStack.h ScopeStack.cpp RulesAndActions.h RulesAndActions.cpp
*   itokcollection.h SemiExp.h SemiExp.cpp Tokenizer.h Tokenizer.cpp ParserUtilities.h
*   ParserUtilities.cpp 
* Build commands
* - devenv Project_3.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.0 : 4 April 16
* - first release
*
*/

#include <mutex>
#include "../ParallelDependencyAnalysis/ParallelDependencyAnalysis.h"

//////////////////////////////////////////////////////////////////////
// ParallelDependencyExecutive class 

class ParallelDependencyExecutive
{
public:
	ParallelDependencyExecutive();
	~ParallelDependencyExecutive();
	void RequirementTwoandThree(std::string p);
	void RequirementOne();
	void RequirementFour();
	void RequirementFive();
	void RequirementSix();
	void RequirementSeven();
	void RequirementEightandNine(int argc, char* argv[]);
private:
	std::mutex ioLock;
};

#endif
