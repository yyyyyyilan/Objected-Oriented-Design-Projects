///////////////////////////////////////////////////////////////////////
// ThreadPool.cpp - child threads processe enqueued work items       //
// ver 1.1                                                           //
// Language:  C++, Visual Studio 2015 Community                      //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			 //
// Application: Object Oriented Design, Spring 2016                  //
// Author:		Yilan Li, Syracuse University, (315)382-9302         //
//				yli41@syr.edu									     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////


//#define TEST_THREADPOOL
#ifdef TEST_THREADPOOL

#include <string>
#include <iostream>
#include <conio.h>
#include "ThreadPool.h"
#include "ThreadUtilities.h"

//----< demonstrate ProcessWorkItem class >----------------------------

using Util = ThreadUtilities::StringHelper;
using WorkResult = std::string;

int main()
{
	Util::Title("Enqueued Work Items");

	std::cout << "\n  main thread id = " << std::this_thread::get_id();

	ThreadPool<WorkResult> threadpool(3);
	threadpool.start();

	// define 1st work item

	WorkItem<WorkResult> wi1 = []() {
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		return "Hello from wi1";
	};
	threadpool.doWork(&wi1);

	// define 2nd work item

	WorkItem<WorkResult> wi2 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		return "wi2 result = " + ThreadUtilities::Converter<size_t>::toString(sum);
	};
	threadpool.doWork(&wi2);

	// define 3rd work item

	WorkItem<WorkResult> wi3 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		return "wi3 comes";
	};
	threadpool.doWork(&wi3);

	// define 4th work item

	WorkItem<WorkResult> wi4 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		return "wi4 comes";
	};
	threadpool.doWork(&wi4);

	
	// the following calls to result() block until results are enqueued

	std::cout << "\n  " << threadpool.result();
	std::cout << "\n  " << threadpool.result();
	std::cout << "\n  " << threadpool.result();
	std::cout << "\n  " << threadpool.result();
	//threadpool.doWork(nullptr);

	// wait for child thread to complete
	threadpool.wait();
	std::cout << "\n\n";
}

#endif