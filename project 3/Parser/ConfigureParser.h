#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
///////////////////////////////////////////////////////////////////////
//  ConfigureParser.h - builds and configures parsers                //
//  ver 2.7                                                          //
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
This module builds and configures parsers.  It builds the parser
parts and configures them with application specific rules and actions.

Public Interface:
=================
ConfigParseToConsole conConfig;
conConfig.Build();
conConfig.Attach(someFileName);

* Required files:
* ====================
* - ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
* RulesAndActions.h, RulesAndActions.cpp, itokcollection.h
* SemiExp.h, SemiExp.cpp, Tokenizer.h, Tokenizer.cpp
* Build commands:
* ====================
* - devenv Project_3.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 2.7 : 26 Mar 26 by Yilan Li
* - Added PushTypedef to TypedefDefinition rule, PushAliases to AliasesDefinition rule
* ver 2.6 : 10 Mar 16 by Yilan Li
* - Added PushInitialization to InitializationDefinition rule
* ver 2.5 : 03 Mar 16 by Yilan Li
* - Added an inline function Scanner::Toker* getToker() in order to re-initialized static parameter,
*	which can be used to hangle multiple files
* ver 2.4 : 01 Mar 16 by Yilan Li
* - Changed HandlePush action to BeginningOfScope rule to Anonymous action to PushAnonymous rule 
* ver 2.3 : 25 Feb 16 by Yilan Li
* - Added:
* 	PushSpecialKeyWords and PrintSpecialKeyWords to SpecialKeyWordsDefinition rule
* 	PushCatch action to CatchDefinition rule
* 	PushTry action to TryDefinition rule
* ver 2.2 : 20 Feb 16 by Yilan Li
* - Added following:
* 	PushClass action and PrintClass to ClassDefinition rule
* 	PushStruct action and PrintStruct to StructDefinition rule
* 	PushNamespace action and PrintNamespace to NamespaceDefinition rule
* 	PushEnumeration action to PrintEnumeration EnumerationDefinition rule
* ver 2.1 : 19 Feb 16 by Jim Fawcett
* - Added PrintFunction action to FunctionDefinition rule
* ver 2.0 : 01 Jun 11 by Jim Fawcett
* - Major revisions to begin building a strong code analyzer
* ver 1.1 : 01 Feb 06 by Jim Fawcett
* - cosmetic changes to ConfigureParser.cpp
* ver 1.0 : 12 Jan 06 by Jim Fawcett
* - first release

*/

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "RulesAndActions.h"

///////////////////////////////////////////////////////////////
// build parser that writes its output to console

class ConfigParseToConsole : IBuilder
{
public:
	ConfigParseToConsole() {};
	~ConfigParseToConsole();
	bool Attach(const std::string& name, bool isFile = true);
	Parser* Build();
	Repository* getRepo();
	Scanner::Toker* getToker(); 
private:
	// Builder must hold onto all the pieces
	std::ifstream* pIn;
	Scanner::Toker* pToker;
	Scanner::SemiExp* pSemi;
	Parser* pParser;
	Repository* pRepo;

	// add Rules and Actions

	EndOfScope* pEndOfScope;
	HandlePop* pHandlePop;
	FunctionDefinition* pFunctionDefinition;
	PushFunction* pPushFunction;
	PreprocStatement* pPreprocStatement;

	ClassDefinition* pClassDefinition;
	PushClass* pPushClass;
	StructDefinition* pStructDefinition;
	PushStruct* pPushStruct;
	NamespaceDefinition* pNamespaceDefinition;
	PushNamespace* pPushNamespace;
	EnumerationDefinition* pEnumerationDefinition;
	PushEnumeration* pPushEnumeration;
	TypedefDefinition* pTypedefDefinition;
	PushTypedef* pPushTypedef;
	AliasesDefinition* pAliasesDefinition;
	PushAliases* pPushAliases;

	CatchDefinition* pCatchDefinition;
	PushCatch* pPushCatch;
	TryDefinition* pTryDefinition;
	PushTry* pPushTry;

	SpecialKeyWordsDefinition* pSpecialKeyWordsDefinition;
	PushSpecialKeyWords* pPushSpecialKeyWords;
	AnonymousDefinition* pAnonymousDefinition;
	PushAnonymous* pPushAnonymous;
	InitializationDefinition* pInitializationDefinition;
	PushInitialization* pPushInitialization;


	void addNewRules();
	void addSpecialNewRules();

	// prohibit copies and assignments
	ConfigParseToConsole(const ConfigParseToConsole&) = delete;
	ConfigParseToConsole& operator=(const ConfigParseToConsole&) = delete;
};

//----< get repository >---------------------------------------------
inline Repository* ConfigParseToConsole::getRepo() { return pRepo; };

//----< get toker >--------------------------------------------------
inline Scanner::Toker* ConfigParseToConsole::getToker() { return pToker; };

#endif
