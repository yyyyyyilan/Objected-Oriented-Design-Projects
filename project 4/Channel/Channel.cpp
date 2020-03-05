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

#include "Channel.h"

//----< receiver get a http message >--------------------------------

HTTPMessage ChannelRecvr::getMessage()
{
	return recvQ_.deQ();
}
//----< return receive queue >---------------------------------------

BlockingQueue<HTTPMessage>& ChannelRecvr::rqueue()
{
	return recvQ_;
}
//----< sender post a http message >---------------------------------

void ChannelSender::postMessage(HTTPMessage msg)
{
	msg.showAttributes();
	return sendQ_.enQ(msg);
}
//----< return sending queue >---------------------------------------

BlockingQueue<HTTPMessage>& ChannelSender::squeue()
{
	return sendQ_;
}
//----< pass pointers to Sender and Receiver >-------------------------------

Channel::Channel(ChannelSender* s, ChannelRecvr* r)
{
	send = s;
	recv = r;
}
//----< channel get message >------------------------------------------------

HTTPMessage Channel::getMessage()
{
	return recv->getMessage();
}
//----< channel post message >-----------------------------------------------

void Channel::postMessage(HTTPMessage msg)
{
	send->postMessage(msg);
}
//----< return receive queue >-----------------------------------------------

BlockingQueue<HTTPMessage>& Channel::rcvQueue()
{
	return recv->rqueue();
}
//----< return send queue >--------------------------------------------------

BlockingQueue<HTTPMessage>& Channel::sendQueue()
{
	return send->squeue();
}
//----< test stub >----------------------------------------------------------

#ifdef TEST_CHANNEL

int main()
{
	std::cout << "\n  Testing Channel class\n "
		<< std::string(32, '=') << std::endl;
	try {
		ChannelSender s;
		ChannelRecvr r;
		Channel channel(&s, &r);
		HTTPMessage msg;
		msg.addAttribute("Name", "Hello World");
		channel.postMessage(msg);
		msg.addAttribute("Content", "CSE687 - Object Oriented Design");
		channel.postMessage(msg);
		BlockingQueue<HTTPMessage> sQ = channel.sendQueue();
		HTTPMessage rcvmsg = sQ.deQ();
		std::cout << "\n received message = \n\t" << rcvmsg.createHeader();
		rcvmsg = sQ.deQ();
		std::cout << "\n received message = \n\t" << rcvmsg.createHeader();
		HTTPMessage emsg;
		emsg.addAttribute("End", "quit");
		channel.postMessage(emsg);
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	return 0;
}
#endif

