#ifndef PALLELDEPENDENCYANALYSIS_H
#define PALLELDEPENDENCYANALYSIS_H
/////////////////////////////////////////////////////////////////////////////////////////
// ParallelDependencyAnalysis.h - Evaluates the dependency graph for all packages      //
// ver 1.0                                                                             //
// Application: Evaluate dependency graph                                              //
//              CSE687 - Object Oriented Design                                        //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			                   //
// Author:		Yilan Li, Syracuse University, (315)382-9302				           //
//				yli41@syr.edu												           //
/////////////////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module defines a ParallelDependencyAnalysis package. It evaluates the dependency graph
* for all the packages in a specified file collection. For each file the analyses run asynchronously,
* using the facilities of the Task clas.
*
* Public Interface:
* =================
* ParallelDependencyAnalysis(size_t threadnum); // create ParallelDependencyAnalysis instance
* void start(size_t pn, char* arg[]);			// get file queue and start analyze;
* void analysis();								// first pass: do type analysis; second pass: do dependency analysis;												
*
* Build Process:
* ==============
* Required files
* - ParallelDependencyAnalysis.h ParallelDependencyAnalysis.cpp Task.h Task.cpp 
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
* ver 1.0 : 2 April 16
* - first release
*
*/

#include <iostream>
#include <string>
#include <map>
#include <mutex>
#include "../Task/Task.h"
#include "../FileMgr/FileMgr.h"
#include "../Analyzer/TypeAnalysis.h"
#include "../Analyzer/DependencyAnalysis.h"

///////////////////////////////////////////////////////////////////////////////
// ParallelDependencyAnalysis class finds all file dependencies of a c++ file

class ParallelDependencyAnalysis
{
public:
	using Table = std::map<std::string, std::vector<std::string>>;
	using SymbolTable = std::map<std::string, std::vector<TypeInfo>>;
	using DependencyTable = std::map<std::string, std::set<std::string>>;
	ParallelDependencyAnalysis(size_t threadnum);
	~ParallelDependencyAnalysis();
	void start(size_t pn, char* arg[]);
	void analysis();	
private:
	size_t thread;
	DataStore datastore;
	SymbolTable* typeAnalysis();
	DependencyTable* dependencyAnalysis();
	BlockingQueue<std::string> typeblockingQueue;
	BlockingQueue<std::string> depblockingQueue;
	std::mutex ioLock;
	SymbolTable* mergedSymbolTable;
	DependencyTable* mergedDependencyTable;
	void doTypeAnalysis();
	void doDependencyAnalysis();
	void showTypeAnalysis();
	void showDependencyAnalysis();
	BlockingQueue<std::string>* getTypeBlockingQueue(){ return &typeblockingQueue; }
	BlockingQueue<std::string>* getDepBlockingQueue() { return &depblockingQueue; }
	SymbolTable* getSymbolTable() { return mergedSymbolTable;  }
	DependencyTable* getDepTable() { return mergedDependencyTable; }
	SymbolTable* mergesymbolTable(std::string f, Table* t);
	DependencyTable mergedependencyTable(DependencyTable* pt);
	void typeshow(std::vector<TypeInfo> t);
};

#endif
