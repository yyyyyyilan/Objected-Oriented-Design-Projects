///////////////////////////////////////////////////////////////////////
// Utilities.cpp - small, generally usefule, helper classes          //
// ver 1.1                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include <cctype>
#include <iostream>
#include "Utilities.h"

#include "utilities.h"
#include <iostream>

void title(const std::string& text, char underline)
{
	std::cout << "\n  " << text;
	std::cout << "\n " << std::string(text.size() + 2, underline);
	if (underline == '=')
		std::cout << "\n";
}

void logit::show()
{
	std::cout << "\n--start of log:";
	std::cout << out_.str() << "\n--end of log\n";
	out_.clear();
}

void logit::start() { running_ = true; }

void logit::stop() { running_ = false; }

void logit::clear() { out_.str(""); }

bool logit::running() { return running_; }

void logit::put(char ch) { out_.put(ch); }

logit& operator<<(logit& out, const std::string& in)
{
	if (out.running())
		for (auto ch : in)
			out.put(ch);
	return out;
}

logit mtLog;

Cosmetic::~Cosmetic() { std::cout << "\n\n"; }

Cosmetic cosmetic;

//#define TEST_UTILITIES
#ifdef TEST_UTILITIES

int main()
{
	Title("Testing Utilities Package");
	putline();

	title("test StringHelper::split(std::string)");

	std::string test = "a, \n, bc, de, efg, i, j k lm nopq rst";
	std::cout << "\n  test string = " << test;

	std::vector<std::string> result = StringHelper::split(test);

	std::cout << "\n";
	for (auto item : result)
	{
		if (item == "\n")
			std::cout << "\n  " << "newline";
		else
			std::cout << "\n  " << item;
	}
	std::cout << "\n";

	title("test std::string Converter<T>::toString(T)");

	std::string conv1 = Converter<double>::toString(3.1415927);
	std::string conv2 = Converter<int>::toString(73);
	std::string conv3 = Converter<std::string>::toString("a_test_string plus more");

	std::cout << "\n  Converting from values to strings: ";
	std::cout << conv1 << ", " << conv2 << ", " << conv3;
	putline();

	title("test T Converter<T>::toValue(std::string)");

	std::cout << "\n  Converting from strings to values: ";
	std::cout << Converter<double>::toValue(conv1) << ", ";
	std::cout << Converter<int>::toValue(conv2) << ", ";
	std::cout << Converter<std::string>::toValue(conv3);

	std::cout << "\n\n";
	return 0;
}
#endif
