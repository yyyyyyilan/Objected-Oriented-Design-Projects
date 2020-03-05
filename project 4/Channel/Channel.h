#ifndef CHANNEL_H
#define CHANNEL_H
/////////////////////////////////////////////////////////////////////////////
// Channel.h                                                               //
// - Channel reads from sendQ and writes to recvQ                          //
// ver 1.0                                                                 //
// Language:    C++, Visual Studio 2015 Community                          //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			       //
// Application: Object Oriented Design, Spring 2016                        //
// Author:		Yilan Li, Syracuse University, (315)382-9302               //
//				yli41@syr.edu									           //
/////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This purpose of this package is to provide a message-passing communication
* system. It supports HTTP style request/response transactions, one-way communication,
* and transmission of bytes streams.
*
* Public Interface:
* =================
* Channel:
*	Channel(ChannelSender* s, ChannelRecvr* r);
*	HTTPMessage getMessage();
*	void postMessage(const HTTPMessage& msg);
*	BlockingQueue<HTTPMessage>& rcvQueue();
*	BlockingQueue<HTTPMessage>& sendQueue();
* ChannelRecvr:
*	HTTPMessage getMessage();
*	BlockingQueue<HTTPMessage>& rqueue();
* ChannelSender
*   void postMessage(const HTTPMessage& msg);
*   BlockingQueue<HTTPMessage>& squeue();
*
* Required Files:
* ===============
* Channel.h Channel.cpp HTTPMessage.h HTTPMessage.cpp BlockingQueue.h BlockingQueue.cpp
*
* Build Process:
* ==============
* - devenv Project_4.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.0 by Yilan Li April 15 by Yilan Li
* - first release
*/

#include "../HTTPMessage/HTTPMessage.h"
#include "../BlockingQueue/BlockingQueue.h"

//////////////////////////////////////////////////////////////////////
// Recvr class

class ChannelRecvr
{
public:
	HTTPMessage getMessage();
	BlockingQueue<HTTPMessage>& rqueue();
private:
	BlockingQueue<HTTPMessage> recvQ_;
};
//////////////////////////////////////////////////////////////////////
// Sender class

class ChannelSender
{
public:
	void postMessage(HTTPMessage msg);
	BlockingQueue<HTTPMessage>& squeue();
private:
	BlockingQueue<HTTPMessage> sendQ_;
};
//////////////////////////////////////////////////////////////////////
// Channel clas

class Channel
{
public:
	Channel(ChannelSender* s, ChannelRecvr* r);
	HTTPMessage getMessage();
	void postMessage(HTTPMessage msg);
	BlockingQueue<HTTPMessage>& rcvQueue();
	BlockingQueue<HTTPMessage>& sendQueue();
private:
	ChannelSender* send;
	ChannelRecvr* recv;
};

#endif