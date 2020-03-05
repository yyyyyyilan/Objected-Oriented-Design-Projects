/////////////////////////////////////////////////////////////////////////////////////////
// ParallelDependencyAnalysis.cpp - Evaluates the dependency graph for all packages    //
// ver 1.0                                                                             //
// Application: Evaluate dependency graph                                              //
//              CSE687 - Object Oriented Design                                        //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			                   //
// Author:		Yilan Li, Syracuse University, (315)382-9302				           //
//				yli41@syr.edu												           //
/////////////////////////////////////////////////////////////////////////////////////////

#include "ParallelDependencyAnalysis.h"
#include <iostream>
using Table = std::map<std::string, std::vector<std::string>>;
using SymbolTable = std::map<std::string, std::vector<TypeInfo>>;
using DependencyTable = std::map<std::string, std::set<std::string>>;

//----< ParallelDependencyAnalysis constructor >---------------------

ParallelDependencyAnalysis::ParallelDependencyAnalysis(size_t threadnum):thread(threadnum)
{
	mergedSymbolTable = new SymbolTable;
	mergedDependencyTable = new DependencyTable;
}
//----< ParallelDependencyAnalysis destructor >----------------------

ParallelDependencyAnalysis::~ParallelDependencyAnalysis()
{}
//----< Start parallel dependency analysis >-------------------------

void ParallelDependencyAnalysis::start(size_t pn, char* arg[])
{
	std::string p = arg[1];
	FileMgr fileMgr(p, datastore);
	for (size_t i = 2; i < pn; i++)
		fileMgr.addPattern(arg[i]);
	fileMgr.search();
	typeblockingQueue = fileMgr.createQueue(datastore);
	depblockingQueue = typeblockingQueue;
}
//----< do analysis >------------------------------------------------

void ParallelDependencyAnalysis::analysis()
{
	doTypeAnalysis();
	std::cout << "\nMerged Symbol table is shown below." << std::endl;
	std::cout << "------------------------------------" << std::endl;
	showTypeAnalysis();
	doDependencyAnalysis();
	std::cout << "\nMerged dependency table is shown below." << std::endl;
	std::cout << "---------------------------------------" << std::endl;
	showDependencyAnalysis();
}
//----< executes parallel type analysis >----------------------------

void ParallelDependencyAnalysis::doTypeAnalysis()
{
	Task<SymbolTable*> typetask(thread);
	std::function<SymbolTable*()> typework = [&]() {
		return typeAnalysis();
	};
	BlockingQueue<std::string>* filequeue = getTypeBlockingQueue();
	size_t filenum = filequeue->size();
	typetask.addwork(&typework,filenum);
}
//----help function to do type analysis >--------------------------------------

SymbolTable* ParallelDependencyAnalysis::typeAnalysis()
{
		BlockingQueue<std::string>* bq = getTypeBlockingQueue();
		std::string f = bq->deQ();
		std::lock_guard<std::mutex> guard(ioLock);
		TypeAnalysis typeanalysis(f);
		Repository r = typeanalysis.parse(f);
		std::cout << "--Type Analysis on thread " << std::this_thread::get_id() << "\n";
		Table  temp = typeanalysis.getTable();
		mergesymbolTable(f, &temp);
		return mergedSymbolTable;
}
//----< merge partial symbol table to symbol table >---------------------------

SymbolTable* ParallelDependencyAnalysis::mergesymbolTable(std::string f, Table* t)
{
		Table* temptable = t;
		std::vector<TypeInfo> tempstruct;
		for (Table::iterator it1 = temptable->begin(); it1 != temptable->end(); ++it1)
		{
			TypeInfo _struct;
			_struct._name = it1->first;
			_struct._file = f;
			_struct._namespace = it1->second[1];
			_struct._type = it1->second[0];
			tempstruct.push_back(_struct);
		}
		mergedSymbolTable->insert(std::pair<std::string, std::vector<TypeInfo>>(f, tempstruct));
		return mergedSymbolTable;
}
//----< executes parallel dependency analysis >----------------------

void ParallelDependencyAnalysis::doDependencyAnalysis()
{
	Task<DependencyTable*> dependencytask(thread);
	std::function<DependencyTable*()> dependencywork = [&]() {
		return dependencyAnalysis();
	};
	BlockingQueue<std::string>* filequeue = getDepBlockingQueue();
	size_t filenum = filequeue->size();
	dependencytask.addwork(&dependencywork, filenum);
}
//----help function to do dependency analysis >--------------------------------

DependencyTable* ParallelDependencyAnalysis::dependencyAnalysis()
{
	BlockingQueue<std::string>* bq = getDepBlockingQueue();
	std::string f = bq->deQ();
	std::lock_guard<std::mutex> guard(ioLock);
	DependencyAnalysis<TypeInfo> dependencyanalysis(f);
	SymbolTable* s = getSymbolTable();
	dependencyanalysis.DependencyTask(f,s);
	std::cout << "\n--Dependency analysis on thread " << std::this_thread::get_id() << "\n\n";
	DependencyTable  temp = dependencyanalysis.getTable();
	mergedependencyTable(&temp);
	return mergedDependencyTable;
}
//----< merge partial dependency results to merged results >-------------------

DependencyTable ParallelDependencyAnalysis::mergedependencyTable(DependencyTable* pt)
{
		for (DependencyTable::iterator it = pt->begin();
		it != pt->end(); ++it)
		{
			std::string first = it->first;
			std::set<std::string> second = it->second;
			mergedDependencyTable->insert(std::pair<std::string, std::set<std::string>>(first, second));
		}
		return *mergedDependencyTable;
}
//----< display merged symbol table >--------------------------------

void ParallelDependencyAnalysis::showTypeAnalysis()
{
		SymbolTable* s = getSymbolTable();
		for (SymbolTable::iterator it = s->begin();
		it!= s->end();++it)
		{
			std::string first = it->first;
			std::cout << "\n[  FILE  ] " << first << std::endl;
			std::vector<TypeInfo> second = it -> second;
			typeshow(second);
		}
}
//----< helper function to display type and function information >-------------

void ParallelDependencyAnalysis::typeshow(std::vector<TypeInfo> t)
{
	for (size_t i = 0; i < t.size(); i++)
	{
		std::cout << "\t [ NAME: " << t[i]._name << " \n";
		std::cout << "\t   TYPE: " << t[i]._type << " \n";
		std::cout << "\t   NAMESPACE: " << t[i]._namespace << " ] \n";
	}
	
}
//----< display merged dependency results >--------------------------

void ParallelDependencyAnalysis::showDependencyAnalysis()
{
		DependencyTable* d = getDepTable();
		for (DependencyTable::iterator it = d->begin();
		it != d->end(); ++it)
		{
			std::cout << "\n[ FILE ] : ";
			std::cout << it->first << "\n";
			std::cout << "  Dependent files list: \n";
			std::set<std::string> tn = it->second;
			while (!tn.empty())
			{
				std::cout << "\t\t--" << *tn.begin() << "\n";
				tn.erase(tn.begin());
			}
			std::cout << "\n";
		}
}

//----< test stub >--------------------------------------------------

#ifdef TEST_PARALLELDEPENDENCYANALYSIS

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing ParallelDependencyAnalysis module\n "
		<< std::string(32, '=') << std::endl;

	if (argc < 3)
	{
		std::cout
			<< "\n  please enter name of file or the pattern of file to process on command line\n\n";
		return 1;
	}
	try {
		size_t threadnumber = 3;
		Task<SymbolTable*> task(threadnumber);
		ParallelDependencyAnalysis paralleldependencyanalysis(threadnumber);
		paralleldependencyanalysis.start(argc, argv);
		paralleldependencyanalysis.analysis();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}

	return 0;
}

#endif