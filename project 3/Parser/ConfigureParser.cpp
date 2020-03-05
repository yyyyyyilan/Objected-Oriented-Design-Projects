///////////////////////////////////////////////////////////////////////
// ConfigureParser.cpp - builds and configures parsers               //
// ver 2.7                                                           //
// Language:  C++, Visual Studio 2015 Community                      //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			 //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:		Yilan Li, Syracuse University, (315)382-9302         //
//				yli41@syr.edu									     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
#include <fstream>
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"
#include "RulesAndActions.h"
#include "ConfigureParser.h"

using namespace Scanner;

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
	// when Builder goes out of scope, everything must be deallocated

	delete pHandlePop;
	delete pEndOfScope;
	delete pPreprocStatement;
	delete pPushFunction;
	delete pFunctionDefinition;
	delete pPushClass;
	delete pClassDefinition;
	delete pPushStruct;
	delete pStructDefinition;
	delete pPushNamespace;
	delete pNamespaceDefinition;
	delete pPushEnumeration;
	delete pEnumerationDefinition;
	delete pPushCatch;
	delete pCatchDefinition;
	delete pPushTry;
	delete pTryDefinition;
	delete pPushSpecialKeyWords;
	delete pSpecialKeyWordsDefinition;
	delete pPushAnonymous;
	delete pAnonymousDefinition;
	delete pInitializationDefinition;
	delete pPushInitialization;
	delete pTypedefDefinition;
	delete pPushTypedef;
	delete pAliasesDefinition;
	delete pPushAliases;

	delete pRepo;
	delete pParser;
	delete pSemi;
	delete pToker;
	pIn->close();
	delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
	if (pToker == 0)
		return false;
	pIn = new std::ifstream(name);
	if (!pIn->good())
		return false;
	return pToker->attach(pIn);
}
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParseToConsole::Build()
{
	try
	{
		// add Parser's main parts
		pToker = new Toker;
		pToker->returnComments(false);
		pSemi = new SemiExp(pToker);
		pParser = new Parser(pSemi);
		pRepo = new Repository(pToker);

		// configure to manage scope
		// these must come first - they return true on match
		// so rule checking continues

		addNewRules();
		addSpecialNewRules();

		// configure to detect and act on end of scope
		pEndOfScope = new EndOfScope();
		pHandlePop = new HandlePop(pRepo);
		pEndOfScope->addAction(pHandlePop);
		pParser->addRule(pEndOfScope);
		return pParser;
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
		return 0;
	}
	//pToker->setInitial(true, true);
}

//---- < function to configure to detect and act on new rules >----

void ConfigParseToConsole::addNewRules()
{
	//configure to detect and act on preprocessor definitions
	pPreprocStatement = new PreprocStatement;
	pParser->addRule(pPreprocStatement);

	// configure to detect and act on namespace definitions
	pNamespaceDefinition = new NamespaceDefinition;
	pPushNamespace = new PushNamespace(pRepo);
	pNamespaceDefinition->addAction(pPushNamespace);
	pParser->addRule(pNamespaceDefinition);

	// configure to detect and act on strcut definitions
	pStructDefinition = new StructDefinition;
	pPushStruct = new PushStruct(pRepo);
	pStructDefinition->addAction(pPushStruct);
	pParser->addRule(pStructDefinition);

	// configure to detect and act on class definitions
	pClassDefinition = new ClassDefinition;
	pPushClass = new PushClass(pRepo);
	pClassDefinition->addAction(pPushClass);
	pParser->addRule(pClassDefinition);

	//configure to detect and act on function definitions
	pFunctionDefinition = new FunctionDefinition;
	pPushFunction = new PushFunction(pRepo);  
	pFunctionDefinition->addAction(pPushFunction);
	pParser->addRule(pFunctionDefinition);

	// configure to detect and act on enumeration definitions
	pEnumerationDefinition = new EnumerationDefinition;
	pPushEnumeration = new PushEnumeration(pRepo);  
	pEnumerationDefinition->addAction(pPushEnumeration);
	pParser->addRule(pEnumerationDefinition);	

	// configure to detect and act on typedef definitions
	pTypedefDefinition = new TypedefDefinition;
	pPushTypedef = new PushTypedef(pRepo);
	pTypedefDefinition->addAction(pPushTypedef);
	pParser->addRule(pTypedefDefinition);

	//configure to detect and act on alises definitions
	pAliasesDefinition = new AliasesDefinition;
	pPushAliases = new PushAliases(pRepo);
	pAliasesDefinition->addAction(pPushAliases);
	pParser->addRule(pAliasesDefinition);
}

void ConfigParseToConsole::addSpecialNewRules()
{
	//configure to detect and act on catch definitions
	pCatchDefinition = new CatchDefinition;
	pPushCatch = new PushCatch(pRepo);
	pCatchDefinition->addAction(pPushCatch);
	pParser->addRule(pCatchDefinition);

	//configure to detect and act on try definitions
	pTryDefinition = new TryDefinition;
	pPushTry = new PushTry(pRepo);
	pTryDefinition->addAction(pPushTry);
	pParser->addRule(pTryDefinition);

	// configure to detect and act on special keywords definitions
	pSpecialKeyWordsDefinition = new SpecialKeyWordsDefinition;
	pPushSpecialKeyWords = new PushSpecialKeyWords(pRepo);
	pSpecialKeyWordsDefinition->addAction(pPushSpecialKeyWords);
	pParser->addRule(pSpecialKeyWordsDefinition);

	//configure to detect anonymous scope
	pAnonymousDefinition = new AnonymousDefinition;
	pPushAnonymous = new PushAnonymous(pRepo);
	pAnonymousDefinition->addAction(pPushAnonymous);
	pParser->addRule(pAnonymousDefinition);

	//configure to detect initialization scope
	pInitializationDefinition = new InitializationDefinition;
	pPushInitialization = new PushInitialization(pRepo);
	pInitializationDefinition->addAction(pPushInitialization);
	pParser->addRule(pInitializationDefinition);
}
//----< test stub >------------------------------------------------

#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing ConfigureParser module\n "
		<< std::string(32, '=') << std::endl;

	// collecting tokens from files, named on the command line

	if (argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}

	for (int i = 1; i<argc; ++i)
	{
		std::cout << "\n  Processing file " << argv[i];
		std::cout << "\n  " << std::string(16 + strlen(argv[i]), '-');

		ConfigParseToConsole configure;
		Parser* pParser = configure.Build();
		try
		{
			if (pParser)
			{
				if (!configure.Attach(argv[i]))
				{
					std::cout << "\n  could not open file " << argv[i] << std::endl;
					continue;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
				return 1;
			}
			// now that parser is built, use it
			while (pParser->next())
				pParser->parse();
			std::cout << "\n\n";
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
		std::cout << "\n\n";
	}
}

#endif
