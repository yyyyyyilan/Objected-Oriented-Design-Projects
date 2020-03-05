#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
/////////////////////////////////////////////////////////////////////////////
// MockChannel.h                                                           //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ                      //
// ver 1.2                                                                 //
// Language:    C++, Visual Studio 2015 Community                          //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			       //
// Application: Object Oriented Design, Spring 2016                        //
// Author:		Yilan Li, Syracuse University, (315)382-9302               //
//				yli41@syr.edu									           //
// Source:      Jim Fawcett, Syracuse University, CST 4-187                //
//              jfawcett@twcny.rr.com                                      //
/////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This purpose of this package is to provide a message-passing communication
* system. It supports HTTP style sending and receiving between client and GUI.
*
* Required Files:
* ===============
* MockChannel.h MockChannel.cpp HTTPMessage.h HTTPMessage.cpp BlockingQueue.h
* BlockingQueue.cpp Channel.h Channel.cpp Client.h Client.cpp FileSystem.h 
* FileSystem.cpp Logger.h Logger.cpp Socket.h Socket.cpp
*
* Build Process:
* ==============
* - devenv Project_4.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.2 April 17 by Yilan Li
* - added void MockChannel::send(HTTPMessage m)	to handle sending
*         HTTPMessage MockChannel::receive() to handle receive
* ver 1.1 April 15 by Yilan Li
* - modified std::string Message to HTTPmessage style message
* ver 1.0 by Jim Fawcett
* - first release
*/

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
#include "../HTTPMessage/HTTPMessage.h"
using Message = std::string;

struct ISendr
{
	virtual void postMessage(const HTTPMessage& msg) = 0;
};

struct IRecvr
{
	virtual HTTPMessage getMessage() = 0;
};

struct IMockChannel
{
public:
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void postMessage(HTTPMessage m) = 0;
	virtual HTTPMessage getMessage() = 0;
};

extern "C" {
	struct ObjectFactory
	{
		DLL_DECL ISendr* createSendr();
		DLL_DECL IRecvr* createRecvr();
		DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr);
	};
}

#endif


