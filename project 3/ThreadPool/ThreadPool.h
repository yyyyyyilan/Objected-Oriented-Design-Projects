#ifndef THREADPOOL_H
#define THREADPOOL_H
///////////////////////////////////////////////////////////////////////
// ThreadPool.h - child threads processe enqueued work items         //
// ver 1.1                                                           //
// Language:  C++, Visual Studio 2015 Community                      //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			 //
// Application: Object Oriented Design, Spring 2016                  //
// Author:		Yilan Li, Syracuse University, (315)382-9302         //
//				yli41@syr.edu									     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module defines a ThreadPool package. It Provides facilities to 
* enqueue work items and execute them asynchronously.
* Public Interface:
* =================
* void start();								// start child threads that dequeu work items 
* void doWork(WorkItem<Result>* pWi);		// enqueue work item
* Result result();							// retrieve results with blocking call
* void wait();								// wait for child thread to terminate
*
* Build Process:
* ==============
* Required files
* - ThreadPool.h ThreadPool.cpp BlockingQueue.h BlockingQueue.cpp ThreadUtilities.h
*   ThreadUtilities.cpp
* Build commands
* - devenv Project_3.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.1 : 24 March 2016 by Yilan Li
* - modified to multiple child threads processe work items equeued by main thread 
* ver 1.0 : by Jim Fawcett
* - first release
*/
#include <thread>
#include <functional>
#include <vector>
#include "../BlockingQueue/BlockingQueue.h"

template<typename Result>
using WorkItem = std::function<Result()>;

///////////////////////////////////////////////////////////////////////
// class to process work items
// - each work item is processed sequentially on a single child thread

template<typename Result>
class ThreadPool
{
public:
	ThreadPool();
	ThreadPool(size_t n);
	void start();
	void doWork(WorkItem<Result>* pWi);
	Result result();
	void wait();
	~ThreadPool();
private:
	std::vector<std::thread*> _pThread;
	BlockingQueue<WorkItem<Result>*> _workItemQueue;
	BlockingQueue<Result> _resultsQueue;
	size_t threadNumber;
};
//----< void constructor >---------------------------------------------
template<typename Result>
ThreadPool<Result>::ThreadPool()
{}

//----< constructor for multiple threads >-----------------------------

template<typename Result>
ThreadPool<Result>::ThreadPool(size_t n)
{
	threadNumber = n;
}
//----< wait for child thread to terminate >---------------------------

template<typename Result>
void ThreadPool<Result>::wait()
{
	for (size_t i = 0; i < threadNumber; ++i)
	{
		doWork(nullptr);
	}
	for (auto& thread : _pThread)
	{
		if (thread->joinable())
			thread->join();
	}		
}
//----< enqueue work item >--------------------------------------------

template<typename Result>
void ThreadPool<Result>::doWork(WorkItem<Result>* pWi)
{
	_workItemQueue.enQ(pWi);
}
//----< retrieve results with blocking call >--------------------------

template<typename Result>
Result ThreadPool<Result>::result()
{
	return _resultsQueue.deQ();
}
//----< start child thread that dequeus work items >-------------------

template<typename Result>
void ThreadPool<Result>::start()
{
	std::function<void()> threadProc =
		[&]() {
		while (true)
		{
			WorkItem<Result>* pWi = _workItemQueue.deQ();
			if (pWi == nullptr)
			{
				return;
			}
			Result result = (*pWi)();
			_resultsQueue.enQ(result);
		}
	};
	
	for (size_t i = 0; i < threadNumber; ++i)
	{
		std::thread* newthread = new std::thread(threadProc);
		_pThread.push_back(newthread);
	}
}
//----< clean up heap >----------------------------------------------

template<typename Result>
ThreadPool<Result>::~ThreadPool()
{
}

#endif
