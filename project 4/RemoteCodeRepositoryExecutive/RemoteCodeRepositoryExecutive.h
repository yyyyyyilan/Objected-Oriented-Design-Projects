#ifndef REMOTECODEREPOSITORYEXECUTIVE_H
#define REMOTECODEREPOSITORYEXECUTIVE_H
///////////////////////////////////////////////////////////////////
// RemoteCodeRepositoryExecutive.cpp                             //
//              - Project_4 automated unit test suite            //
// ver 1.0                                                       //
// Application: Remote Code Repository Executive                 //
// Language:    C++, Visual Studio 2015 Community                //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education	     //
// Author:		Yilan Li, Syracuse University, (315)382-9302     //
//				yli41@syr.edu									 //
///////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This package contains class RemoteCodeRepositoryExecutive. It's the 
* automated unit test suite of Project_4.
*
* Public Interface:
* =================
* RemoteCodeRepositoryExecutive();
* ~RemoteCodeRepositoryExecutive();
* void RequirementOneandTwo();
* void RequirementThreeandFive();
* void RequirementFour();
* void RequirementSix();
* void RequirementSeven();
* void RequirementEightandNine();
* void RequirementTen();
* void RequirementEleven();
*
* Required Files:
* ===============
* RemoteCodeRepositoryExecutive.h RemoteCodeRepositoryExecutive.cpp 
* Server.h Server.cpp Client.h Client.cpp Channel.h Channel.cpp 
* HTTPMessage.h HTTPMessage.cpp BlockingQueue.h BlockingQueue.cpp
* FileSystem.h FileSystem.cpp Sockets.h Sockets.cpp XmlElement.h XmlElement.cpp
* Logger.h Logger.cpp
*
* Build Command:
* ====================
* - devenv Project_4.sln /rebuild debug
*
* Maintenance History :
* == == == == == == == == == ==
* ver 1.0 : 1 May 2016
* - first release
*/

#include <iostream>
#include <string>
#include <fstream>
#include <xstring>
#include "../Client/Client.h"
#include "../Server/Server.h"
#include "../Channel/Channel.h"

//////////////////////////////////////////////////////////////////////
// RemoteCodeRepositoryExecutive class 

class RemoteCodeRepositoryExecutive
{
public:
	RemoteCodeRepositoryExecutive();
	~RemoteCodeRepositoryExecutive();
	void RequirementOneandTwo(std::string p);
	void RequirementThreeandFive(std::string p);
	void RequirementFour(std::string p);
	void RequirementSeven(std::string p);
	void RequirementEightandNine(std::string p);
	void RequirementTen(std::string p);
	void RequirementEleven(std::string p);
private:
	void withDep(std::string p);
	void withoutDep(std::string p);
};

#endif