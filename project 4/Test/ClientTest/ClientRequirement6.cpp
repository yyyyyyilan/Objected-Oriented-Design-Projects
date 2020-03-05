/////////////////////////////////////////////////////////////////////
// Requirement6.cpp -   test requirement 6                         //
// ver 1.0                                                         //
// Application: CSE687 - Object Oriented Design                    //
//              Project 4 Dependency-Based Remote Code Repository  //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education		   //
// Author:		Yilan Li, Syracuse University, (315)382-9302	   //
//				yli41@syr.edu									   //
/////////////////////////////////////////////////////////////////////

#include "Requirement6.h"

Requirement3::Requirement6()
{}

Requirement3::~Requirement6()
{}

void Requirement6::R6test()
{
	std::cout << "This is test of requirement six. \n";
}


int main()
{
	Requirement6 r6;
	r6.R6test();
	return 0;
}