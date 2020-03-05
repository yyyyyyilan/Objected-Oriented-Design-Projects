///////////////////////////////////////////////////////////////////
// RemoteCodeRepositoryExecutive.cpp                             //
//              - Project_4 automated unit test suite            //
// ver 1.0                                                       //
// Application: Remote Code Repository Executive                 //
// Language:    C++, Visual Studio 2015 Community                //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education	     //
// Author:		Yilan Li, Syracuse University, (315)382-9302     //
//				yli41@syr.edu									 //
///////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include "RemoteCodeRepositoryExecutive.h"

//----< RemoteCodeRepositoryExecutive constructor >------------------

RemoteCodeRepositoryExecutive::RemoteCodeRepositoryExecutive()
{}
//----< RemoteCodeRepositoryExecutive destructor >-------------------

RemoteCodeRepositoryExecutive::~RemoteCodeRepositoryExecutive()
{}
//----< Requirement one and two demo >-------------------------------

void RemoteCodeRepositoryExecutive::RequirementOneandTwo(std::string p)
{
	std::cout << "///////////////////////////////////////\n";
	std::cout << "// Demo for Requirement ONE and TWO. //" << std::endl;
	std::cout << "///////////////////////////////////////\n\n";
	std::string line;
	std::ifstream myfile;
	myfile.open("./README.txt");
	while (!myfile.eof())
	{
		std::getline(myfile, line);
		std::cout << line << "\n";
	}
	myfile.close();
	std::cout << "\n\n";
}
//----< Requirement three demo >-------------------------------------

void RemoteCodeRepositoryExecutive::RequirementThreeandFive(std::string p)
{
	std::cout << "\n\n\n";
	std::cout << "//////////////////////////////////////////\n";
	std::cout << "// Demo for Requirement THREE and FIVE. //" << std::endl;
	std::cout << "//////////////////////////////////////////\n\n";
	std::cout << "Upload Requirement3.h and Requirement3.cpp to the server.\n";
	std::cout << "The metadata is shown on server(8080) side.";
	try {
		ClientSenderHandler c1;
		HTTPMessage m1;
		m1.addAttribute("Command", "UPLOAD");
		m1.addAttribute("Content", "ClientRequirement3.h");
		m1.addAttribute("ToPort", "8080");
		m1.addAttribute("FromAddr", "localhost");
		c1.sendMsg(m1);
		HTTPMessage m2;
		m2.addAttribute("Command", "UPLOAD");
		m2.addAttribute("Content", "ClientRequirement3.cpp");
		m2.addAttribute("ToPort", "8080");
		m2.addAttribute("FromAddr", "localhost");
		c1.sendMsg(m2);
		HTTPMessage m3;
		m3.addAttribute("Command", "DISPLAY METADATA");
		m3.addAttribute("Content","ClientRequirement3.h");
		m3.addAttribute("ToPort", "8080");
		m3.addAttribute("FromAddr", "localhost");
		c1.sendMsg(m3);
		std::thread t1(
			[&]() { c1.execute(); }
		);
		t1.join();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n";
	}
}
//----< Requirement four demo >--------------------------------------

void RemoteCodeRepositoryExecutive::RequirementFour(std::string p)
{
	std::cout << "\n\n\n";
	std::cout << "////////////////////////////////\n";
	std::cout << "// Demo for Requirement FOUR. //" << std::endl;
	std::cout << "////////////////////////////////\n\n";
	std::cout << "The dependency is added manually.\n";
	std::cout << "ClientRequirement4.h depends on ClientRequirement3 package. \n";
	try {
		ClientSenderHandler c1;
		HTTPMessage m1;
		m1.addAttribute("Command", "UPLOAD");
		m1.addAttribute("Content", "ClientRequirement4.h");
		m1.addAttribute("ToPort", "8080");
		m1.addAttribute("FromAddr", "localhost");
		c1.sendMsg(m1);
		HTTPMessage m2;
		m2.addAttribute("Command", "DISPLAY METADATA");
		m2.addAttribute("Content", "ClientRequirement4.h");
		m2.addAttribute("ToPort", "8080");
		m2.addAttribute("FromAddr", "localhost");
		c1.sendMsg(m2);
		HTTPMessage m3;
		m3.addAttribute("Command", "ADD DEPENDENCY");
		m3.addAttribute("Dependency", "ClientRequirement3.h");
		m3.addAttribute("Content", "ClientRequirement4.h");
		m3.addAttribute("ToPort", "8080");
		m3.addAttribute("FromAddr", "localhost");
		c1.sendMsg(m3);
		HTTPMessage m4;
		m4.addAttribute("Command", "DISPLAY METADATA");
		m4.addAttribute("Content", "ClientRequirement4.h");
		m4.addAttribute("ToPort", "8080");
		m4.addAttribute("FromAddr", "localhost");
		c1.sendMsg(m4);
		std::thread t1(
			[&]() { c1.execute(); }
		);
		t1.join();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n";
	}
}
//----< Requirement seven demo >-------------------------------------

void RemoteCodeRepositoryExecutive::RequirementSeven(std::string p)
{
	std::cout << "\n\n\n";
	std::cout << "//////////////////////////////////\n";
	std::cout << "// Demo for Requirement SEVEN. //" << std::endl;
	std::cout << "/////////////////////////////////\n\n";
	std::cout << "Upload Requirement6.h and Requirement6.cpp to server. \n";
	std::cout << "Assuming package Requirement6 depends on package Requirement3. \n";
	std::cout << "Extract Requirement6.cpp with dependency. \n";
	std::cout << "---------------------------------------------\n";
	withDep(p);
	std::cout << "Extract Requirement6.cpp without dependency. \n";
	std::cout << "------------------------------------------------\n";
	withoutDep(p);
}
//----< helper function for R7 extraction with dependency >----------

void RemoteCodeRepositoryExecutive::withDep(std::string p)
{
	try {
		ClientSenderHandler c2;
		HTTPMessage m1;
		m1.addAttribute("Command", "UPLOAD");
		m1.addAttribute("Content", "ClientRequirement6.h");
		m1.addAttribute("ToPort", "8080");
		m1.addAttribute("FromAddr", "localhost");
		c2.sendMsg(m1);
		HTTPMessage m2;
		m2.addAttribute("Command", "UPLOAD");
		m2.addAttribute("Content", "ClientRequirement6.cpp");
		m2.addAttribute("ToPort", "8080");
		m2.addAttribute("FromAddr", "localhost");
		c2.sendMsg(m2);
		HTTPMessage m3;
		m3.addAttribute("Command", "ADD DEPENDENCY");
		m3.addAttribute("Dependency", "ClientRequirement3.h");
		m3.addAttribute("Content", "ClientRequirement6.h");
		m3.addAttribute("ToPort", "8080");
		m3.addAttribute("FromAddr", "localhost");
		c2.sendMsg(m3);
		HTTPMessage m4;
		m4.addAttribute("Command", "GET FILELISTS");
		m4.addAttribute("Content", "ClientRequirement6.cpp");
		m4.addAttribute("WithDep", "true");
		m4.addAttribute("ToPort", "8080");
		m4.addAttribute("FromAddr", "localhost");
		c2.sendMsg(m4);
		std::thread t2(
			[&]() { c2.execute(); }
		);
		t2.join();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n";
	}
}
//----< helper function for R7 extraction without dependency >-------

void RemoteCodeRepositoryExecutive::withoutDep(std::string p)
{
	try {
		ClientSenderHandler c2;
		HTTPMessage m1;
		m1.addAttribute("Command", "UPLOAD");
		m1.addAttribute("Content", "ClientRequirement6.h");
		m1.addAttribute("ToPort", "8080");
		m1.addAttribute("FromAddr", "localhost");
		c2.sendMsg(m1);
		HTTPMessage m2;
		m2.addAttribute("Command", "UPLOAD");
		m2.addAttribute("Content", "ClientRequirement6.cpp");
		m2.addAttribute("ToPort", "8080");
		m2.addAttribute("FromAddr", "localhost");
		c2.sendMsg(m2);
		HTTPMessage m3;
		m3.addAttribute("Command", "ADD DEPENDENCY");
		m3.addAttribute("Dependency", "ClientRequirement3.h");
		m3.addAttribute("Content", "Requirement6.h");
		m3.addAttribute("ToPort", "8080");
		m3.addAttribute("FromAddr", "localhost");
		c2.sendMsg(m3);
		HTTPMessage m4;
		m4.addAttribute("Command", "GET FILELISTS");
		m4.addAttribute("Content", "ClientRequirement6.cpp");
		m4.addAttribute("WithDep", "false");
		m4.addAttribute("ToPort", "8080");
		m4.addAttribute("FromAddr", "localhost");
		c2.sendMsg(m4);
		std::thread t2(
			[&]() { c2.execute(); }
		);
		t2.join();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n";
	}
}
//----< Requirement eight and nine demo >----------------------------

void RemoteCodeRepositoryExecutive::RequirementEightandNine(std::string p)
{
	std::cout << "\n\n\n";
	std::cout << "//////////////////////////////////////////\n";
	std::cout << "// Demo for Requirement EIGHT and NINE. //" << std::endl;
	std::cout << "//////////////////////////////////////////\n\n";
	std::cout << "Client1 and Client2 are both sending HTTP messages to Server. \n";
	std::cout << "Client1 upload Requirement8.h to Server(8080). \n";
	std::cout << "Client2 upload Requirement9.cpp to Server(8080) asynchronously. \n";
	std::cout << "Server reply messages are below. \n";
	try {
		ClientSenderHandler c1;
		ClientSenderHandler c2;
		HTTPMessage m1;
		m1.addAttribute("Command", "UPLOAD");
		m1.addAttribute("Content", "ClientRequirement8.h");
		m1.addAttribute("ToPort", "8080");
		m1.addAttribute("FromAddr", "localhost");
		std::cout << "HTTP message one: ";
		m1.showAttributes();
		HTTPMessage m2;
		m2.addAttribute("Command", "UPLOAD");
		m2.addAttribute("Content", "ClientRequirement9.cpp");
		m2.addAttribute("ToPort", "8080");
		m2.addAttribute("FromAddr", "localhost");
		std::cout << "HTTP message two: ";
		m2.showAttributes();
		c1.sendMsg(m1);
		c2.sendMsg(m2);
		std::thread t1(
			[&]() { c1.execute(); }
		);
		std::thread t2(
			[&]() { c2.execute(); }
		);
		t1.join();
		t2.join();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n";
	}

}
//----< Requirement ten demo >---------------------------------------

void RemoteCodeRepositoryExecutive::RequirementTen(std::string p)
{
	std::cout << "\n\n\n";
	std::cout << "///////////////////////////////\n";
	std::cout << "// Demo for Requirement TEN. //" << std::endl;
	std::cout << "///////////////////////////////\n\n";
	std::cout << "ClientRequirement10.cpp is uploaded to ../Test/ServerTest/ \n";
	std::cout << "ServerRequirement10.cpp is downloaded to ../Test/ClietnTest/ \n";
	try{
		ClientSenderHandler c;
		HTTPMessage m1;
		m1.addAttribute("Command", "UPLOAD");
		m1.addAttribute("Content", "ClientRequirement10.cpp");
		m1.addAttribute("ToPort", "8080");
		m1.addAttribute("FromAddr", "localhost");
		HTTPMessage m2;
		m2.addAttribute("Command", "DOWNLOAD");
		m2.addAttribute("Content", "ServerRequirement10.cpp");
		m2.addAttribute("ToPort", "8080");
		m2.addAttribute("FromAddr", "localhost");
		c.sendMsg(m1);
		c.sendMsg(m2);
		std::thread t1(
			[&]() { c.execute(); }
		);
		t1.join();
	}
	catch(std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n";
	}
}
//----< Requirement eleven demo >------------------------------------

void RemoteCodeRepositoryExecutive::RequirementEleven(std::string p)
{
	std::cout << "\n\n\n";
	std::cout << "//////////////////////////////////\n";
	std::cout << "// Demo for Requirement ELEVEN. //" << std::endl;
	std::cout << "//////////////////////////////////\n";
	std::cout << "Requirement Eleven demo can also be done by using GUI. \n";
}
//----< test stub >--------------------------------------------------

#ifdef TEST_REMOTECODEREPOSITORYEXECUTIVE

int main(int argc, char* argv[])
{
	try {
		RemoteCodeRepositoryExecutive rcrEecutive;
		rcrEecutive.RequirementOneandTwo(argv[1]);
		rcrEecutive.RequirementThreeandFive(argv[1]);
		rcrEecutive.RequirementFour(argv[1]);
		rcrEecutive.RequirementSeven(argv[1]);
		rcrEecutive.RequirementEightandNine(argv[1]);
		rcrEecutive.RequirementTen(argv[1]);
		rcrEecutive.RequirementEleven(argv[1]);
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n";
	}
	return 0;
}

#endif



