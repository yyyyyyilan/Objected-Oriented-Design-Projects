/////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp                                                         //
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


#define IN_DLL
#include "MockChannel.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Client/Client.h"
#include <string>
#include <thread>
#include <iostream>
using BQueue = BlockingQueue < HTTPMessage >;

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
	void postMessage(const HTTPMessage& msg);
	BQueue& queue();
private:
	BQueue sendQ_;
};

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
	HTTPMessage getMessage();
	BQueue& queue();
private:
	BQueue recvQ_;
};

/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
	MockChannel(ISendr* pSendr, IRecvr* pRecvr);
	void start();
	void stop();
	void postMessage(HTTPMessage m);
	HTTPMessage getMessage() ;
	ISendr& sendr() { return *pISendr_; };
	IRecvr& rcvr() { return *pIRecvr_; };
private:
	std::thread thread_;
	std::thread thread_reply;
	ISendr* pISendr_;
	IRecvr* pIRecvr_;
	bool stop_ = false;
	ClientSenderHandler client_;
};


//----< sender post a http message >---------------------------------
void Sendr::postMessage(const HTTPMessage& msg)
{
	sendQ_.enQ(msg);
}

//----< return sending queue >---------------------------------------
BQueue& Sendr::queue() { return sendQ_; }

//----< receiver get a http message >--------------------------------
HTTPMessage Recvr::getMessage()
{
	return recvQ_.deQ();
}

//----< return receive queue >---------------------------------------
BQueue& Recvr::queue()
{
	return recvQ_;
}

//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr)
{
}
//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
	std::cout << "\n  MockChannel starting up";
	thread_ = std::thread(
		[this] {
		Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
		Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
		if (pSendr == nullptr || pRecvr == nullptr)
		{
			std::cout << "\n  failed to start Mock Channel\n\n";
			return;
		}
		client_.execute();	
		std::cout << "\n  Server stopping\n\n";
	});
}
//----< post message >-----------------------------------------------

void MockChannel::postMessage(HTTPMessage m)
{	
	client_.sendMsg(m);
}
//----< get message >------------------------------------------------

HTTPMessage MockChannel::getMessage()
{
	HTTPMessage ms = client_.getMsg();
	return ms;
}
//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr)
{
	return new MockChannel(pISendr, pIRecvr);
}

//----< test stub >----------------------------------------------------------

#ifdef TEST_MOCKCHANNEL

int main()
{
	ObjectFactory objFact;
	ISendr* pSendr = objFact.createSendr();
	IRecvr* pRecvr = objFact.createRecvr();
	IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
	pMockChannel->start();
	HTTPMessage msg;
	msg.addAttribute("Name", "Hello World");
	pSendr->postMessage(msg);
	msg.addAttribute("Content", "CSE687 - Object Oriented Design");
	pSendr->postMessage(msg);
	HTTPMessage rcvmsg = pRecvr->getMessage();
	std::cout << "\n received message = \n\t" << rcvmsg.createHeader() ;
	rcvmsg = pRecvr->getMessage();
	std::cout << "\n received message = \n\t" << rcvmsg.createHeader() ;
	pMockChannel->stop();
	HTTPMessage emsg;
	emsg.addAttribute("End", "quit");
	pSendr->postMessage(emsg);
	std::cin.get();
}
#endif
