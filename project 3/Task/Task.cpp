/////////////////////////////////////////////////////////////////////////////////
// Task.cpp - execute a specified callable object on a ThreadPool thread       //
// ver 1.1                                                                     //
// Application: Evaluate specified package on a thread                         //
//              CSE687 - Object Oriented Design                                //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			           //
// Author:		Yilan Li, Syracuse University, (315)382-9302				   //
//				yli41@syr.edu												   //
/////////////////////////////////////////////////////////////////////////////////

#include "Task.h"
#include "../ThreadPool/ThreadPool.h"
#include <string>
#include <thread>
#include <mutex>
#include <functional>

//----< test stub >------------------------------------------------------------

#ifdef TEST_TASK

int main()
{
	try {
		size_t threadnumber = 3;
		size_t worknum = 5;
		Task<std::string> task(threadnumber);
		std::function<std::string()> work = [&]() {
			std::cout << "\n  working on thread " << std::this_thread::get_id();
			std::cout << ": testing task package \n";
			return "task package";
		};
		task.addwork(&work, worknum);
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}

	std::cout << "\n  main thread id = " << std::this_thread::get_id() << "\n";
	return 0;
}

#endif