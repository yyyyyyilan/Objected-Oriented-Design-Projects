#ifndef TASK_H
#define TASK_H
/////////////////////////////////////////////////////////////////////////////////
// Task.h - execute a specified callable object on a ThreadPool thread         //
// ver 1.1                                                                     //
// Application: Evaluate specified package on a thread                         //
//              CSE687 - Object Oriented Design                                //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			           //
// Author:		Yilan Li, Syracuse University, (315)382-9302				   //
//				yli41@syr.edu												   //
/////////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module defines a Task class. It provides a Task class that executes a specified
* callable object on a ThreadPool thread, using services of the ThreadPool package.
* 
* Public Interface:
* =================
* void addwork(std::function<type()> work, size_t worknum);   //execute a callable object
*																on thread
*
* Build Process:
* ==============
* Required files
* - Task.h Task.cpp ThreadPool.h ThreadPool.cpp BlockingQueue.h BlockingQueue.cpp 
*   ThreadUtilities.h ThreadUtilities.cpp
* Build commands
* - devenv Project_3.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.1 : 1 April 16
* Changed Class Task to operate with generic types. (Task<T>)
* ver 1.0 : 26 Mar 16
* - first release
*
*/

#include <map>
#include <vector>
#include <mutex>
#include <sstream>
#include <iostream>
#include <string>
#include "../ThreadPool/ThreadPool.h"

//////////////////////////////////////////////////////////////////////
// Task class 

template<typename type>
class Task
{
public:
	Task();
	Task(size_t num);
	~Task();
	void addwork(std::function<type()>* work, size_t worknum);
private:
	ThreadPool<type>*  threadpool;
	size_t threadnumber;
};
//----< void constructor >-------------------------------------------

template<typename type>
Task<type>::Task()
{}
//----< construct Task from thread number >--------------------------

template<typename type>
Task<type>::Task(size_t num)
{
	threadnumber = num;
	threadpool = new ThreadPool<type>(threadnumber);
	threadpool->start();
}
//----< Task destructor >--------------------------------------------

template<typename type>
Task<type>::~Task()
{}
//----< execute callable objects on thread >-------------------------

template<typename type>
void Task<type>::addwork(std::function<type()>* w, size_t worknum)
{
	for (size_t i = 0; i < worknum; i++)
		threadpool->doWork(w);
	threadpool->wait();
}

#endif
