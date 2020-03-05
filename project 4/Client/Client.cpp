/////////////////////////////////////////////////////////////////////
// Client.h -   Client class                                       //
// ver 1.2                                                         //
// Application: CSE687 - Object Oriented Design                    //
//              Project 4 Dependency-Based Remote Code Repository  //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education		   //
// Author:		Yilan Li, Syracuse University, (315)382-9302	   //
//				yli41@syr.edu									   //
/////////////////////////////////////////////////////////////////////

#include "Client.h"
#include "../FileMgr/FileSystem.h"
#include <iostream>
#include <string>

//----< ClientSenderHandler constructor >----------------------------

ClientSenderHandler::ClientSenderHandler()
{
	_channel = new Channel(&sender, &receiver);
}
//----< helper function to connect with the server >-----------------

bool ClientSenderHandler::isConnect(SocketConnecter& socketc_, std::string h, size_t p)
{
	size_t index = 10;
	while (!socketc_.connect(h, p))
	{
		if (index == 0)
		{
			std::cout << "\n Connection Failed.";
			return false;
		}
		else
		{
			index--;
			std::cout << "\n waiting to connect... ";
			::Sleep(100);
		}
	}
	return true;
}
//----< client sender execute >--------------------------------------

void ClientSenderHandler::execute()
{
	while (true)
	{
		::Sleep(100);
		SocketSystem ss;
		SocketConnecter _socketConnector;
		BlockingQueue<HTTPMessage> sQ = sender.squeue();
		if (sQ.size() == 0)
			continue;	
		HTTPMessage httpmsg = sender.squeue().deQ();
		host_ = httpmsg.findValue("FromAddr");
		port_ = std::stoul(httpmsg.findValue("ToPort"));
		if (isConnect(_socketConnector, host_, port_))
		{
			std::string command = httpmsg.findValue("Command");
			if (command != "")
			{
				commandAnalyzer(_socketConnector, httpmsg, command);
				_socketConnector.shutDownSend();
			}
			else
				continue;
		}
		else
			continue;
	}
}
//

void ClientSenderHandler::commandAnalyzer(Socket& socket_, HTTPMessage m, std::string c)
{
	if (c == "UPLOAD")
		UploadHandling(socket_, m);
	if (c == "DOWNLOAD")
		DownloadHandling(socket_, m);
	if (c == "ADD DEPENDENCY")
		AddDependencyHandling(socket_, m);
	if (c == "GET FILELISTS")
		GetFileListsHandling(socket_, m);
	if (c == "DISPLAY METADATA")
		socket_.sendString(m.createHeader());
}
//----< client sender upload handling >------------------------------

bool ClientSenderHandler::UploadHandling(Socket& socket_, HTTPMessage m)
{
	if (socket_ == INVALID_SOCKET)
		return false;
	std::string path = "./Test/ClientTest/" + m.findValue("Content");
	FileSystem::File inFile(path);
	inFile.open(FileSystem::File::in, FileSystem::File::binary);
	if (!inFile.isGood())
	{
		std::cout << "\n Can't open " << path;
		return false;
	}
	char buffer[BUFFERSIZE];
	while (true)
	{	
		size_t contentsize = inFile.getBuffer(BUFFERSIZE, buffer);
		std::string c(buffer, contentsize);
		m.addAttribute("Length", std::to_string(contentsize));
		socket_.sendString(m.createHeader());
		socket_.send(contentsize, buffer);
		if (contentsize < BUFFERSIZE || !inFile.isGood())
		{
			inFile.close();
			break;
		}
		::Sleep(100);
	}
	socket_.sendString("QUIT");
	while(true)
	{
		std::string reply = socket_.recvString();
		HTTPMessage r;
		r.parseAttribute(reply);
		if (r.findValue("Command") == "SERVER UPLOAD SUCCESS")
		{
			getResult(reply);
			std::cout << "\n -- " << reply ;
			break;
		}
	}	
	return true;
}
//----< client sender download handling >----------------------------

bool ClientSenderHandler::DownloadHandling(Socket& socket_, HTTPMessage m)
{
	if (socket_ == INVALID_SOCKET)
		return false;
	socket_.sendString(m.createHeader());
	std::string path = "./Test/ClientTest/" + m.findValue("Content");
	FileSystem::File outFile(path);
	char buffer[BUFFERSIZE];
	outFile.open(FileSystem::File::out, FileSystem::File::binary);
	if (!outFile.isGood())
	{
		std::cout << "\n Can't download " << path << "\n";
		return false;
	}
	std::string h = socket_.recvString();
	HTTPMessage httpmsg;
	httpmsg.parseAttribute(h);
	size_t contentsize = std::stoul(httpmsg.findValue("Length"));
	while (true)
	{
		::Sleep(100);
		if (socket_.recv(contentsize, buffer))
		{
			outFile.putBuffer(contentsize, buffer);
			std::string out(buffer, contentsize);
			if (contentsize < BUFFERSIZE)
			{
				outFile.close();
				std::string rmsg = "Command:CLIENT DOWNLOAD SUCCESS; Content:" + path + " ;";
				getResult(rmsg);
				std::cout << "\n" << rmsg;
				break;
			}
			std::string rcvs = socket_.recvString();
			HTTPMessage h;
			h.parseAttribute(rcvs);
			contentsize = std::stoul(h.findValue("Length"));
		}
		else
			return false;
	}
	return true;
}
//----< client sender add package dependency handling >--------------

bool ClientSenderHandler::AddDependencyHandling(Socket& socket_, HTTPMessage m)
{
	if (socket_ == INVALID_SOCKET)
		return false;
	std::string pakname = m.findValue("Content");
	socket_.sendString(m.createHeader());
	return true;
}
//----< client sender get package list handling >--------------

bool ClientSenderHandler::GetFileListsHandling(Socket& socket_, HTTPMessage m)
{
	if (socket_ == INVALID_SOCKET)
		return false;
	std::string name = m.findValue("Content");
	socket_.sendString(m.createHeader());
	return true;
}
//----< client send file >-------------------------------------------

bool ClientSenderHandler::sendFileHandling(std::string name, Socket& socket_)
{
	if (socket_ == INVALID_SOCKET)
		return false;
	std::string path = "./Test/ClientTest/" + name;
	FileSystem::FileInfo info(path);
	size_t size = info.size();
	FileSystem::File inFile(path);
	inFile.open(FileSystem::File::in, FileSystem::File::binary);
	if (!inFile.isGood())
	{
		std::cout << "\n Can't open " << path;
		return false;
	}
	char buffer[BUFFERSIZE];
	while (true)
	{
		FileSystem::Block blk = inFile.getBlock(BUFFERSIZE);
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];
		socket_.send(blk.size(), buffer);
		if (!inFile.isGood())
			break;
	}
	inFile.close();
	return true;
}
//----< client  send http message >----------------------------------

void ClientSenderHandler::sendMsg(HTTPMessage m)
{
	sender.postMessage(m);
}

//----< client receive http message >--------------------------------

HTTPMessage ClientSenderHandler::getMsg()
{
	HTTPMessage m;
	receiver.getMessage();
	return m;
}
//----< put reply message into client receive queue >----------------

void ClientSenderHandler::getResult(std::string reply)
{
	HTTPMessage m;
	m.parseAttribute(reply);
	receiver.rqueue().enQ(m);
}
//----< test stub >--------------------------------------------------

#ifdef TEST_CLIENT

int main(int argc, char* argv[])
{
	std::cout << "\n Testing Client class\n"
		<< std::string(32, '=') << std::endl;
	try
	{
		HTTPMessage end;
		end.addAttribute("Command", "QUIT");
		ClientSenderHandler c1;
		HTTPMessage m1;
		m1.addAttribute("Command", "UPLOAD");
		m1.addAttribute("Content", "a.txt");
		m1.addAttribute("ToAddr", argv[1]);
		m1.addAttribute("FromAddr", "localhost");
		c1.sendMsg(m1);
		ClientSenderHandler c2;
		HTTPMessage m2;
		m2.addAttribute("Command", "UPLOAD");
		m2.addAttribute("Content", "b.cpp");
		m2.addAttribute("ToAddr", argv[1]);
		m2.addAttribute("FromAddr", "localhost");
		c2.sendMsg(m2);
		std::thread t1(
			[&]() { c1.execute(); }
		);
		t1.join();
		std::thread t2(
			[&]() {c2.execute(); }
		);	
		t2.join();
		ClientSenderHandler c3;
		HTTPMessage m3;
		m3.addAttribute("Command", "ADD DEPENDENCY");
		m3.addAttribute("Content", "a.txt");
		m3.addAttribute("ToAddr", argv[1]);
		m3.addAttribute("FromAddr", "localhost");
		m3.addAttribute("Dependency", "b.cpp");		
		c3.sendMsg(m3);
		std::thread t3(
			[&]() {c3.execute(); }
		);	
		t3.join();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n";
	}
	return 0;
}

#endif