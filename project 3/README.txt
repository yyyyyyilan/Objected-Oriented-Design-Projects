This is a Parallel Dependency Analyzer.


This is declaration of Requirement TWO.
-----------------------------------------
No use of the C language I/O, e.g. printf, scanf, etc, nor the C memory management,
e.g. calloc, malloc, or free.


This is declaration of Requirement THREE.
-----------------------------------------
------------------
Packages included:
------------------
- Tokenizer: Extracts tokens from a stream of characters.
	     Tokenizer.h  Tokenizer.cpp
- SemiExpression: Groups tokens into sets depending on special terminating characters,
	     and each contain all the infomation needed to analyze some grammatical 
	     construct.
	     itokcollection.h  SemiExp.h  SemiExp.cpp
- ScopeStack: Contains one class ScopeStack<element>. Element is a specific type to hole 
	     information need to stack.
	     ScopeStack.h  ScopeStack.cpp
- RulesAndActions: contains definitions of rules and corresponding actions used by Parser.
	     These rules and corresponding actions are built to detect type definitions:
	     classes, structs, enums, typedefs, and aliases. It can be modified for each 
             type of code analysis.
	     RulesAndActions.h  RulesAndActions.cpp
- Parser: Uses rules to identify type definitions and build an map to hold parsing 
	  information. It contains instances of Rule classes and passes token collections 
	  it accquires from ITokenCollection to each rule.
	  ConfigureParser.h  ConfigureParser.cpp  Parser.h  Parser.cpp
- ThreadPool: Provides facilities to enqueue work items and execute them asynchronously.
	      And each work item is a callable object which is implemented with C++ lambda 
	      bound std::function objects.
 	      ThreadPool.h ThreadPool.cpp ThreadUtilities.h ThreadUtilities.cpp
- BlockingQueue: Contains a thread-safe class: BlockingQueue<T> to support sending messages 
	         between threads.
- Task: Executes a specified callable object on a ThreadPool thread. It uses the services which
	are provided by the ThreadPool package.
        Task.h Task.cpp 
- TypeAnalysis: Find all the types deefined in each of a collection of c++ source files. It builds
		rules to detect definitions: classes, structs, enums, typedefs and aliases, and 
		capture their fully qualified names and files where they are defined.
		TypeAnalysis.h TypeAnalysis.cpp 
- DependencyAnalysis: Finds all other files from the collection on which they depend. This package
		      doesn't record dependencies of a file outside the file set. 
		      DependencyAnalysis.h DependencyAnalysis.cpp
- ParallelDependencyAnalysis: Evaluates the dependency graph for all the packages in a specified 
 			      file collection. Analysis of each file runs asynchronously. This 
			      package uses the facilities of Task class. 
 			      ParallelDependencyAnalysis.h ParallelDependencyAnalysis.cpp
- ParallelDependencyExecutive: Defined the main() function as the entrance of the main project.
			       ParallelDependencyExecutive.h ParallelDependencyExecutive.cpp
- Untilities: Some helper functions
	      ParserUtilities.h  ParserUtilities.cpp TokenizerUtilities.h TokenizerUtilities.cpp
- Test: contains all test files that demo needs
	RequirementSix.cpp			 // used for Reuirement SIX demo
	RequirementSeven.cpp RequirementSeven.h  // used for Requirement Seven demo
	/FinalDemo				 // files used to for Requirement EIGHT&NINE


-------------
Build Command
-------------
devenv Project_3.sln /rebuild debug

-----------
Run Command
-----------
.\Debug\ParallelDependencyExecutive.exe ./Test/FinalDemo *.h *.cpp

----------
Reference
----------
http://ecs.syr.edu/faculty/fawcett/handouts/CSE687/Lectures/Project3-S2016.htm



This is declaration of Requirement FOUR.
-----------------------------------------
Four threads are created by calling ThreadPool::start() function. And these three threads 
executes enqueued work items asynchronously. The thread id of these four threads are shown 
in the demo result when they executes enqueued work items.


This is declaration of Requirement FIVE.
-----------------------------------------
Task class executes a callable object on a ThreadPool thread. Every work item is defined as 
C++ lambda bound std::function objects. 
In Task.cpp file: void addwork(std::function<type()> work, size_t worknum);


This is declaration of Requirement SIX.
-----------------------------------------
The result of symbol table is shown in the demo. For each file, it identifies the type, name 
and namespace of type definitions.


This is declaration of Requirement SEVEN.
-----------------------------------------
The result of DependencyAnalysis package is shown in the demo result. Each file has a list of
dependent files' name in a specified file collection, which is Test folder. 


This is declaration of Requirement EIGHT.
-----------------------------------------
ParallelDependencyAnalysis package executes in two passes. In the first pass, using TypeAnalysis
to find all type defninitions. In the second pass, using DependencyAnalysis to find all dependencies
between files. In both of these two passes, the analysises run asynchronously. Creating mulptiple 
threads for type analysis, then in the second pass creating new mutiplt threads for dependency
analysis. 

This is declaration of Requirement NINE.
-----------------------------------------
This demo collects all of the files matching .h and .cpp patterns in Test directory. The demo 
result is shown below.