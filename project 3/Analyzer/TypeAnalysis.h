#ifndef TYPEANALYSIS_H
#define TYPEANALYSIS_H
/////////////////////////////////////////////////////////////////////////////////
// TypeAnalysis.h - Finds all the types defined in a C++ file                  //
// ver 1.0                                                                     //
// Application: Evaluate specified package                                     //
//              CSE687 - Object Oriented Design                                //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			           //
// Author:		Yilan Li, Syracuse University, (315)382-9302				   //
//				yli41@syr.edu												   //
/////////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module defines a class to find all the types defined in a c++ source file.
* It detects type definitions: classes, structs, enums, typedefs and aliases. It 
* also detects global functions and functions in classes.
*
* Public Interface:
* =================
* TypeAnalysis(std::string f);		//create TypeAnalysis instance
* Repository parse(std::string pa)  //using facilities in Parser package to parse the file
* typeanalysis.getTable()			//create partial symbol table for one file
* typeanalysis.displayTable()		//display partial symbol table
*
* Build Process:
* ==============
* Required files
* - TypeAnalysis.h TypeAnalysis.cpp Parser.h Parser.cpp ConfigureParser.h 
*   ConfigureParser.cpp itokcollection.h ScopeStack.h ScopeStack.cpp
*   RulesAndActions.h RulesAndActions.cpp SemiExp.h SemiExp.cpp Tokenizer.h 
*   Tokenizer.cpp TokenizerUtilities.h TokenizerUtilities.cpp  FileSystem.h
*   FileSystem.cpp
* Build commands
* - devenv Project_3.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.0 : 26 Mar 16
* - first release
*
*/

#include <map>
#include <iostream>
#include <vector>
#include "../Parser/RulesAndActions.h"
#include "../BlockingQueue/BlockingQueue.h"

///////////////////////////////////////////////////////////////
// TypeAnalysis class finds all types define in a c++ file

class TypeAnalysis
{
public:
	using Table = std::map<std::string, std::vector<std::string>>;
	TypeAnalysis();
	TypeAnalysis(std::string f);
	~TypeAnalysis();
	Repository parse(std::string pa);	
	void displayTable();
	Table getTable();
private:
	Repository* pRepo;
	Table* symbolTable;
};

#endif
