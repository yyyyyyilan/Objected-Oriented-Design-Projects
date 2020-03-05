
This a parser-based code analyzer. 

------------------
Packages included:
------------------
- Tokenizer: extracts tokens from a stream of characters.
	 Tokenizer.h  Tokenizer.cpp
- SemiExpression: groups tokens into sets depending on special terminating characters,
		  and each contain all the infomation needed to analyze some grammatical
		  construct.
	itokcollection.h  SemiExp.h  SemiExp.cpp
- ScopeStack: contains one class ScopeStack<element>. Element is a specific type to hole 
	      information need to stack.
	ScopeStack.h  ScopeStack.cpp
- AbstractSyntaxTree: builds an abstract syntax tree and provides interface to add a scope
		      node to the AST.
	MNode.h  MNode.cpp  MTree.h  MTree.cpp
- RulesAndActions: contains definitions of rules and corresponding actions used by Parser.
		   It can be modified for each type of code analysis.
	RulesAndActions.h  RulesAndActions.cpp
- Parser: uses rules to identify syntactical constrcuts and build an AST to hold parsing 
	  information. It contains instances of Rule classes and passes token collections 
	  it accquires from ITokenCollection to each rule.
	ConfigureParser.h  ConfigureParser.cpp  Parser.h  Parser.cpp
- Analyzer: evaluates and displays information that is stored in an AST.
	MetricsAnalysis.h  MetricsAnalysis.cpp
- FileMgr: finds files matching specific patterns on a specific path
	FileMgr.h  FileMgr.cpp  FileSystem.h  FileSystem.cpp  DataStore.h  DataStore.cpp
- MetricsExecutive: defined the main() function as the entrance of the main project. It 
	 	    collects metrics for all packages
	MetricsExecutive.h  MetricsExecutive.cpp  
- Untilities: helper functions
	ParserUtilities.h  ParserUtilities.cpp
- Test: contains all test files

-------------
Build Command
-------------
devenv Project_2.sln /rebuild debug

-----------
Run Command
-----------
.\Debug\MetricsExecutive.exe ./Test *.h *.cpp

----------
Reference
----------
http://ecs.syr.edu/faculty/fawcett/handouts/CSE687/Lectures/Project2-S2016.html


This is declaration of Requirement Four.
Parser is a container for Rules and Actions.
--------------------------------------------
Rules:
     FunctionDefinition: detects begining of function scope
     PreprocStatement: detects preprocessor
     ClassDefinition: detects begining of class scope
     StructDefinition: detects begining of struct scope	
     EnumDefinition: detects begining of enum scope
     NamespaceDeifinition: detects begining of namespace scope
     SpecialKeyWordDefinition: detects begining of for, while, if-else,do-while scopes
     CatchDefinition: detects catch block scope
     TryDefinition: detects try block scope
     AnonymousDefnition:detects begining of anaonymous scope
     InitializationDefinition: detects begining of initialization scopes
     EndOfScope: detects end of scope

Actions:
     HandlePop to EndOfScope rule
     PrintPreproc to PreprocStatement rule
     PushFunction, PrintFunction,PrettyPrintFunction to FunctionDefinition rule
     PushClass, PrintClass to ClassDefinition rule
     PushStruct, PrintStruct to StructDefinition rule
     PushNamespace, PrintNamespace to NamespaceDefinition rule
     PushEnumeration, PrintEnumeration to EnumerationDefinition rule
     PushCatch, PrintCatch to CatchDefinition rule
     PushTry, PrintTry to TryDefinition rule
     PushSpecialKeyWordDefinition, PrintSpecialKeyWordDefinition to SpecialKeyWordDefinition rule
     PushAnonymous, PrintAnonymous to AnonymousDefinition rule
     PushInitialization to InitializationDefinition rule


This is declaration of Requirement Five.
AbstractSyntaxTree package: 
	The result is shown below by using six files in different folder.


This is demo of Requirement Six.
Analyzer package
-----------------
 It evaluates specified files infotmation that is stored in each AST, including adding scope noded to a AST and methods 
calculating the size and complexity of functions in each of a set of specified packages. 
The default start linenumber and end linenumber of root, which is Global Namespace are both 0.
./Test/test1/RequirementSeven1.h has two more scope nodes than ./Test/test1/RequirementSeven2.h, which is shown below.


This is file declaration used by Requirment Seven, Eight and Nine demo.
test files descriptions
-----------------------

RequirementFive1.h: 
	tests namespace scope, struct scope, class scope, enum scope, function scope
RequirementFive12.cpp: 
	tests if-else scope, for scope, while scope, do-while scope, catch scope, try scope
test1/RequirementSeven1.h and test1/RequirementSeven2.h:
	tests filemgr searching files subfolder at a specified path
	tests interfaces for adding scope node to AST (RequirementSeven2.h has two more scopes than RequirementSeven1.h)
test1/test1_sub/TestGrandchildFolder.cpp:
	tests filemgr searching files in sub-subfolder
test2/RequirementSeven.h:
	tests filemgr searing file in different subfolders

The parsing result of each file shows function summary information first and then the structure of AST.
