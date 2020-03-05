#ifndef SERVER_H
#define SERVER_H
/////////////////////////////////////////////////////////////////////
// Server.h -   Server class                                       //
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
* This package contains ServerSenderHandler class. 
* Its purpose is to provide an interface to handle operations, 
* such as UPLOAD, DOWNLOAD, GETFILELIST AND ADDDEPENDENCY, which are performed 
* by server side.
*
* Public Interface:
* =================
* ServerRcvHandler();
* using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;
* void operator()(Socket& socket_);
* bool UploadHandling(Socket& socket_);
* bool DownloadHandling(Socket& socket_);
* bool AddDependencyHandling(Socket& socket_);
* bool GetFilelistHandling(Socket& socket_);
* void displayMetadata();
* sPtr copyMetadata(sPtr r);
* bool receiveFileHandling(std::string name, Socket& socket_, size_t s);
*
* Required Files:
* ===============
* Server.h Server.cpp Channel.h Channel.cpp HTTPMessage.h HTTPMessage.cpp
* FileSystem.h FileSystem.cpp Logger.h Logger.cpp Socket.h Socket.cpp
* XmlElement.h XmlElement.cpp
*
* Build Command:
* ====================
* - devenv Project_4.sln /rebuild debug
*
* Maintenance History :
* ======================
* ver 1.2 : 22 April 2016
* - added two functions in Class ServerRcvHandler
*		bool GetFilelistHandling(Socket& socket_)
*		bool AddDependencyHandling(Socket& socket_)
* - added several functions class to ServerRcvHandler to implement checkin functionality
*       void checkin(HTTPMessage msg)
*		sPtr createMetadata(std::string packname, HTTPMessage msg)
*		std::string getPackname(std::string fname);
*		std::vector<std::string> createDir(std::string name, std::string date)
*		std::string getTime()
* ver 1.1 : 18 April 2016 
* - modified Server class to class ServerSenderHandler
* ver 1.0 : 17 April 2016
* - first release
*/

#include <ctime>
#include <vector>
#include "../Sockets/Sockets.h"
#include "../FileMgr/FileSystem.h"
#include "../XmlDocument/XmlElement.h"
#include "../Channel/Channel.h"
#include "../HTTPMessage/HTTPMessage.h"
#include <mutex>
#define BUFFERSIZE 1024

//////////////////////////////////////////////////////////////////////
// Server's RcvHandler class 

class ServerRcvHandler
{
public:
	ServerRcvHandler();
	using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;
	void operator()(Socket& socket_);
	bool UploadHandling(Socket& socket_);
	bool DownloadHandling(Socket& socket_);
	bool AddDependencyHandling(Socket& socket_);
	bool GetFilelistHandling(Socket& socket_); 
	void displayMetadata();
	sPtr copyMetadata(sPtr r);
	bool receiveFileHandling(std::string name, Socket& socket_, size_t s);
private:
	void checkin(HTTPMessage msg);
	void createMetadata(std::string packname, HTTPMessage msg);
	bool checkinAvaliable(sPtr data);
	std::string getPackname(std::string fname);
	std::vector<std::string> createDir(std::string name, std::string date);
	std::string getTime();
	void addDepend(std::string packname, std::string deppackname);
	void getDepfiles(std::string pname);
	void saveMetadata(std::string name , sPtr f, std::string t);
	void createNewdata(std::string pname, std::string name, HTTPMessage msg,std::string v, bool op);
	size_t hasMetadata(std::string packname);
	std::string getFileNameOnly(std::string pname);
	sPtr findCheckin(sPtr p);
	std::vector<sPtr> metadataSet;
	std::vector<std::string> filelist;
	std::string root = "./Test/ServerTest/";
	sPtr _root;
	sPtr xmlDeclaration = XmlProcessing::makeTaggedElement(root + "xmlDirectory ");
	Channel* _channel;
	ChannelSender s;
	ChannelRecvr r;
	BlockingQueue<HTTPMessage> sendqueue;
	BlockingQueue<HTTPMessage> receivequeue;
	std::vector<std::string>* dir_;
	std::mutex ioLock;
};

#endif
