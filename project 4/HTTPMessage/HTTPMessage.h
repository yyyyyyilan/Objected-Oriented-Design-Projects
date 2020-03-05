#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H
/////////////////////////////////////////////////////////////////////////////
// HTTPMessage.h - manipulate http style messages                          //
// ver 1.0                                                                 //
// Language:    C++, Visual Studio 2015 Community                          //
// Platform:  	Dell Inspiron 13-7352, Windows 10 Education			       //
// Application: Object Oriented Design, Spring 2016                        //
// Author:		Yilan Li, Syracuse University, (315)382-9302               //
//				yli41@syr.edu   		                                   //
/////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This package contains one HTTPMessage class. Its purpose is to provide an 
* interface to create and parse http style messages.
* 
* Public Interface:
* =================
* HTTPMessage();
* ~HTTPMessage();
* Attributes& addAttribute(const Message& name, const Message& value);
* Message findValue(const Message& name);
* Attributes parseAttribute(Message& attri);
* Message createHeader();
* bool hasAttrib(std::string a);
* void showHeader();
* void showAttributes();
*
* Required Files:
* ===============
* HTTPMessage.h HTTPMessage.cpp 
*
* Build Process:
* ==============
* - devenv Project_4.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.1 : 27 April 2016
* - added function bool hasAttrib(std::string a);
* ver 1.0 : 15 April 2016
* - first release
*/

#include <string>
#include <iostream>
#include <vector>


//////////////////////////////////////////////////////////////////////
// HTTPMessage class 

class HTTPMessage
{
public:
	using Message = std::string;
	using Attributes = std::vector <std::pair<Message, Message>>;
	using Body = std::vector<char>;
	HTTPMessage();
	~HTTPMessage();
	Attributes& addAttribute(const Message& name, const Message& value);
	Message findValue(const Message& name);
	Attributes parseAttribute(Message& attri);
	Message createHeader();
	bool hasAttrib(std::string a);
	void showHeader();
	void showAttributes();
private:
	Message trimFront(Message m);
	Attributes _attributes;
	Message _header;
	Body _body;
	size_t _sourcePort;
	size_t _targetPort;
};

#endif