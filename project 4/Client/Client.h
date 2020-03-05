#ifndef CLIENT_H
#define CLIENT_H
/////////////////////////////////////////////////////////////////////
// Client.h -   Client class                                       //
// ver 1.2                                                         //
// Application: CSE687 - Object Oriented Design                    //
//              Project 4 Dependency-Based Remote Code Repository  //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education		   //
// Author:		Yilan Li, Syracuse University, (315)382-9302	   //
//				yli41@syr.edu									   //
/////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This package contains ClientSenderHandler class, and Client class. 
* Its purpose is to provide an interface to handle operations, such as 
* UPLOAD, DOWNLOAD, GET FILELISTS AND ADD DEPENDENCY, which are performed
* by client side.
*
* Public Interface:
* =================
* ClientSenderHandler();
* void execute();
* bool UploadHandling(Socket& socket_, HTTPMessage m);
* bool DownloadHandling(Socket& socket_, HTTPMessage m);
* bool AddDependencyHandling(Socket& socket_, HTTPMessage m);
* bool GetFileListsHandling(Socket& socket_, HTTPMessage m);
* void sendMsg(HTTPMessage m);
* HTTPMessage getMsg();
* bool sendFileHandling(std::string name, Socket& socket_);
* void setStart(BlockingQueue<HTTPMessage>* sq, BlockingQueue<HTTPMessage>* rq);
*
* Required Files:
* ===============
* Client.h Client.cpp Channel.h Channel.cpp FileSystem.h FileSystem.cpp Logger.h
* Logger.cpp Socket.h Socket.cpp HTTPMessage.h HTTPMessage.cpp
* 
* Build Command:
* ====================
* - devenv Project_4.sln /rebuild debug
*
* Maintenance History :
* =====================
* ver 1.2 : 1 May 2016
* - added void setStart(BlockingQueue<HTTPMessage>* sq, BlockingQueue<HTTPMessage>* rq)
* ver 1.1 : 22 April 2016
* - added two functions in Class ClientSenderHandler
*		bool GetFilelistHandling(Socket& socket_)
*		bool AddDependencyHandling(Socket& socket_)
* ver 1.0 : 19 April 2016
* - first release
*/


#include "../Sockets/Sockets.h"
#include "../FileMgr/FileSystem.h"
#include "../Channel/Channel.h"
#include "../HTTPMessage/HTTPMessage.h"
#include <mutex>
#define BUFFERSIZE 1024

//////////////////////////////////////////////////////////////////////
// Client's SenderHandler class

class ClientSenderHandler
{
public:
	ClientSenderHandler();
	void execute();
	bool UploadHandling(Socket& socket_, HTTPMessage m);
	bool DownloadHandling(Socket& socket_, HTTPMessage m);
	bool AddDependencyHandling(Socket& socket_, HTTPMessage m);
	bool GetFileListsHandling(Socket& socket_, HTTPMessage m);
	void sendMsg(HTTPMessage m);
	HTTPMessage getMsg();
	bool sendFileHandling(std::string name, Socket& socket_);
private:
	void commandAnalyzer(Socket& socket_, HTTPMessage m, std::string c);
	bool isConnect(SocketConnecter& socketc_, std::string h, size_t p);
	void getResult(std::string reply);
	SocketConnecter* _sc;
	ChannelSender sender;
	ChannelRecvr receiver;
	Channel* _channel;
	std::mutex ioLock;
	size_t port_;
	std::string host_;
};

#endif