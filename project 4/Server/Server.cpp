/////////////////////////////////////////////////////////////////////
// Server.h -   Server class                                       //
// ver 1.2                                                         //
// Application: CSE687 - Object Oriented Design                    //
//              Project 4 Dependency-Based Remote Code Repository  //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education		   //
// Author:		Yilan Li, Syracuse University, (315)382-9302	   //
//				yli41@syr.edu									   //
/////////////////////////////////////////////////////////////////////

#include "Server.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <time.h>
#include <stdio.h>
#include <vector>

using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;
//----< ServerRcvHandler default constructor >-----------------------

ServerRcvHandler::ServerRcvHandler()
{
	ChannelSender* s = new ChannelSender;
	ChannelRecvr* r = new ChannelRecvr;
	_channel = new Channel(s, r);
	dir_ = new std::vector<std::string>;
	dir_->push_back(root);
	_root = XmlProcessing::makeTaggedElement("?xml " + root);
	_root->addChild(xmlDeclaration);
}
//----< server receiver handler by calling operator >----------------

void ServerRcvHandler::operator()(Socket& socket_)
{
	while (true)
	{
		std::string msg = socket_.recvString();
		if ((msg == "QUIT") || (msg.size() == 0))
			break;		
		HTTPMessage httpmsg;
		httpmsg.parseAttribute(msg);
		_channel->rcvQueue().enQ(httpmsg);					
		std::string command = httpmsg.findValue("Command");
		if (command.size() == 0)
			std::cout << "No command.\n";
		if (command == "UPLOAD")
			UploadHandling(socket_);
		if (command == "DOWNLOAD")
			DownloadHandling(socket_);
		if (command == "ADD DEPENDENCY")
			AddDependencyHandling(socket_);
		if (command == "GET FILELISTS")
			GetFilelistHandling(socket_);
		if (command == "DISPLAY METADATA")
			displayMetadata();
	}
}
//----< server receiver upload handling >----------------------------

bool ServerRcvHandler::UploadHandling(Socket& socket_)
{
	if (socket_ == INVALID_SOCKET)
		return false;
	HTTPMessage httpmsg = _channel->getMessage();
	checkin(httpmsg);					
	std::string path = root + httpmsg.findValue("Content");
	FileSystem::File outFile(path);
	char buffer[BUFFERSIZE];
	outFile.open(FileSystem::File::out, FileSystem::File::binary);
	if (!outFile.isGood())
	{
		std::cout << "\n Can't upload " << path << "\n";
		return false;
	}
	size_t contentsize = std::stoul(httpmsg.findValue("Length"));
	while(true)
	{
		::Sleep(100);
		if (socket_.recv(contentsize, buffer))
		{
			outFile.putBuffer(contentsize, buffer);
			std::string out(buffer, contentsize);
			if (contentsize < BUFFERSIZE)
			{
				outFile.close();
				std::string rmsg = "Command:SERVER UPLOAD SUCCESS; Content:" + path + " ;";
				socket_.sendString(rmsg);
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
//----< server receiver download handling >--------------------------

bool ServerRcvHandler::DownloadHandling(Socket& socket_)
{
	if (socket_ == INVALID_SOCKET)
		return false;
	HTTPMessage httpmsg = _channel->getMessage();
	std::string path = "./Test/ServerTest/" + httpmsg.findValue("Content");
	FileSystem::File inFile(path);
	inFile.open(FileSystem::File::in, FileSystem::File::binary);
	if (!inFile.isGood())
	{
		std::cout << "\n Can't download " << path << "\n";
		return false;
	}
	char buffer[BUFFERSIZE];
	while (true)
	{
		size_t contentsize = inFile.getBuffer(BUFFERSIZE, buffer);
		std::string c(buffer, contentsize);
		httpmsg.addAttribute("Length", std::to_string(contentsize));
		socket_.sendString(httpmsg.createHeader());
		socket_.send(contentsize, buffer);
		if (contentsize < BUFFERSIZE)
		{
			inFile.close();
			break;
		}
		::Sleep(100);
	}
	return true;
}
//----< server receiver add package dependency handling >------------

bool ServerRcvHandler::AddDependencyHandling(Socket& socket_)
{
	if (socket_ == INVALID_SOCKET)
		return false;
	HTTPMessage httpmsg = _channel->getMessage();
	std::string depfile = root + httpmsg.findValue("Dependency");
	std::string file = root + httpmsg.findValue("Content");
	addDepend(file, depfile);
	return true;
}
//----< server receiver get package list handling >------------------

bool ServerRcvHandler::GetFilelistHandling(Socket& socket_)
{
	if (socket_ == INVALID_SOCKET)
		return false;
	HTTPMessage httpmsg = _channel->getMessage();
	std::string name = root + httpmsg.findValue("Content");
	std::string pname = getPackname(name);
	std::string withdep = httpmsg.findValue("WithDep");
	std::cout << "\n\n " << name;
	std::vector<sPtr> childset;
	for (auto pChild: _root->children())
	{
		childset = pChild->children();
		for (size_t i = 0; i < childset.size(); i++)
		{
			if (childset[i]->tag() == pname)
			{	
				if (withdep == "True")
				{
					std::cout << "\n extraction with dependency: \n";
					std::vector<sPtr> gradchild = childset[i]->children();
					for (size_t in = 0; in < gradchild.size(); in++)
					{		
						std::string temp = gradchild[in]->value();
						size_t p = temp.find("Dependency:");
						if (p < temp.size())
							getDepfiles(temp.substr(temp.find(":") + 2));
					}
				}
				else
					std::cout << "\n extraction without dependency. \n";
			}
		}
	}
	return true;
}
//----< helper function to get dependency files >--------------------

void ServerRcvHandler::getDepfiles(std::string pname)
{
	for (auto pChild : _root->children())
	{
		std::vector<sPtr> ch = pChild->children();
		for (size_t i = 0; i < ch.size(); i++)
		{
			if (ch[i]->tag() == pname)
			{
				std::vector<sPtr> temp = ch[i]->children();
				for (size_t in = 0; in < temp.size(); in++)
				{
					std::string t = temp[in]->value();
					size_t p = t.find("Files:");
					if (p < t.size())
						filelist.push_back(t.substr(t.find(":") + 2));
				}
			}
		}
	}
	for (size_t index = 0; index < filelist.size(); index++)
		std::cout << "\t -- Depend on: " << filelist[index] << "\n";
}
//----< server receve a file >---------------------------------------

bool ServerRcvHandler::receiveFileHandling(std::string name, Socket& socket_, size_t s)
{
	if (socket_ == INVALID_SOCKET)
		return false;
	std::string path = "./Test/ServerTest/" + name;
	FileSystem::File outFile(path);
	outFile.open(FileSystem::File::out, FileSystem::File::binary);
	if (!outFile.isGood())
	{
		std::cout << "\n Can't download " << path << "\n";
		return false;
	}
	char buffer[BUFFERSIZE];
	size_t bytesToRead;
	while (true)
	{
		if (s > BUFFERSIZE)
			bytesToRead = BUFFERSIZE;
		else
			bytesToRead = s;
		socket_.recv(bytesToRead, buffer);
		FileSystem::Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);
		outFile.putBlock(blk);
		if (s < BUFFERSIZE)
			break;
		s -= BUFFERSIZE;
	}
	outFile.close();
	return true;
}
//----< display metadata >-------------------------------------------

void ServerRcvHandler::displayMetadata()
{
	HTTPMessage httpmsg = _channel->getMessage();
	std::lock_guard<std::mutex> guard(ioLock);
	std::cout << "\n Dislpay created Metadata \n"
		<< std::string(32, '-') << std::endl;
	sPtr doc = XmlProcessing::makeDocElement(_root);
	std::cout << "  " << doc->toString();
	std::cout << "\n\n";	
	for (size_t i = 0; i < dir_->size(); i++)
		std::cout << dir_->at(i) << "\n";
}
//----< copy metadata >----------------------------------------------

sPtr ServerRcvHandler::copyMetadata(sPtr r)
{
	sPtr newnode = r ;
	for (auto pChild : r->children())
	{
		newnode->addChild(pChild);
	}
	return newnode;
}
//----< file check-in >----------------------------------------------

void ServerRcvHandler::checkin(HTTPMessage msg)
{
	std::string name = msg.findValue("Content");
	std::string pname = getPackname(name);
	createMetadata(pname, msg);
}
//----< helper function to get package name >------------------------

std::string ServerRcvHandler::getPackname(std::string fname)
{
	std::string sub;
	for (size_t i = fname.size() - 1; i >= 0; i--)
	{
		if (fname[i] == '.')
		{
			sub = fname.substr(0, i);
			break;
		}
	}
	return sub;
}
//----< helper function to create metadata when check in >-----------

void ServerRcvHandler::createMetadata(std::string packname, HTTPMessage msg)
{
	bool close = false;
	if (msg.hasAttrib("Checkin"))
	{
		if (msg.findValue("Checkin") == "Closed")
			close = true;
	}
	std::string name = root + msg.findValue("Content");
	std::string t = root + packname;
	std::string v = getTime();
	createDir(name, v);
	size_t position = hasMetadata(t);
	if (position < metadataSet.size())
	{
		if (checkinAvaliable(metadataSet[position]))
		{
			if (close == true)
			{
				sPtr ch = findCheckin(metadataSet[position]);
				metadataSet[position]->addChild(XmlProcessing::makeTextElement("Files: " + name));
				metadataSet[position]->removeChild(ch);
				metadataSet[position]->addChild(XmlProcessing::makeTextElement("Checkin: closed"));
			}
			else
				metadataSet[position]->addChild(XmlProcessing::makeTextElement("Files: " + name));
		}
		else
			createNewdata(packname, name, msg, v, close);
	}
	else
		createNewdata(packname, name, msg, v, close);
	saveMetadata(name, _root,v);
}
//----< helper function to find check in child >---------------------

sPtr ServerRcvHandler::findCheckin(sPtr p)
{
	std::vector<sPtr> temp = p->children();
	for (auto i : temp)
	{
		if (i->value().find("Checkin") < i->value().size())
			return i;
	}
	return _root;
}
//----< helper function to create new metadata >---------------------

void ServerRcvHandler::createNewdata(std::string pname, std::string name, HTTPMessage msg, std::string v, bool op)
{
	sPtr newfile = XmlProcessing::makeTaggedElement(root + pname);
	newfile->addChild(XmlProcessing::makeTextElement("Version: " + v));
	newfile->addChild(XmlProcessing::makeTextElement("Files: " + name));
	if (msg.hasAttrib("Dependency"))
	{
		std::string dep = msg.findValue("Dependency");
		newfile->addChild(XmlProcessing::makeTextElement("Dependency: " + dep));
	}
	if (op == true)
		newfile->addChild(XmlProcessing::makeTextElement("Checkin: closed"));
	else
		newfile->addChild(XmlProcessing::makeTextElement("Checkin: open"));
	xmlDeclaration->addChild(newfile);
	metadataSet.push_back(newfile);
}
//----< helper function to see if or not having metadata already >---

size_t ServerRcvHandler::hasMetadata(std::string packname)
{
	size_t po = metadataSet.size();
	for (size_t i = 0; i < metadataSet.size(); i++)
	{
		if (metadataSet[i]->tag() == packname)
			po = i;
	}
	return po;
}
//----< helper function to check checkin status >--------------------

bool ServerRcvHandler::checkinAvaliable(sPtr data)
{
	std::vector<sPtr> att = data->children();
	for (auto iter : att)
	{
		if (iter->value().find ("Checkin") < iter->value().size())
		{
			std::string t = iter->value();
			if (t.find("open"))
				return true;
		}
	}
	return false;
}
//----< helper function to save metadata in xml file >---------------

void ServerRcvHandler::saveMetadata(std::string name, sPtr f,std::string t)
{
	std::ofstream file;
	std::string p = getPackname(name);
	std::string fname = getFileNameOnly(p);
	std::string test = root + "xmlDirectory/" + fname + t + ".xml";
	file.open(root + "xmlDirectory/" + fname + t +".xml");
	std::string content = f->toString();
	std::cout <<"\n\n --Created Metadata : \n"  << content;
	file << content;
	file.close();
}
//----< helper function only get file name >-------------------------

std::string ServerRcvHandler::getFileNameOnly(std::string pname)
{
	for (size_t i = pname.size() - 1; i >= 0; i--)
	{
		if (pname[i] == '/')
		{
			std::string temp = pname.substr(i + 1);
			return temp;
		}
	}
	return "INVALID";
}
//----< create a unique directory >----------------------------------

std::vector<std::string> ServerRcvHandler::createDir(std::string name,std::string version)
{
	std::string dir = name + " Created: " + version;
	dir_->push_back(dir);
	return *dir_;
}
//----< helper function to get time as version>----------------------

std::string ServerRcvHandler::getTime()
{
	std::stringstream ss;
	time_t t = time(0);
	struct tm time_info;
	auto time = localtime_s(&time_info,&t);
	std::string version = "-" + std::to_string(time_info.tm_year + 1900) + "-"
		+ std::to_string(time_info.tm_mon + 1) + "-" 
		+ std::to_string(time_info.tm_mday ) + "-"
		+ std::to_string(time_info.tm_hour) + "-"
		+ std::to_string(time_info.tm_min) + "-"
		+ std::to_string(time_info.tm_sec);
	return version;
}
//----< helper function to add dependency >--------------------------

void ServerRcvHandler::addDepend(std::string f, std::string df)
{
	std::string packname = getPackname(f);
	std::string deppackname = getPackname(df);
	for (auto pChild : _root->children())
	{
		std::vector<sPtr> c = pChild->children();
		for (size_t i = 0; i < c.size(); i++)
		{
			if (c[i]->tag() == packname)
			{
				c[i]->addChild(XmlProcessing::makeTextElement("Dependency: " + deppackname));
				return;
			}
		}
	}
	displayMetadata();
}
//----< test stub >--------------------------------------------------

#ifdef TEST_SERVER

int main(int argc, char* argv[])
{
	std::cout << "\n Testing Server class\n"
		<< std::string(32, '=') << std::endl;
	try
	{
		SocketSystem ss;
		SocketListener sl(std::stoul(argv[1]), Socket::IP6);
		ServerRcvHandler rcvHandler;
		sl.start(rcvHandler);
		std::cout << "\n --------------------\n  press key to exit: \n --------------------";
		std::cout.flush();
		std::cin.get();	
	}
	catch (std::exception& exc)
	{
		std::cout << "\n  Exeception caught: ";
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		std::cout << exMsg;
	}
	return 0;
}
#endif