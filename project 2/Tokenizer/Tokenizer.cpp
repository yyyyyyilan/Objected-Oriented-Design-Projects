/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 3.10                                                         //
// Language:  C++, Visual Studio 2015 Community                    //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:		Yilan Li, Syracuse University, (315)382-9302       //
//				yli41@syr.edu									   //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education		   //
// Source:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////

/*
  Helper code that does not attempt to handle all tokenizing
  special cases like escaped characters.
*/
#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <fstream>
#include <vector>

/////////////////////////////////////////////////////////////////////
// Private abstract class ConsumeState

namespace Scanner
{
	class ConsumeState
	{
	public:
		ConsumeState();
		virtual ~ConsumeState();
		void attach(std::istream* pIn) { _pIn = pIn; }
		virtual void eatChars() = 0;
		void consumeChars() {
			_pState->eatChars();
			_pState = nextState();
		}
		bool canRead() { return _pIn->good(); }
		std::string getTok() { return token; }
		bool hasTok() { return token.size() > 0; }
		ConsumeState* nextState();
		void SetCommentOption(bool op) { Comment = op; }
		void setSpecialSingleChar(std::vector<std::string> ssc) { SingleChar = ssc; }
		void setSpecialCharPairs(std::vector<std::string> scp) { CharPairs = scp; }
		void setInitial(bool cons, bool des) { Constructor = cons, Destructor = des; }
		size_t LineCounter() { return line_counter; }
		void SetLine(size_t number) { line_counter = number; };
	protected:
		static std::string token;
		static std::istream* _pIn;
		static int prevChar;
		static int currChar;
		static ConsumeState* _pState;
		static ConsumeState* _pEatCppComment;
		static ConsumeState* _pEatCComment;
		static ConsumeState* _pEatWhitespace;
		static ConsumeState* _pEatPunctuator;
		static ConsumeState* _pEatAlphanum;
		static ConsumeState* _pEatNewline;
		static ConsumeState* _pEatSpecialSingleChar;
		static ConsumeState* _pEatSpecialTwoChars;
		static ConsumeState* _pEatQuotedString;	
		bool issingle(char single);
		bool istwo(char curr, char chnext);
		void DoubleQuote();
		void SingleQuote();
		static bool Comment;
		void DisplayComment(bool c_op);
		static std::vector<std::string> SingleChar;
		static std::vector<std::string> CharPairs;
		static bool Constructor;
		static bool Destructor;
		static size_t line_counter;
		bool CalculateLineNumber();
	};
}
//----< define a namespace Scanner>----------------------------------

using namespace Scanner;		
//----< initialize static data members >-----------------------------

std::string ConsumeState::token;
std::istream* ConsumeState::_pIn;
int ConsumeState::prevChar;
int ConsumeState::currChar;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatNewline;
ConsumeState* ConsumeState::_pEatSpecialSingleChar = nullptr;
ConsumeState* ConsumeState::_pEatSpecialTwoChars = nullptr;
ConsumeState* ConsumeState::_pEatQuotedString = nullptr;
bool ConsumeState::Comment = true;
std::vector<std::string> ConsumeState::SingleChar = { "[","]","{","}","(",")","<",">",":",";","+","-","=","*","\"","\'","\n" };
std::vector<std::string> ConsumeState::CharPairs = { "<<",">>","+=","-=","*=","/=","::","==","++","--" };
bool ConsumeState::Constructor = true;
bool ConsumeState::Destructor = true;
//size_t ConsumeState::line_counter = 0;
size_t ConsumeState::line_counter = 1;

//----< function to test state transition >--------------------------

void testLog(const std::string& msg);
//----< function to determine whether it's a special single char >---

bool ConsumeState::issingle(char single)
{
	std::string temp;
	temp.push_back(single);
	for (size_t i = 0; i < SingleChar.size(); ++i)
		if (SingleChar[i] == temp)
			return true;
	return false;
}
//----< function to determine whether it's a special two chars >-----

bool ConsumeState::istwo(char curr, char chnext)
{
	std::string temp;
	temp.push_back(curr);
	temp.push_back(chnext);
	for (size_t i = 0; i < CharPairs.size(); ++i)
		if (CharPairs[i] == temp)
			return true;
	return false;
}
//----< function to each double quote string >-----------------------

void ConsumeState::DoubleQuote()
{
	do {
		prevChar = currChar;
		token += prevChar;
		if (!CalculateLineNumber())
		//if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		if ((currChar == '\\') && (prevChar == '\\'))
		{
			currChar = _pIn->get();
			break;
		}
	} while (!((currChar == '\"') && (prevChar != '\\')));
	if (!((currChar == '\"') && (prevChar != '\\')))
	{
		token += prevChar;
	}
}
//----< function to each single quote string >-----------------------

void ConsumeState::SingleQuote()
{
	do {
		prevChar = currChar;
		token += prevChar;
		if (!CalculateLineNumber())
		//if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		if ((currChar == '\\') && (prevChar == '\\'))
		{
			currChar = _pIn->get();
			break;
		}
	} while (!((currChar == '\'') && (prevChar != '\\')));
	if (!((currChar == '\'') && (prevChar != '\\')))
	{
		token += prevChar;
	}
}
//----< function to display comment when comment isn't returned >----

void  ConsumeState::DisplayComment(bool c_op)
{
	if (c_op)
		std::cout << "\n  comment \n";
}
//----< function to calculate the line number of the file >----------

bool ConsumeState::CalculateLineNumber()
{
	if (!(_pIn->good()))		// end of stream
		return false;
	if (currChar == '\n')
		line_counter = line_counter + 1;
	return true;
}
//----< transition to next state >-----------------------------------

ConsumeState* ConsumeState::nextState()
{
  if (!(_pIn->good()))
	return nullptr;
  int chNext = _pIn->peek();
  if (chNext == EOF)
  {
	  _pIn->clear();
	  // if peek() reads end of file character, EOF, then eofbit is set and
	  // _pIn->good() will return false.  clear() restores state to good
  }
  if (std::isspace(currChar) && currChar != '\n')
  {
    testLog("state: eatWhitespace");
	return _pEatWhitespace;
  }
  if (currChar == '/' && chNext == '/')
  {
    testLog("state: eatCppComment");
	return _pEatCppComment;
  }
  if (currChar == '/' && chNext == '*')
  {
    testLog("state: eatCComment");
	return _pEatCComment;
  }
  if (currChar == '\n')
  {
    testLog("state: eatNewLine");
	return _pEatNewline;
  }
  if (std::isalnum(currChar))
  {
    testLog("state: eatAlphanum");
	return _pEatAlphanum;
  }
  if (istwo(currChar, chNext))
  {  
	  testLog("state: eatSpecialTwoChar");
	  return _pEatSpecialTwoChars;
  }
  else if (issingle(currChar))
  {
	  if ((currChar != '\"')&&(currChar != '\''))
		{
			testLog("state: eatSpecialSingleChar");
			return _pEatSpecialSingleChar;
		}
	  else
		{
			testLog("state: eatQuotedString");
			return _pEatQuotedString;
		}
  }
  if (ispunct(currChar))
  {
	  testLog("state: eatPunctuator");
	  return _pEatPunctuator;
  }
  if (!_pIn->good())
    return _pEatWhitespace;
  throw(std::logic_error("invalid type"));
}
/////////////////////////////////////////////////////////////////////
// Derived state that consumes white space and throws it away

class EatWhitespace : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
	prevChar = currChar;
    //std::cout << "\n  eating whitespace";
    do {
        if (!CalculateLineNumber())
		//if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (std::isspace(currChar) && currChar != '\n');
  }
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes C++ comment and throws it away
// - note that Project #1 requires you to have an option to
//   return comments

class EatCppComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
	prevChar = currChar;
    //std::cout << "\n  eating C++ comment";
	do {
		if (Comment == true)
			{
				token += currChar;
			}
		if (!CalculateLineNumber())
		//if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	} while (currChar != '\n');
	//DisplayComment(!Comment);
  }
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes C comment and throws it away
// - note that Project #1 requires you to have an option to
//   return comments

class EatCComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
	prevChar = currChar;
    //std::cout << "\n  eating C comment";
	do {
		if (Comment == true)
			{
				token += currChar;
			}
		if (!CalculateLineNumber())
		//if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	} while (currChar != '*' || _pIn->peek() != '/');
	if (Comment == true)
	{
		token += currChar;
		token += _pIn->peek();
	}
	_pIn->get();
	currChar = _pIn->get();
	//DisplayComment(!Comment);
  }
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes punctuators and returns as token
// - note that Project #1 requires you to return special punctuators
//   as single or two character tokens.  This class does not do that.

class EatPunctuator : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
	prevChar = currChar;
    //std::cout << "\n  eating punctuator";
    do {
      token += currChar;
	  if (!CalculateLineNumber())
	  //if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while ((ispunct(currChar)) && (!issingle(currChar)) && (!istwo(currChar, _pIn->peek())));
  }
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes alphanumerics and returns as token

class EatAlphanum : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
	prevChar = currChar;
    //std::cout << "\n  eating alphanum";
    do {
      token += currChar;
	  if (!CalculateLineNumber())
	  //if (!_pIn->good())  // end of stream
			return;
      currChar = _pIn->get();
    } while (isalnum(currChar));
  }
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes newline and returns as single
// character token.  You might extend this to return other single
// and two character tokens as required by Project #1.

class EatNewline : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
	prevChar = currChar;
    //std::cout << "\n  eating newline";
    token += currChar;
	if (!CalculateLineNumber())
	//if (!_pIn->good())  // end of stream
      return;
    currChar = _pIn->get();
  }
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes special single characters and returns
// as a token.

class EatSpecialSingleChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		prevChar = currChar;
		//std::cout << "\n  eating special single character"; 
		token += currChar;
		if (!CalculateLineNumber())
		//if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes special two characters and returns
// as a token.

class EatSpecialTwoChars : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		prevChar = currChar;
		//std::cout << "\n  eating special two characters"; 
		do {
			token += currChar;
			if (!CalculateLineNumber())
			//if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (istwo(currChar, _pIn->peek()));
		token += currChar;
		currChar = _pIn->get();
	}
};
/////////////////////////////////////////////////////////////////////
// Derived state that consumes quoted strings and returns  as a 
// token.

class EatQuotedString : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating quoted string";
		if ((currChar == '\"') && (prevChar != '\\'))
		{	
			DoubleQuote();
		}
		if ((currChar == '\'') && (prevChar != '\\'))
		{
			SingleQuote();
		}
		token += currChar;
		currChar = _pIn->get();	
	}
};
//----< base class member function to create derived states >--------

ConsumeState::ConsumeState()
{
  //static bool first = true;
  if (Constructor)
  {
	Constructor = false;
    _pEatAlphanum = new EatAlphanum();
    _pEatCComment = new EatCComment();
    _pEatCppComment = new EatCppComment();
    _pEatPunctuator = new EatPunctuator();
    _pEatWhitespace = new EatWhitespace();
    _pEatNewline = new EatNewline();
	_pEatSpecialSingleChar = new EatSpecialSingleChar();
	_pEatSpecialTwoChars = new EatSpecialTwoChars();
	_pEatQuotedString = new EatQuotedString();
    _pState = _pEatWhitespace;
  }
}
//----< remove derived states from native heap >---------------------

ConsumeState::~ConsumeState()
{
  if(Destructor)
  {
	Destructor = false;
    delete _pEatAlphanum;
    delete _pEatCComment;
    delete _pEatCppComment;
    delete _pEatPunctuator;
    delete _pEatWhitespace;
    delete _pEatNewline;
	delete _pEatSpecialSingleChar;
	delete _pEatSpecialTwoChars;
	delete _pEatQuotedString;
  }
}
//----< initialize the toker to start with EatWhitespace >-----------
Toker::Toker() : pConsumer(new EatWhitespace()) {}

//----< class Toker destructor >-------------------------------------

Toker::~Toker() { delete pConsumer; }

//----< function allows us to attach to a file or stringstream >-----

bool Toker::attach(std::istream* pIn)
{
	if (pIn != nullptr && pIn->good())
	{
		pConsumer->attach(pIn);
		return true;
	}
	return false;
}
//----< this method will be called by SemiExpression >---------------

std::string Toker::getTok()
{
  while(true) 
  {
    pConsumer->consumeChars();  // attempt to fill token - not all states do that
    if (pConsumer->hasTok())
      break;
    if (!pConsumer->canRead())   // end of stream (file or string)?
      return "";
  }
  return pConsumer->getTok();    // return filled token
}

//----< function tells us the input is available to be read >--------

bool Toker::canRead() { return pConsumer->canRead(); }

//----< function to test state transition >--------------------------

void testLog(const std::string& msg)
{
#ifdef TEST_LOG
	std::cout << "\n  " << msg;
#endif
}
//----< fundction to give an option to return comments or not>-------

bool Toker::SetCommentOption(bool op)
{
	pConsumer->SetCommentOption(op);
	return op;
}
//----< function to change default special single character tokens >-

std::vector<std::string> Toker::setSpecialSingleChar(std::vector<std::string> ssc)
{
	pConsumer->setSpecialSingleChar(ssc);
	return specialSingleChar;
}
//----< function to change default special two character tokens >----

std::vector<std::string> Toker::setSpecialCharPairs(std::vector<std::string> scp)
{
	pConsumer->setSpecialCharPairs(scp);
	return specialCharPairs;
}
//----< function to set initial value of static variables >----------

void Toker::setInitial(bool cons, bool des)
{
	pConsumer->setInitial(cons, des);
	pConsumer->SetLine(0);
}
//----< function to keep a record of line number >-------------------

size_t Toker::LineCounter()
{
	size_t line;
	line = pConsumer->LineCounter();
	return line;
}
//----< function to determine it's a comment token or not >----------

bool Toker::isCommentToken(const std::string& c_token)
{
	if (c_token.size() < 2)
		return false;
	else if ((c_token[0] == '/') && ((c_token[1] == '/') || (c_token[1] == '*')))
		return true;
	return false;
};
//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER
//----< test driver for returning comments as single token >---------

void ReturnCommentToken_TestDriver(Toker &toker, std::string f)
{
	std::ifstream in(f);
	if (!in.good())
	{
		std::cout << "\n  can't open " << f << "\n\n";
		//return 1;
	}
	toker.attach(&in);
	std::cout << "\n  current line count = " << toker.LineCounter();
	while (in.good())
	{
		toker.SetCommentOption(true);
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	}
	std::cout << "\n  current line count = " << toker.LineCounter();
	std::cout << "\n\n";
	in.seekg(0, std::ios::beg); //reset read pos to beginning
	in.clear();				  //clear eofbit
	in.close();
	toker.setInitial(true, true);
}
//----< test driver for not returning comments as single token >-----

void NotReturnCommentToken_TestDriver(Toker &toker, std::string f)
{
	std::ifstream in(f);
	if (!in.good())
	{
		std::cout << "\n  can't open " << f << "\n\n";
		//return 1;
	}
	toker.attach(&in);
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
	in.clear();				  //clear eofbit
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
		toker.SetCommentOption(false);
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
//----< test driver for requirement4 >-------------------------------

void Test_Requirement4(std::string file)
{
	Toker toker1;
	std::cout << "--------Return Comment Content--------" << std::endl;
	ReturnCommentToken_TestDriver(toker1, file);
	Toker toker2;
	std::cout << "------Not Return Comment Content------" << std::endl;
	NotReturnCommentToken_TestDriver(toker2, file);
	Toker toker3;
	std::cout << "---------Change Special Token---------" << std::endl;
	ChangeSpecialToken_TestDriver(toker3, file);
}
//----< main function >----------------------------------------------

int main()
{  
	std::cout << "****************************************************************" << std::endl;
	std::cout << "*********************Test for Requirement 4*********************" << std::endl;
	std::cout << "****************************************************************" << std::endl;
	std::string fileSpec1 = "../Test/test_requirement4.txt";
	//std::string fileSpec1="../Project_1/Test/test_requirement4.txt"
	//std::string fileSpec1 = "../Test/quotetest.txt";
	Test_Requirement4(fileSpec1);

	return 0;
}
#endif
