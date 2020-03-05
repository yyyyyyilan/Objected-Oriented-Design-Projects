///////////////////////////////////////////////////////////////////
// ServerExecutive.cpp  - Project_4 automated unit test suite    //
// ver 1.0                                                       //
// Application: Remote Code Repository Executive                 //
// Language:    C++, Visual Studio 2015 Community                //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education	     //
// Author:		Yilan Li, Syracuse University, (315)382-9302     //
//				yli41@syr.edu									 //
///////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This package is the automated unit test suite. Its purpose is to implement
* the demo.
* Required Files:
* ===============
* ServerExecutive.cpp Server.h Server.cpp BlockingQueue.h BlockingQueue.cpp
* Channel.h Channel.cpp FileSystem.h FileSystem.cpp HTTPMessage.h HTTPMessage.cpp
* Logger.h Logger.cpp Socket.h Socket.cpp XmlElement.h XmlElement.cpp
*
* Build Command:
* ====================
* - devenv Project_4.sln /rebuild debug
*
* Maintenance History :
* ======================
* ver 1.0 : 1 May 2016
* - first release
*/

#include "../Sockets/Sockets.h"
#include "../FileMgr/FileSystem.h"
#include "../Server/Server.h"

//----< test stub >--------------------------------------------------

#ifdef TEST_SERVEREXECUTIVE

int main(int argc, char* argv[])
{
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