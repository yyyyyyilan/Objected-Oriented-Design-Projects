/////////////////////////////////////////////////////////////////////
// Requirement9.cpp -   test requirement 9                         //
// ver 1.0                                                         //
// Application: CSE687 - Object Oriented Design                    //
//              Project 4 Dependency-Based Remote Code Repository  //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education		   //
// Author:		Yilan Li, Syracuse University, (315)382-9302	   //
//				yli41@syr.edu									   //
/////////////////////////////////////////////////////////////////////

#include "Requirement9.h"

Requirement3::Requirement9()
{}

Requirement3::~Requirement9()
{}

void Requirement9::R9test()
{
	std::cout << "This is test of requirement six. \n";
}


int main()
{
	Requirement9 r9;
	r9.R9test();
	return 0;
}