#ifndef UTILITIES_H
#define UTILITIES_H
///////////////////////////////////////////////////////////////////////
// Utilities.h - small, generally usefule, helper classes            //
// ver 1.1                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes StringHelper and Converter and a global
* function putline().  This class will be extended continuously for
* awhile to provide convenience functions for general C++ applications.
*
* Build Process:
* --------------
* Required Files: Utilities.h, Utilities.cpp
*
* Build Command: devenv Utilities.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.1 : 06 Feb 2015
* - fixed bug in split which turns a comma separated string into
*   a vector of tokens.
* - added comments
* ver 1.0 : 05 Feb 2016
* - first release
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/
#include <string>
#include <vector>
#include <sstream>
#include <functional>

void title(const std::string& text, char underline = '-');

/////////////////////////////////////////////////////////////////////////////
// logit class

class logit
{
public:
	void start();
	void stop();
	void show();
	void clear();
	bool running();
	void put(char ch);
private:
	std::ostringstream out_;
	bool running_;
};

logit& operator<<(logit& out, const std::string& in);

extern logit mtLog;

struct Cosmetic
{
	~Cosmetic();
};

extern Cosmetic cosmetic;


#endif

