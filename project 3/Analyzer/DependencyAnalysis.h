#ifndef DEPENDENCYANALYSIS_H
#define DEPENDENCYANALYSIS_H
/////////////////////////////////////////////////////////////////////////////////
// DependencyAnalysis.cpp - Finds all file dependencies for a file             //
// ver 1.2                                                                     //
// Application: Evaluate specified package                                     //
//              CSE687 - Object Oriented Design                                //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			           //
// Author:		Yilan Li, Syracuse University, (315)382-9302				   //
//				yli41@syr.edu												   //
/////////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module defines a DependencyAnalysis package. It finds all other files from the 
* file collection on which they depend. And it does not record depenencies of a file
* on files outside the file.
* 
* Public Interface:
* =================
*   DependencyAnalysis(std::string f);							// create DependencyAnalysis instance
*	void DependencyTask(std::string path, SymbolTable* s);		// do dependency analysis
*	std::map<std::string, std::set<std::string>> getTable();	// create dependency table
*	void displayTable();										// show dependency result table
*
* Build Process:
* ==============
* Required files
* - DependencyAnalysis.h DependencyAnalysis.cpp TypeAnalysis.h TypeAnalysis.cpp
*   Task.h Task.cpp Parser.h Parser.cpp ConfigureParser.h ConfigureParser.cpp  
*   ScopeStack.h ScopeStack.cpp RulesAndActions.h RulesAndActions.cpp itokcollection.h
*   SemiExp.h SemiExp.cpp Tokenizer.h Tokenizer.cpp TokenizerUtilities.h 
*   TokenizerUtilities.cpp ThreadPool.h ThreadPool.cpp FileSystem.h FileSystem.cpp
* Build commands
* - devenv Project_3.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.2 : 3 April 16
* - fixed the bug that can't distinguish same type with different namespace
* ver 1.1 : 1 April 16
* - Added a type analysis element TypeInfo
*   Changed Class DependencyAnalysis to operate with generic types. (DependencyAnalysis<T>)
* ver 1.0 : 30 Mar 16
* - first release
*
*/

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <utility>
#include "../Parser/RulesAndActions.h"
#include "../Parser/ConfigureParser.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Tokenizer/Tokenizer.h"
#include "TypeAnalysis.h"
using namespace Scanner;

///////////////////////////////////////////////////////////////////////
//// type analysis element 

struct TypeInfo
{
	std::string _name;
	std::string _type;
	std::string _file;
	std::string _namespace;
	std::string show()
	{
		std::ostringstream temp;
		temp << "\t [ FILE: ";
		temp << _file << " ";
		temp << "\t   NAME: ";
		temp << _name << " ";
		temp << "\t   TYPE: ";
		temp << _type << " ";
		temp << "\t   NAMESPACE: ";
		temp << _namespace << " ]";
		return temp.str();
	}
};

/////////////////////////////////////////////////////////////////////
// DependencyAnalysis class finds all file dependencies of a c++ file

template <typename type>
class DependencyAnalysis
{
public:
	using Table = std::map<std::string, std::vector<std::string>>;
	using SymbolTable = std::map<std::string, std::vector<type>>;
	DependencyAnalysis();
	DependencyAnalysis(std::string f);
	~DependencyAnalysis();
	void DependencyTask(std::string path, SymbolTable* s);
	std::map<std::string, std::set<std::string>> getTable();
	void displayTable();
private:
	std::string currentNS;
	std::map<std::string, std::set<std::string>> _dependencyTable;
	BlockingQueue<std::string> _filequeue;
	std::map<std::string, std::set<std::string>> findDependency(std::string f, std::string t, SymbolTable* s, bool ns, bool as);
	std::map<std::string, std::set<std::string>> checkNamespace(std::set<std::vector<std::string>> file, std::string f);
};
//----< void constructor >-----------------------------------------------------

template <typename type>
DependencyAnalysis<type>::DependencyAnalysis()
{
	currentNS = "GlobalNamespace";
};
//----< construct DependencyAnalysis from file name >--------------------------

template <typename type>
DependencyAnalysis<type>::DependencyAnalysis(std::string f)
{};
//----< constructor of dependency >--------------------------------------------

template <typename type>
DependencyAnalysis<type>::~DependencyAnalysis()
{};
//----< finds all other depended files >---------------------------------------

template <typename type>
void DependencyAnalysis<type>::DependencyTask(std::string path, SymbolTable* st)
{
	std::ifstream in(path);
	Toker toker;
	toker.attach(&in);
	std::string tok;
	currentNS = "Globalnamespace";
	do
	{
		bool nsFlag = false;
		bool aliasFlag = false;
		tok = toker.getTok();
		if (tok != "main")
		{
			if (tok == "using")
			{
				tok = toker.getTok();
				if (tok == "namespace")
				{
					tok = toker.getTok();
					nsFlag = true;
					currentNS = currentNS.append("::");
					currentNS = currentNS.append(tok);
				}
				else
					break;
			}
			if (tok == "namespace")
			{
				tok = toker.getTok();
				aliasFlag = true;
			}
			if (tok == "\n")
				tok = "newline";
			else 
				_dependencyTable = findDependency(path, tok, st, nsFlag, aliasFlag);
		}
	} while (in.good());
}
//----< find all file dependencies of one file >---------------------

template <typename type>
std::map<std::string, std::set<std::string>> DependencyAnalysis<type>::findDependency(std::string f, std::string tok, SymbolTable* s, bool ns,bool as)
{
	std::set<std::vector<std::string>> filecollection;
	for (SymbolTable::iterator it = s->begin(); it != s->end(); ++it)
	{
		if (f != it->first)
		{
			size_t num = it->second.size();
			for (size_t i = 0; i < num; i++)
			{
				if (tok == it->second[i]._name)
				{
					if (ns == true)
					{
						if (it->second[i]._type == "namespace")
							filecollection.insert({ it->first, it->second[i]._namespace });
					}
					else if (as == true)
					{
						if (it->second[i]._type == "namespace")
							filecollection.insert({ it->first, it->second[i]._namespace });
					}
					else
						filecollection.insert({ it->first, it->second[i]._namespace });
				}
					
			}
		}
	}
	_dependencyTable = checkNamespace(filecollection, f);
	return _dependencyTable;
}
//----< helper function to check namespace >-----------------------------------

template <typename type>
std::map<std::string, std::set<std::string>> DependencyAnalysis<type>::checkNamespace(std::set<std::vector<std::string>> file, std::string f)
{
	if (file.size() == 1)
	{
		std::vector<std::string> temp = *file.begin();
		_dependencyTable[f].insert(temp.at(0));
	}
	else if (file.size() > 1)
	{
		std::set<std::vector<std::string>>::iterator in;
		size_t s = file.size();
		for (in = file.begin(); in != file.end(); ++in)
		{
			const std::vector<std::string>& temp = (*in);
			if (currentNS == temp.at(1))
				_dependencyTable[f].insert(temp.at(0));
		}
	}
	return _dependencyTable;
}
//----< display dependency result of one file >----------------------

template <typename type>
void DependencyAnalysis<type>::displayTable()
{
	std::map<std::string, std::set<std::string>> t = getTable();
	for (std::map<std::string, std::set<std::string>>::iterator it = t.begin();
	it != t.end(); ++it)
	{
		std::cout << "[ FILE ] : ";
		std::cout << it->first << "\n";
		std::cout << "Dependent files list: \n";
		std::set<std::string> tn = it->second;
		while (!tn.empty())
		{
			std::cout << "\t\t--" << *tn.begin() << "\n";
			tn.erase(tn.begin());
		}
		std::cout << "\n\n";
	}
}
//----< return the merged result table >---------------------------------------

template <typename type>
std::map<std::string, std::set<std::string>> DependencyAnalysis<type>::getTable()
{
	return _dependencyTable;
}

#endif