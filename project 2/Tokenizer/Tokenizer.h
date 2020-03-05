#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream                       //
// ver 3.11                                                          //
// Language:  C++, Visual Studio 2015 Community                      //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			 //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:		Yilan Li, Syracuse University, (315)382-9302         //
//				yli41@syr.edu									     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a public Toker class and private ConsumeState class.
 * Toker reads words from a std::stream, throws away whitespace and comments
 * and returns words from the stream in the order encountered.  Quoted
 * strings and certain punctuators and newlines are returned as single tokens.
 *
 * This is a new version, based on the State Design Pattern.  Older versions
 * exist, based on an informal state machine design.
 *
 * Build Process:
 * --------------
 * Required Files: Tokenizer.h, Tokenizer.cpp
 * Build Command: devenv Project_2.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 3.11 : 19 Feb 2016 by Yilan Li
 * - Added function bool isCommentToken(const std::string& c_token) to determine the token is comment or not
 * ver 3.10 : 13 Feb 2016 by Yilan Li
 * - Added functions size_t Toker::LineCounter() and size_t ConsumeState::LineCounter()
 *   to keep a record of line number in the file
 * - Added a function bool ConsumeState::CalculateLineNumber() to calculate total number of lines 
 *   in the file
 * ver 3.9 : 07 Feb 2016 by Yilan Li
 * - Modified EatQuotedString state to return literal string token with quotes
 * - Fixed bug about static variable when tring to create several objects of the same class
 *   Added function void setInitial(bool cons, bool des)
 * ver 3.8 : 03 Feb 2016 by Jim Fawcett 
 * - fixed bug that prevented reading of last character in source by
 *   clearing stream errors at beginning of ConsumeState::nextState()
 * ver 3.7 : 02 Feb 2016 by Jim Fawcett
 * - declared ConsumeState copy constructor and assignment operator = delete
 * ver 3.6 : 02 Feb 2016 by Yilan Li
 * - Modified default special single character token set and special two character token set
 * ver 3.5 : 30 Jan 2016 by Yilan Li
 * - Added a public function bool SetCommentOption(bool op) and modified EatCComment state and EatCppComment states 
 *   both to have an option to return as single token
 * - Added EatQuotedString state to consume quoted strings as single token
 * - Merged EatNewline state into EatSpecialSingleChar state
 * ver 3.4 : 29 Jan 2016 by Yilan Li
 * - Added two functions in ConsumeState class which can be called to change special character tokens   
 *   std::string setSpecialSingleChar(std::string ssc)
 *   std::string setSpecialCharPairs(std::string scp)
 * - Updated issingle and istwo functions without hardcoding
 * ver 3.3 : 28 Jan 2016 by Yilan Li
 * - Added SpecialSingleChar state that consumes special single characters
 *   such as ", ', <, >, [, ], :.
 * - Added two protected functions in ConsumeState class:
 *	 (1) bool issingle(char single) to determine whether it's a character is a special single character or not
 *   (2) bool istwo(char curr, char chnext) to determine whether it's a special two characters or not.
 * ver 3.2 : 28 Jan 2016 by Jim Fawcett
 * - fixed bug in ConsumeState::nextState() by returning a valid state
 *   pointer if all tests fail due to reaching end of file instead of
 *   throwing logic_error exception.
 * ver 3.1 : 27 Jan 2016 Jim Fawcett
 * - fixed bug in EatCComment::eatChars()
 * - removed redundant statements assigning _pState in derived eatChars() 
 *   functions
 * - removed calls to nextState() in each derived eatChars() and fixed
 *   call to nextState() in ConsumeState::consumeChars()
 * ver 3.0 : 11 Jun 2014 Jim Fawcett
 * - first release of new design
 *
 */
#include <iosfwd>
#include <string>
#include <vector>

namespace Scanner
{
	class ConsumeState;

	class Toker
	{
	public:
		Toker();
		Toker(const Toker&) = delete;
		~Toker();
		bool attach(std::istream* pIn);
		std::string getTok();
		bool canRead();
		bool SetCommentOption(bool op);	
		std::vector<std::string> setSpecialSingleChar(std::vector<std::string> ssc);
		std::vector<std::string> setSpecialCharPairs(std::vector<std::string> scp);
		void setInitial(bool cons, bool des);
		size_t LineCounter();
		bool isCommentToken(const std::string& c_token);
	private:
		ConsumeState* pConsumer;
		std::vector<std::string> specialSingleChar;
		std::vector<std::string> specialCharPairs; 
	};
}

#endif

