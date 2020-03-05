/////////////////////////////////////////////////////////////////////////////////
// ParallelDependencyExecutive.cpp - conducts dependency analysis              //
// ver 1.0                                                                     //
// Application: Parallel Dependency Analysis                                   //
//              CSE687 - Object Oriented Design                                //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			           //
// Author:		Yilan Li, Syracuse University, (315)382-9302				   //
//				yli41@syr.edu												   //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <mutex>
#include "ParallelDependencyExecutive.h"

using Table = std::map<std::string, std::vector<std::string>>;
using SymbolTable = std::map<std::string, std::vector<TypeInfo>>;
using DependencyTable = std::map<std::string, std::set<std::string>>;

//----< construct ParallelDependencyExecutive >--------------------------------

ParallelDependencyExecutive::ParallelDependencyExecutive()
{}
//----< ParallelDependencyExecutive destructor >-------------------------------

ParallelDependencyExecutive::~ParallelDependencyExecutive()
{}
//----< test function for requirement one >------------------------------------

void ParallelDependencyExecutive::RequirementOne()
{
	std::cout << " \nThis is declaration of Requirement ONE." << "\n";
	std::cout << " ---------------------------------------" << "\n";
	std::cout << " Project #3- Parallel Dependency Analysis" << std::endl;
	std::cout << " Yilan Li  Syracuse University" << std::endl;
	std::cout << " yli41@syr.edu" << std::endl;
	std::cout << " Platform: Dell Inspiron 13-7352, Windows 10 Education" << std::endl;
	std::cout << " Language: C++, Visual Studio 2015 Community\n" << std::endl;
}
//----< test function for requirement two and three >------------------------------------

void ParallelDependencyExecutive::RequirementTwoandThree(std::string p)
{
	std::string line;
	std::ifstream myfile;
	myfile.open(p);
	while (std::getline(myfile, line))
	{
		std::cout << line << "\n";
	}
	myfile.close();
	std::cout << "\n\n";
}
//----< test function for requirement four >------------------------------------

void ParallelDependencyExecutive::RequirementFour()
{
	std::cout << "////////////////////////////////\n";
	std::cout << "// Demo for Requirement FOUR. //" << std::endl;
	std::cout << "////////////////////////////////\n";
	std::cout << "\n Testing threalpool package, creating 3 threads.";
	ThreadPool<std::string> threadpool(3);
	threadpool.start();
	std::function<std::string()> work = [&]() {
		std::lock_guard<std::mutex> guard(ioLock);
		std::cout << "\n ---working on thread " << std::this_thread::get_id();
		return " --- testing thread";
	};
	for (size_t i = 0; i < 6;i++)
		threadpool.doWork(&work);
	for (size_t i = 0; i < 6; i++)
	{
		std::cout << "\n  " << threadpool.result();
	}
	threadpool.wait();
	std::cout << "\n\n";
}
//----< test function for requirement five >------------------------------------

void ParallelDependencyExecutive::RequirementFive()
{
	std::cout << "////////////////////////////////\n";
	std::cout << "// Demo for Requirement FIVE. //" << std::endl;
	std::cout << "////////////////////////////////\n";
	std::cout << "\n Testing task package executes a callable object on ThreadPool thread." ;
	size_t thread = 3;
	size_t worknum = 5;
	std::cout << "Creating " << thread << " threads to do " << worknum << "work.\n";
	Task<std::string> tasktest(thread);
	std::function<std::string()> work = [&]() {
		static int count = 1;
		std::lock_guard<std::mutex> guard(ioLock);
		std::cout << "\n ---working on thread " << std::this_thread::get_id()<<" doing work "<< count;
		count = count + 1;
		return "\t doing new work";
	};
	tasktest.addwork(&work, worknum);
	std::cout << "\n\n";
}
//----< test function for requirement six >------------------------------------

void ParallelDependencyExecutive::RequirementSix()
{
	std::string file = "./Test/RequirementSix.cpp";
	std::cout << "///////////////////////////////\n";
	std::cout << "// Demo for Requirement SIX. //" << std::endl;
	std::cout << "///////////////////////////////\n";
	std::cout << "\n --- Processing type analysis on file: " << file;
	TypeAnalysis typeAnalysis(file);
	Repository repository = typeAnalysis.parse(file);
	std::cout << " --- Partial Symbol Table" << "\n";
	typeAnalysis.displayTable();
	std::cout << "\n\n";
}
//----< test function for requirement seven >------------------------------------

void ParallelDependencyExecutive::RequirementSeven()
{
	std::cout << "//////////////////////////////////\n";
	std::cout << "// Demo for Requirement SEVEN. //" << std::endl;
	std::cout << "/////////////////////////////////\n";
	std::string firstfile = "./Test/RequirementSeven.cpp";
	std::string secondfile = "./Test/RequirementSeven.h";
	std::cout << "Demo files: " << std::endl;
	std::cout << "\t" << firstfile;
	std::cout << "\n\t" << secondfile;
	TypeAnalysis typeAnalysis(firstfile);
	Repository repository = typeAnalysis.parse(firstfile);
	Table symboltable = typeAnalysis.getTable();
	std::vector<TypeInfo> tempstruct;
	for (Table::iterator it1 = symboltable.begin(); it1 != symboltable.end(); ++it1)
	{
		TypeInfo _struct;
		_struct._name = it1->first;
		_struct._file = firstfile;
		_struct._namespace = it1->second[1];
		_struct._type = it1->second[0];
		tempstruct.push_back(_struct);
	}
	std::map<std::string, std::vector<TypeInfo>> symbol;
	symbol.insert(std::pair<std::string, std::vector<TypeInfo>>(firstfile, tempstruct));
	DependencyAnalysis<TypeInfo> dependencyanalysis(secondfile);
	dependencyanalysis.DependencyTask(secondfile, &symbol);
	dependencyanalysis.displayTable();
	std::cout << "\n";
}
//----< test function for requirement eight and nine >--------------------------

void ParallelDependencyExecutive::RequirementEightandNine(int argc, char* argv[])
{
	std::cout << "//////////////////////////////////////////\n";
	std::cout << "// Demo for Requirement EIGHT and NINE. //" << std::endl;
	std::cout << "//////////////////////////////////////////\n";
	size_t threadnumber = 4;
	std::cout << "Creating " << threadnumber << " threads to do analysis." << std::endl;
	Task<SymbolTable*> task(threadnumber);
	ParallelDependencyAnalysis paralleldependencyanalysis(threadnumber);
	paralleldependencyanalysis.start(argc, argv);
	paralleldependencyanalysis.analysis();
	std::cout << "\n";
}
//----< test stub >------------------------------------------------------------

//#define TEST_PARALLELDEPENDENCYEXECUTIVE
#ifdef TEST_PARALLELDEPENDENCYEXECUTIVE

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout
			<< "\n  please enter name of file or the pattern of file to process on command line\n\n";
		return 1;
	}
	try {
		size_t threadnumber = 3;
		ParallelDependencyExecutive paralledepExecutive;
		paralledepExecutive.RequirementOne();
		paralledepExecutive.RequirementTwoandThree("./README.txt");
		paralledepExecutive.RequirementFour();
		paralledepExecutive.RequirementFive(); 
		paralledepExecutive.RequirementSix();
		paralledepExecutive.RequirementSeven();
		paralledepExecutive.RequirementEightandNine(argc, argv);
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n";
	}

	std::cout << "\n\n  main thread id = " << std::this_thread::get_id() << "\n";
	std::cout << "Parallel Dependency Analysis Project Demo Completed.\n" << std::endl;
	return 0;
}

#endif


