/////////////////////////////////////////////////////////////////////
// ServerRequirement10.cpp -   test requirement 9                  //
// ver 1.0                                                         //
// Application: CSE687 - Object Oriented Design                    //
//              Project 4 Dependency-Based Remote Code Repository  //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education		   //
// Author:		Yilan Li, Syracuse University, (315)382-9302	   //
//				yli41@syr.edu									   //
/////////////////////////////////////////////////////////////////////


ServerRequirement10::ServerRequirement10()
{}

ServerRequirement10::~ServerRequirement10()
{}

void ServerRequirement10::ServerR10test()
{
	std::cout << "This is test of requirement six. \n";
}


int main()
{
	ServerRequirement10 sr10;
	sr10.ServerR10test();
	return 0;
}