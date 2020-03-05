///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 3.7                                                           //
//  Language:      C++, Visual Studio 2015 Community                 //
//  Platform:	   Dell Inspiron 13-7352, Windows 10 Education	     //
//  Application:   Parser component, CSE687 - Object Oriented Design //
//  Author:		   Yilan Li, Syracuse University, (315)382-9302      //
//				   yli41@syr.edu								     //
//  Source:        Jim Fawcett, Syracuse University, CST 4-187       //
//                 jfawcett@twcny.rr.com                             //
///////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

//----< define a namespace Scanner>----------------------------------

using namespace Scanner;
//----< initialize static data members >-----------------------------

bool SemiExp::CommentSemiExp = false;
//----< class SemiExp constructor to initialize data members>--------

SemiExp::SemiExp(Toker* pToker) : _pToker(pToker), TerminatorSet("{};"),
								SpecialTermintor("public,protected,private"),
								NewlineOption(false)
{
}
//----< class SemiExp destructor>------------------------------------

SemiExp::~SemiExp()
{
}
//----< function to handling for(;;) expression >--------------------

bool SemiExp::GetStart()
{
	bool temp = get(true);
	if (temp)
	{
		if ((find("for") >= 0) && (find("(") - 1 == find("for")))
		{
			get(false);
			get(false);
			if ((find("(") < find(";")) && (find(";") < find(")")))
				return true;
		}
		else
		{
			return true;
		}
	}
	return false;
}
//----< function to collect tokens as one semiexression >------------

bool SemiExp::get(bool clear)
{
  if (_pToker == nullptr)
	throw(std::logic_error("no Toker reference"));
  if (clear)
	_tokens.clear();
  Token token;
  do
  {
	if (!_pToker->canRead())	//end of file or not?
	{
	  if (length() > 0)
		  return true;
	  return false;
	}
	token = _pToker->getTok();
	if (token == "")
		break;
	if (token!="\n")
		_tokens.push_back(token);
	//if (token == "\n")
		//return true;
  } while (!isTerminator(token));
  trimFront();
  return true;
}
//----< index operator to find a particular token >------------------

Token& SemiExp::operator[](int n)
{
  if (n < 0 || (size_t)n >= _tokens.size())
    throw(std::invalid_argument("index out of range"));
  return _tokens[n];
}
//----< function to get the size of a token >------------------------

size_t SemiExp::length()
{
  return _tokens.size();
}
//----< function to return the index of a token in token vectors>----

size_t SemiExp::find(const Token& token)
{
	for (size_t i = 0; i < _tokens.size(); i++)
	{
		if (token == _tokens[i])
			return i;
	}
	return length();
}
//----< function to push back a token>-------------------------------

void SemiExp::push_back(const Token& token)
{
	_tokens.push_back(token);
}
//----< function to remove a particular token >----------------------

bool SemiExp::remove(size_t s)
{
	if (s<0 || _tokens.size() <= s)
		return false;
	std::vector<Token>::iterator it = _tokens.begin();
	_tokens.erase(it + s);
	return true;
}
//----< function to tokens from semiexpression >---------------------

bool SemiExp::remove(const Token& token)
{
	std::vector<Token>::iterator it;
	it = std::find(_tokens.begin(), _tokens.end(), token);
	if (it != _tokens.end())
	{
		_tokens.erase(it);
		return true;
	}
	return true;
}
//----< function to display  the vector tokens>----------------------

Token SemiExp::show(bool showNewLines)
{
  std::cout << "\n  ";
  if (_tokens.size() == 0)
	  return "";
  Token t;
  for (auto token : _tokens)
  {
	  if (token != "\n" || showNewLines)
	  {
		  t.append(" ").append(token);
		  std::cout << token << " ";
	  }
  }
  std::cout << "\n";
  return t;
}
//----< function to clear token from semiexpression>-----------------

void SemiExp::clear()
{
	_tokens.clear();
}
//----< function to remove leading newlines >------------------------

void SemiExp::trimFront()
{
		while (_tokens.size() > 0 && ((_tokens[0] == "\n") || (_tokens[0] == "")))
		{
			remove(0);
		}
}
//----< function to change tokens to lowercase >--------------------

void SemiExp::toLower()
{
	for (size_t i = 0; i < length(); i++)
	{
		for (size_t j = 0; j < ((*this)[i].length()); j++)
		{
			(*this)[i][j] = tolower((*this)[i][j]);
		}
	}
}
//----< function to judge whether a token is a terminator>-----------

bool SemiExp::isTerminator(Token token)
{
	if ((TerminatorSet.find(token) != Token::npos) || isSpecialTerminator(token))
	{
		return true;
	}
	if (token == "\n")
	{
		size_t s = find("#");
		if ((isFindPoundSign(_tokens)) && (s==0))
			return true;
	}
	if (isCommentSemiExp(token))
		return true;
	return false;
}
//----< function to judge whether a token is a special terminator>---

bool SemiExp::isSpecialTerminator(Token token)
{	
	int t = find(token) - 1;
	if (token == ":")
	{
		if (SpecialTermintor.find(_tokens[t]) != Token::npos)
		{
			return true;
		}
	}
	return false;
}
//----< function to return newline >---------------------------------

void SemiExp::ReturnNewline(bool newline)
{
	NewlineOption = newline;
}
//----< function to find # >-----------------------------------------

bool SemiExp::isFindPoundSign(std::vector<Token> Ptoken)
{
	char PoundSign = '#';
	for (size_t i = 0; i < length(); i++)
	{
		for (size_t j = 0; j < ((*this)[i].length()); j++)
		{
			if (Ptoken[i][j] == PoundSign)
				return true;
		}
	}
	return false;
}
//----< function to determine whether the token is comment or not >--

bool SemiExp::isCommentSemiExp(Token c_token)
{
	if (c_token.size() < 2)
		return false;
	else if ((c_token[0] == '/') && ((c_token[1] == '/') || (c_token[1] == '*')))
		return true;
	return false;
}
//----< function to return commnets as semiexpression >--------------

bool SemiExp::SetCommentSemiExpOption(bool op)
{
	 return (_pToker->SetCommentOption (op));
}
//----< test stub >--------------------------------------------------

#define TEST_SEMIEXP
#ifdef TEST_SEMIEXP
//----< test driver for showing each tokenizer with comment >--------

void TokenDisplayWithComment_TestDriver(Toker &toker, std::string f)
{
	std::ifstream in(f);
	toker.attach(&in);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << f << "\n\n";
	}
	while (in.good())
	{
		toker.SetCommentOption(true);
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	}
	std::cout << "\n\n";
	in.seekg(0, std::ios::beg); //reset read pos to beginning
	in.clear();				    //clear eofbit
	in.close();
	toker.setInitial(true, true);
}
//----< test driver for showing each tokenizer without comment >-----

void TokenDisplayWithoutComment_TestDriver(Toker &toker, std::string f)
{
	std::ifstream in(f);
	toker.attach(&in);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << f << "\n\n";
	}
	while (in.good())
	{
		toker.SetCommentOption(false);
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	}
	std::cout << "\n\n";
	in.seekg(0, std::ios::beg); //reset read pos to beginning
	in.clear();				    //clear eofbit
	in.close();
	toker.setInitial(true, true);
}
//----< test driver for change special character tokens >------------

void ChangeSpecialToken_TestDriver(Toker &toker, std::string f)
{
	std::ifstream in(f);
	if (!in.good())
	{
		std::cout << "\n  can't open " << f << "\n\n";
		//return 1;
	}
	toker.attach(&in);
	toker.setSpecialSingleChar({ ":",",",";","+","-","=","*","\"","\'","\n" });
	std::cout << "special single character tokens changed to: " << ":,;,+,-,=,*,\",\',\n" << std::endl;
	toker.setSpecialCharPairs({ "+=","-=","*=","/=","::","==","++","--" });
	std::cout << "special two character tokens changed to: " << "+=,-=,*=,/=,::,==,++,--" << std::endl;
	while (in.good())
	{
		toker.SetCommentOption(true);
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	}
	std::cout << "\n\n";
	in.seekg(0, std::ios::beg); //reset read pos to beginning
	in.clear();				   //clear eofbit
	in.close();
	toker.setInitial(true, true);
}
//----< test driver for returning comments as SemiExpression >-------

void ReturnComment_TestDriver(Toker &toker, std::string file)
{
	std::fstream in(file);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << file << "\n\n";
	}
	toker.attach(&in);
	SemiExp semi(&toker);
	semi.SetCommentSemiExpOption(true);
	while (semi.GetStart())
	{
		std::cout << "\n -- semiExpression --";
		semi.show(true);
	}
	/*
	May have collected tokens, but reached end of stream
	before finding SemiExp terminator.
	*/
	if (semi.length() > 0)
	{
		std::cout << "\n -- semiExpression --";
		semi.show(true);
		std::cout << "\n\n";
	}
	std::cout << "\n\n";
	in.seekg(0, std::ios::beg); //reset read pos to beginning
	in.clear();				    //clear eofbit
	in.close();
	toker.setInitial(true, true);
}
//----< test driver for not returning comments as SemiExpression >---

void NotReturnComment_TestDriver(Toker &toker, std::string file)
{
	std::fstream in(file);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << file << "\n\n";
	}
	toker.attach(&in);
	SemiExp semi(&toker);
	semi.SetCommentSemiExpOption(false);
	while (semi.GetStart())
	{
		std::cout << "\n -- semiExpression --";
		semi.show(true);
	}
	/*
	May have collected tokens, but reached end of stream
	before finding SemiExp terminator.
	*/
	if (semi.length() > 0)
	{
		std::cout << "\n -- semiExpression --";
		semi.show(true);
		std::cout << "\n\n";
	}
	std::cout << "\n\n";
	in.seekg(0, std::ios::beg); //reset read pos to beginning
	in.clear();				    //clear eofbit
	in.close();
	toker.setInitial(true, true);
}
//----< test driver for requirement4 >-------------------------------

void Test_Requirement4(std::string file)
{
	Toker toker1;
	std::cout << "--------Return Comment Content--------" << std::endl;
	TokenDisplayWithComment_TestDriver(toker1, file);
	Toker toker2;
	std::cout << "------Not Return Comment Content------" << std::endl;
	TokenDisplayWithoutComment_TestDriver(toker2, file);
	Toker toker3;
	std::cout << "---------Change Special Token---------" << std::endl;
	ChangeSpecialToken_TestDriver(toker3, file);
}
//----< test driver for requirement7 >-------------------------------

void Test_Requirement7(std::string file)
{
	Toker toker;
	ReturnComment_TestDriver(toker, file);
}
//----< test driver for requirement8 >-------------------------------

void Test_Requirement8(std::string file)
{
	Toker toker;
	ReturnComment_TestDriver(toker, file);
}
//----< main function >----------------------------------------------

int main()
{
  std::string fileSpec1 = "../Project_1/Test/test_requirement4.txt";
	//std::string fileSpec1 = "../Tokenizer/Tokenizer.cpp";
  std::cout << "****************************************************************" << std::endl;
  std::cout << "***********Test Requirement 4 for Showing Each Token************" << std::endl;
  std::cout << "****************************************************************" << std::endl;
  Test_Requirement4(fileSpec1);

  std::cout << "****************************************************************" << std::endl;
  std::cout << "***********************Test Requirement 7***********************" << std::endl;
  std::cout << "****************************************************************" << std::endl;
  std::string fileSpec2 = "../Project_1/Test/test_requirement7.txt";
  Test_Requirement7(fileSpec2);

  std::cout << "****************************************************************" << std::endl;
  std::cout << "***********************Test Requirement 8***********************" << std::endl;
  std::cout << "****************************************************************" << std::endl;
  std::string fileSpec3 = "../Project_1/Test/test_requirement8.txt";
  Test_Requirement8(fileSpec3);

  return 0;
}
#endif