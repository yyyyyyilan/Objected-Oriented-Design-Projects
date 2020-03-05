/////////////////////////////////////////////////////////////////////////////
// HTTPMessage.cpp - manipulate http style messages                        //
// ver 1.0                                                                 //
// Language:    C++, Visual Studio 2015 Community                          //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			       //
// Application: Object Oriented Design, Spring 2016                        //
// Author:		Yilan Li, Syracuse University, (315)382-9302               //
//				yli41@syr.edu									           //
/////////////////////////////////////////////////////////////////////////////

#include "HTTPMessage.h"
#include <string>
#include <iostream>
#include <iterator>
using Message = std::string;
using Attributes = std::vector <std::pair<Message, Message>>;
using Body = std::vector<char>;
//----< create HTTPMessage instance >----------------------------------------

HTTPMessage::HTTPMessage():_header(""), _sourcePort(0),_targetPort(0)
{}
//----< HTTPMessage destructor >---------------------------------------------

HTTPMessage::~HTTPMessage()
{}
//----< add attributes pair>-------------------------------------------------

Attributes& HTTPMessage::addAttribute(const Message& name, const Message& value)
{
	std::pair<Message, Message> temp = std::make_pair(name, value);
	_attributes.push_back(temp);
	return _attributes;
}
//----< find an attribute value >--------------------------------------------

Message HTTPMessage::findValue(const Message& name)
{
	Message value = "";
	Message tempHeader = createHeader();
	size_t valuePosition = tempHeader.find(name);
	Message t = tempHeader.substr(valuePosition);
	if (t.find(":") < t.size())
	{
		Message temp = t.substr(t.find(":") + 1);
		size_t t = temp.find(";");
		value = temp.substr(0, t);
	}
	else
		value = "NOT FOUND";
	return value;
}
//---- < parse a string to attribute pair>-----------------------------------

Attributes HTTPMessage::parseAttribute(Message& attri)
{
	Message temp = trimFront(attri);
	size_t p1 = temp.find(":");
	size_t p2 = temp.find(";");
	size_t len = p2 - p1;
	Message name = temp.substr(0, p1);
	Message value = temp.substr(p1 + 1, len - 1);
	if (len != 0)
	{
		std::pair<Message, Message> t;
		t = std::make_pair(name, value);
		_attributes.push_back(t);
		Message remainMsg = temp.substr(p2 + 1);
		parseAttribute(remainMsg);
	}
	return _attributes;
}
//----< create a header string from attribute>-------------------------------

Message HTTPMessage::createHeader()
{
	std::string temp;
	for (auto iter : _attributes)
	{
		std::string t = iter.first + ":" + iter.second;
		temp = temp + t + "; ";
	} 
	temp = temp + "\r\n";
	_header = temp;
	return _header;
}
//----< whether has an particular attribute >--------------------------------

bool HTTPMessage::hasAttrib(std::string a)
{
	for (auto iter : _attributes)
	{
		std::pair<Message, Message> temp = iter;
		if (temp.first == a)
			return true;
	}
	return false;
}
//----< display header >-----------------------------------------------------

void HTTPMessage::showHeader()
{
	std::cout << createHeader();
}
//----< display attributes >-------------------------------------------------

void HTTPMessage::showAttributes()
{
	std::cout << "\n Created HTTPmessage's attributes are: \n";
	for (auto iter : _attributes)
	{
		std::pair<Message, Message> temp = iter;
		std::cout << "\t--" << iter.first << " : " << iter.second << "\n";
	}
}
//----< remove spaces or newlines from front of message >--------------------

Message HTTPMessage::trimFront(Message m)
{
	while (m.size() > 0)
	{
		if ((m[0] ==  ' ') || (&m[0] == "\n"))
			m = m.substr(1);
		else
			break;
	}
	return m;
}

//----< test stub >----------------------------------------------------------

#ifdef TEST_HTTPMESSAGE

void main()
{
	std::cout << "\n Testing HTTPMessage class\n"
		<< std::string(32, '=') << std::endl;
	try
	{
		std::string filename = "myfile";
		std::string mode = "oneway";
		std::string to = "128.230.96.123";
		std::string from = "128.230.96.456";
		std::string filecontent = "FileStream";
		std::string command = "UPLOAD";
		size_t size = 35430;
		std::cout << "\n Testing create HTTP message\n"
			<< std::string(32, '-') << std::endl;
		HTTPMessage httpMsg;
		httpMsg.addAttribute("Command", command);
		httpMsg.addAttribute("Mode", mode);
		httpMsg.addAttribute("ToAddr", "123");
		httpMsg.addAttribute("FromAddr", "456");
		httpMsg.addAttribute("Content", "FileStream");
		httpMsg.addAttribute("Name", "myfile");
		httpMsg.addAttribute("Length", std::to_string(size));
		std::cout << "\n\nHeader is: ";
		httpMsg.createHeader();
		httpMsg.showHeader();
		std::cout << "\n Testing parse HTTP message\n"
			<< std::string(32, '-') << std::endl;
		std::string testAttri = "Conmand:upload; Mode:oneway;From:123;To:456;Length:32580;";
		HTTPMessage parsehttpMsg;
		parsehttpMsg.parseAttribute(testAttri);
		parsehttpMsg.showAttributes();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n   " << ex.what() << "\n\n";
	}
}

#endif
