#ifndef RULESANDACTIONS_H
#define RULESANDACTIONS_H
///////////////////////////////////////////////////////////////////////
// RulesAndActions.h - declares new parsing rules and actions        //
// ver 2.6                                                           //
// Language:  C++, Visual Studio 2015 Community                      //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			 //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:		Yilan Li, Syracuse University, (315)382-9302         //
//				yli41@syr.edu									     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines several action classes.  Its classes provide
specialized services needed for specific applications.  The modules
Parser, SemiExpression, and Tokenizer, are intended to be reusable
without change.  This module provides a place to put extensions of
these facilities and is not expected to be reusable.

Public Interface:
=================
Toker t(someFile);              // create tokenizer instance
SemiExp se(&t);                 // create a SemiExp attached to tokenizer
Parser parser(se);              // now we have a parser
Rule1 r1;                       // create instance of a derived Rule class
Action1 a1;                     // create a derived action
r1.addAction(&a1);              // register action with the rule
parser.addRule(&r1);            // register rule with parser
while(se.getSemiExp())          // get semi-expression
parser.parse();                //   and parse it

Build Process:
==============
Required files
- Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
RulesAndActions.h, RulesAndActions.cpp, ConfigureParser.cpp,
ItokCollection.h, SemiExp.h, SemiExp.cpp, Tokenizer.h, Tokenizer.cpp
Build commands 
- devenv Project_2.sln /rebuild debug

Maintenance History:
====================
ver 2.6 : 10 Mar 16 by Yilan Li
- Added class PushInitialization:public IACtion and class InitializationDefinition:public IRule
ver 2.5 : 5 Mar 16 by Yilan Li
- Added sPtr self to element struct to use when adding children
- Fixed the bug that can't get the same tree when parsing one file
- Added void setInitial(bool f) to clean a tree after parsing one file
ver 2.4 : 29 Feb 16 by Yilan Li
- Added more functions in Repository class
   - public function void setTree() and private function element* makeRootElem() 
	 to provide a field to hold root of AST
   - MTree<std::string>* getTree(): return AST to make sure create one tree for only one file
   - void addNode(element* elem): interface to add a new node to AST
- Added PushSpecialkeyword and PrintSpecialkeyword actions to SpecialkeywordDefinition rule
ver 2.3 : 22 Feb 16 by Yilan Li
- Added more members in element struct: 
	 size_t lineCountStart: the start line number of an element
	 size_t lineCountEnd: the end line number of a element
	 element* parentElement: parent element pointer of an element 
ver 2.2 : 20 Feb 16 by Yilan Li
- Added following classes:
	class ClassDefinition :public IRule
	class PushClass :public IAction
	class PrintClass:public IAction
	class StructDefinition :public IRule
	class PushStruct :public IAction
	class PrintStruct:public IAction
	class NamespaceDefinition :public IRule
	class PushNamespace	:public IAction
	class PrintNamespace :public IAction
	class EnumDefinition :public IRule
	class PushEnum :public IAction
ver 2.1 : 15 Feb 16 by Jim Fawcett
- small functional change to a few of the actions changes display strategy
- preface the (new) Toker and SemiExp with Scanner namespace
ver 2.0 : 01 Jun 11 by Jim Fawcett
- added processing on way to building strong code analyzer
ver 1.1 : 17 Jan 09 by Jim Fawcett
- changed to accept a pointer to interfaced ITokCollection instead
of a SemiExpression
ver 1.0 : 12 Jan 06 by Jim Fawcett
- first release

*/

#include "../Tokenizer/Tokenizer.h"
#include "../SemiExpression/SemiExp.h"
#include "../SemiExpression/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "../AbstractSyntaxTree/MTree.h"

using MNodeStr = MNode < std::string >;
using sPtr = std::shared_ptr < MNodeStr >;

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific

struct element
{
	std::string type;
	std::string name;
	size_t lineCountStart;
	size_t lineCountEnd;
	size_t cc;
	size_t level;
	element* parentElement;
	sPtr self;

	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << "start: ";
		temp << lineCountStart;
		temp << ", ";
		temp << "end: ";
		temp << lineCountEnd;
		temp << ")";
		temp << "complexity: ";
		temp << cc;
		temp << "Parent element name: ";
		temp << parentElement->name;
		return temp.str();
	}
};

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.
/*
* ToDo:
* - add AST Node class
* - provide field to hold root of AST
* - provide interface to access AST
* - provide helper functions to build AST, perhaps in Tree class
* - copy construction of tree class (yilan)
*/

class Repository  // application specific
{
	element* makeRootElem()
	{
		element* r = new element;
		r->name = "Global";
		r->type = "Namespace";
		r->level = 1;
		r->lineCountStart = 0;
		r->lineCountEnd = 0;
		r->parentElement = nullptr;
		return r;
	}
	ScopeStack<element*> stack;
	Scanner::Toker* p_Toker;
	static MTree<std::string>* mTree;
	static sPtr pRoot;
	static bool first;
	size_t nlevel;
public:
	Repository(Scanner::Toker* pToker)
	{
		p_Toker = pToker;	
		if (first)
		{
			setTree();
		}
		first = false;
	}
	void setTree()
	{
		//sPtr r(new MNode<std::string>("Global"));
		MTree<std::string>* m = new MTree<std::string>;
		mTree = std::move(m);
		mTree->root()->value() = "Global";
		mTree->root()->setType("Namespace");
		mTree->root()->setStartLine(0);
		mTree->root()->setEndLine(0);
		mTree->root()->setLevel(0);
		mTree->root()->setParent(nullptr);
		pRoot = std::move(mTree->root());		
		element* root= makeRootElem();
		root->self = pRoot;
		scopeStack().push(root);
	}
	MTree<std::string>* getTree()
	{
		return mTree;
	}
	ScopeStack<element*>& scopeStack()
	{
		return stack;
	}
	Scanner::Toker* Toker()
	{
		return p_Toker;
	}
	size_t lineCount()
	{
		return (size_t)(p_Toker->LineCounter());
	}
	void addNode(element* elem)
	{
		sPtr tempNode;
		tempNode = mTree->makeNode(elem->name, elem->type, elem->lineCountStart, elem->lineCountEnd,elem->level);
		elem->self = tempNode;		//keep record of self sPtr
		sPtr pPtr = elem->parentElement->self;
		MTree<std::string>* t = getTree();
		pPtr->addChild(tempNode);
	}	
	void setInitial(bool f)
	{
		first = f;
	}
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("}") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
	Repository* p_Repos;
public:
	HandlePop(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n--EndOfScope rule";
		if (p_Repos->scopeStack().size() == 0)
			return;

		element* elem= p_Repos->scopeStack().top();
		elem->lineCountEnd = p_Repos->lineCount();
		elem = p_Repos->scopeStack().pop();	
		sPtr temp = elem->self;
		elem->self->setEndLine(elem->lineCountEnd);
		if (elem->type == "function")
		{
			//std::cout << "\nHandlePop";
			size_t line = elem->lineCountEnd - elem->lineCountStart + 1;
			std::cout << "\n \t Function " << elem->name << ", total lines = " << line;
			std::cout << "\n";
		}
	}
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("#") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
	Repository* p_Repos;
public:
	PrintPreproc(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  Preproc Stmt: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch", "else", "try"}; 
		for (size_t i = 0; i<7; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
			{
					doActions(pTc);
					return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
	Repository* p_Repos;
	bool isLambda(ITokCollection& tc)
	{
		size_t p1 = tc.find("[");
		size_t p2 = tc.find("]");
		if (p1 < p2)
		{
			if ((p2 < tc.length()) && (p2= tc.find("(")-1))
			{
				return true;
			}
		}
		return false;
	}
public:
	PushFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// next statement is now  handled in PrintFunction
		// push function scope
		ITokCollection& tc = *pTc;
		std::string name = "";
		if (isLambda(tc))
		{
			name = "lambda";
		}
		else
		{
			name = (*pTc)[pTc->find("(") - 1];
		}
		element *elem = new element;
		elem->type = "function";
		elem->name = name;
		elem->lineCountStart = p_Repos->lineCount();
		elem->level = p_Repos->scopeStack().size() + 1;
		element* ep = p_Repos->scopeStack().top();
		elem->parentElement = ep;		//get parent then push into stack
		p_Repos->scopeStack().push(elem);
		p_Repos->addNode(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
	Repository* p_Repos;
	bool isLambda(ITokCollection& tc)
	{
		size_t p2 = tc.find("[");
		size_t p3 = tc.find("]");
		if ((p2 < p3) && ( (p2+1) != p3))
		{
			if ((p3 < tc.length()) && (p3 = tc.find("{") - 1))
			{
				return true;
			}
		}
		return false;
	}
public:
	PrintFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (isLambda(tc))
		{
			std::cout << "\n  LambdaFuncDef: " << pTc->show().c_str();
		}
		else
		{
			std::cout << "\n  FuncDef: " << pTc->show().c_str();
		}
	}
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		pTc->remove("public");
		pTc->remove(":");
		pTc->trimFront();
		size_t len = pTc->find(")");
		std::cout << "\n\n  Pretty Stmt:    ";
		for (size_t i = 0; i<len + 1; ++i)
			std::cout << (*pTc)[i] << " ";
		std::cout << "\n";
	}
};

///////////////////////////////////////////////////////////////
// rule to detect class definitions

class ClassDefinition :public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("class");
			if (len < tc.length())
			{
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push class name onto ScopeStack

class PushClass : public IAction
{
	Repository* p_Repos;
public:
	PushClass(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// push class scope
		std::string name = (*pTc)[pTc->find("class") + 1];
		element* elem = new element;
		elem->type = "class";
		elem->name = name;
		elem->lineCountStart = p_Repos->lineCount();
		elem->level = p_Repos->scopeStack().size() + 1;
		element* ep = p_Repos->scopeStack().top();
		elem->parentElement = ep;		//get parent then push into stack
		p_Repos->scopeStack().push(elem);
		p_Repos->addNode(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a class def
// to console

class PrintClass : public IAction
{
	Repository* p_Repos;
public:
	PrintClass(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  ClassDef: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect struct definitions

class StructDefinition :public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("struct");
			if (len < tc.length())
			{
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push struct name onto ScopeStack

class PushStruct : public IAction
{
	Repository* p_Repos;
public:
	PushStruct(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// push struct scope
		std::string name = (*pTc)[pTc->find("struct") + 1];
		element* elem = new element;
		elem->type = "struct";
		elem->name = name;
		elem->lineCountStart = p_Repos->lineCount();
		elem->level = p_Repos->scopeStack().size() + 1;
		element* ep = p_Repos->scopeStack().top();
		elem->parentElement = ep;		//get parent then push into stack
		p_Repos->scopeStack().push(elem);
		p_Repos->addNode(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a strcut def
// to console

class PrintStruct : public IAction
{
	Repository* p_Repos;
public:
	PrintStruct(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  StructDef: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect namespace definitions

class NamespaceDefinition :public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("namespace");
			if (len < tc.length())
			{
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push namespace onto ScopeStack

class PushNamespace : public IAction
{
	Repository* p_Repos;
public:
	PushNamespace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// push namespace scope
		std::string name = (*pTc)[pTc->find("namespace") + 1];
		element* elem = new element;
		elem->type = "namespace";
		elem->name = name;
		elem->lineCountStart = p_Repos->lineCount();
		elem->level = p_Repos->scopeStack().size() + 1;
		element* ep = p_Repos->scopeStack().top();
		elem->parentElement = ep;		//get parent then push into stack
		p_Repos->scopeStack().push(elem);
		p_Repos->addNode(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a namespace def
// to console

class PrintNamespace : public IAction
{
	Repository* p_Repos;
public:
	PrintNamespace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  NamespaceDef: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect enumeration definitions

class EnumerationDefinition :public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("enum");
			if (len < tc.length())
			{
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push enumeration onto ScopeStack

class PushEnumeration :public IAction
{
	Repository* p_Repos;
public:
	PushEnumeration(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// push enumeration scope
		std::string name = (*pTc)[pTc->find("enum") + 1];
		element* elem = new element;
		elem->type = "enumeration";
		elem->name = name;
		elem->lineCountStart = p_Repos->lineCount();
		elem->level = p_Repos->scopeStack().size() + 1;
		element* ep = p_Repos->scopeStack().top();
		elem->parentElement = ep;		//get parent then push into stack
		p_Repos->scopeStack().push(elem);
		p_Repos->addNode(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a enumeration def
// to console

class PrintEnumeration : public IAction
{
	Repository* p_Repos;
public:
	PrintEnumeration(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  EnumDef: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect catch definitions

class CatchDefinition :public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t p = tc.find("catch");
			if (p<(tc.length() - 1))
			{
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push catch onto ScopeStack

class PushCatch :public IAction
{
	Repository* p_Repos;
public:
	PushCatch(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// push catch scope
		std::string name;
		element* elem = new element;
		elem->type = "catch block";
		elem->name = "catch";
		elem->lineCountStart = p_Repos->lineCount();
		elem->level = p_Repos->scopeStack().size() + 1;
		element* ep = p_Repos->scopeStack().top();
		elem->parentElement = ep;		//get parent then push into stack
		p_Repos->scopeStack().push(elem);
		p_Repos->addNode(elem);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect try definitions

class TryDefinition :public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t p = tc.find("try");
			if (p<(tc.length() - 1))
			{
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push try onto ScopeStack

class PushTry :public IAction
{
	Repository* p_Repos;
public:
	PushTry(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// push try scope
		std::string name;
		element* elem = new element;
		elem->type = "try block";
		elem->name = "try";
		elem->lineCountStart = p_Repos->lineCount();
		elem->level = p_Repos->scopeStack().size() + 1;
		element* ep = p_Repos->scopeStack().top();
		elem->parentElement = ep;		//get parent then push into stack
		p_Repos->scopeStack().push(elem);
		p_Repos->addNode(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a catch def to 
// console

class PrintCatch : public IAction
{
	Repository* p_Repos;
public:
	PrintCatch(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  CatchDef: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a try def to console

class PrintTry : public IAction
{
	Repository* p_Repos;
public:
	PrintTry(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  TryDef: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect condition and loop scope definition

class SpecialKeyWordsDefinition :public IRule
{
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "do", "else"};
		for( size_t i = 0; i<6; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && isSpecialKeyWord(tc[len - 1]))
			{
				doActions(pTc);
				return true;
			}
			if ((tc[0]=="else") || (tc[0]=="do"))
			{
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push condition and loop scope onto ScopeStack

class PushSpecialKeyWords :public IAction
{
	Repository* p_Repos;
	bool isCondition(const std::string& tok)
	{
		const static std::string keys1[]
			= { "if", "switch"};
		for (size_t i = 0; i<2; ++i)
			if (tok == keys1[i])
				return true;
		return false;
	}
	bool isLoop(const std::string& tok)
	{
		const static std::string keys2[]
			= { "do", "while", "for" };
		for (size_t i = 0; i<3; ++i)
			if (tok == keys2[i])
				return true;
		return false;
	}
	bool isElse(const std::string& tok)
	{
		if (tok == "else")
			return true;
		return false;
	}
public:
	PushSpecialKeyWords(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// push special keywords scope
		ITokCollection& tc = *pTc;
		std::string name = "";
		size_t p = tc.find("{");
		if ((tc.find("else") <p) && (p < tc.length()))
			name = "else";
		if ((tc.find("do") <p) && (p < tc.length()))
			name = "do";
		std::string word = tc[tc.find("(") - 1];
		if (isCondition(word))
		{
			name = "ConditionStatement-" + tc[tc.find("(") - 1];
		}
		if (isLoop(word))
		{
			name = "LoopStatement-" + tc[tc.find("(") - 1];
		}
		element* elem = new element;
		elem->type = "special keyword";
		elem->name = name;
		elem->lineCountStart = p_Repos->lineCount();
		elem->level = p_Repos->scopeStack().size() + 1;
		element* ep = p_Repos->scopeStack().top();
		elem->parentElement = ep;		//get parent then push into stack
		p_Repos->scopeStack().push(elem);
		p_Repos->addNode(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a condition and 
// loop def to console

class PrintSpecialKeyWords : public IAction
{
	Repository* p_Repos;
public:
	PrintSpecialKeyWords(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  SpecialKeywordsDef: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect anonymous scope definition
class AnonymousDefinition :public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("{") == 0)
		{
			doActions(pTc);
			return true;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push anonymous scope onto ScopeStack
class PushAnonymous:public IAction
{
	Repository* p_Repos;
public:
	PushAnonymous(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//push ananomous scope
		element* elem = new element;
		elem->type = "unknown";
		elem->name = "anonymous";
		elem->lineCountStart = p_Repos->lineCount();
		elem->level = p_Repos->scopeStack().size() + 1;
		element* ep = p_Repos->scopeStack().top();
		elem->parentElement = ep;		//get parent then push into stack
		p_Repos->scopeStack().push(elem);
		p_Repos->addNode(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a anonymous to 
// console
class PrintAnonymous :public IAction
{
	Repository* p_Repos;
public:
	PrintAnonymous(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  AnonymousDef: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect initialization
class InitializationDefinition :public IRule
{
	bool hasSpecialKeyWord(ITokCollection& tc)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "do", "else","catch","try" };
		for (size_t i = 0; i < 8; ++i)
		{
			for (size_t j = 0; j < tc.length(); j++)
			{
				if (tc[j] == keys[i])
					return true;
			}
		}
		return false;
	}
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("=");
			if (len < (tc.length()-1) && !hasSpecialKeyWord(tc))
			{
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push initialization scope onto ScopeStack
class PushInitialization :public IAction
{
	Repository* p_Repos;
public:
	PushInitialization(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//push ananomous scope
		element* elem = new element;
		elem->type = "initialization";
		elem->name = "unknown";
		elem->lineCountStart = p_Repos->lineCount();
		elem->level = p_Repos->scopeStack().size() + 1;
		element* ep = p_Repos->scopeStack().top();
		elem->parentElement = ep;		//get parent then push into stack
		p_Repos->scopeStack().push(elem);
		p_Repos->addNode(elem);
	}
};

#endif
