#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
///////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis                    //
// ver 3.7                                                           //
//  Language:      C++, Visual Studio 2015 Community                 //
//  Platform:	   Dell Inspiron 13-7352, Windows 10 Education	     //
//  Application:   Parser component, CSE687 - Object Oriented Design //
//  Author:		   Yilan Li, Syracuse University, (315)382-9302      //
//				   yli41@syr.edu								     //
//  Source:        Jim Fawcett, Syracuse University, CST 4-187       //
//                 jfawcett@twcny.rr.com                             //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a 
* sequence of tokens that ends in {.  This will be extended to use the
* full set of terminators: {, }, ;, and '\n' if the line begins with #.
*
* This is a new version for students in CSE687 - OOD, Spring 2016 to use
* for Project #1.
*
* Build Process:
* --------------
* Required Files: 
*   itokcollection.h, SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp
* 
* Build Command: devenv Project_1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 3.7 : 08 Feb 2016 by Yilan Li
* - Fixed bug about static variale when trying to to creat several objects of same class
* ver 3.6 : 07 Feb 2016 by Yilan Li
* - Modified terminator set to deal with newline token which starts with #
* ver 3.5 : 03 Feb 2016 by Yilan Li
* - Implemented the interface ITokCollection with a declared method get()
* - Added a function GetStart() to handle for(;;) expression
* - Added functions bool isCommentSemiExp(Token c_token) and bool SetCommentSemiExpOption(bool op) 
*   to return comments if comments are returned as tokens
* ver 3.4 : 2 Feb 2016 by Jim Fawcett
* - declared SemiExp copy constructor and assignment operator = delete
* - added default argument for Toker pointer to nullptr so SemiExp
*   can be used like a container of tokens.
* - if pToker is nullptr then get() will throw logic_error exception
* ver 3.3 : 2 Feb 2016 by Yilan Li
* - Modified isTerminator function to ignore the (two) semicolons within parentheses in a 
*   for(;;) expression
* - Modified get() function to extract newline if a "#" is the first character on that line
* ver 3.2 : 31 Jan 2016 by Yilan Li
* - Added a function size_t find(std::string token) to get the token index
* - Added a function bool isTerminator(std::string token) to judge whether it's a terminator or not
* - Added a function bool isSpecialTerminator(std::string token) to judge whether it's a single token
*   ":" immediately preceeded by one of the tokens: "public", "protected", or "private".
* - Modified get() function accoding to the change of terminators
* - Added SemiExp dextructor ~SemiExp()
* ver 3.1 : 30 Jan 2016 Jim Fawcett
* - changed namespace to Scanner
* - fixed bug in termination due to continually trying to read
*   past end of stream if last tokens didn't have a semiExp termination
*   character
* ver 3.0 : 29 Jan 2016 Jim Fawcett
* - built in help session, Friday afternoon
*
* Planned Additions and Changes:
* ------------------------------  
* - move creation of tokenizer into semiExp constructor so
*   client doesn't have to write that code.
*
*/

#include <vector>
#include "itokcollection.h"
#include "../Tokenizer/Tokenizer.h"

namespace Scanner
{
  using Token = std::string;

  class SemiExp : public ITokCollection
  {
  public:
	SemiExp(Toker* pToker);
	SemiExp(const SemiExp&) = delete;
	SemiExp& operator=(const SemiExp&) = delete;
	~SemiExp();
	bool GetStart();
    bool get(bool clear = true);
	Token& operator[](int n);
    size_t length();
	size_t find(const Token& token);
	void push_back(const Token& token);
	bool remove(size_t s);
	bool remove(const Token& token);
	void toLower();
	void trimFront();
    Token show(bool showNewLines = false);
	void clear();
	bool SetCommentSemiExpOption(bool op);
	void ReturnNewline(bool newline = false);
  private:
    std::vector<Token> _tokens;
    Toker* _pToker;
	Token TerminatorSet;
	Token SpecialTermintor;
	bool isTerminator(Token token);
	bool isSpecialTerminator(Token specialToken);
	bool NewlineOption;
	bool isFindPoundSign(std::vector<Token> Ptoken);
	bool isCommentSemiExp(Token c_token);
	static bool CommentSemiExp;
  };
}
#endif
