/////////////////////////////////////////////////////////////////////
// Requirement3.cpp -   test requirement 3                         //
// ver 1.0                                                         //
// Application: CSE687 - Object Oriented Design                    //
//              Project 4 Dependency-Based Remote Code Repository  //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education		   //
// Author:		Yilan Li, Syracuse University, (315)382-9302	   //
//				yli41@syr.edu									   //
/////////////////////////////////////////////////////////////////////

#include "Requirement3.h"

Requirement3::Requirement3()
{}

Requirement3::~Requirement3()
{}

void Requirement3::R3test()
{
	std::cout << "This is test of requirement three. \n";
}


int main()
{
	Requirement3 r3;
	r3.R3test();
	return 0;
}