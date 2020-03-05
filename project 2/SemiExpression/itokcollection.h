#ifndef ITOKCOLLECTION_H
#define ITOKCOLLECTION_H
///////////////////////////////////////////////////////////////////////
//  ITokCollection.h - package for the ITokCollection interface      //
//  ver 1.2                                                          //
//  Language:      C++, Visual Studio 2015 Community                 //
//  Platform:	   Dell Inspiron 13-7352, Windows 10 Education	     //
//  Application:   Parser component, CSE687 - Object Oriented Design //
//  Author:		   Yilan Li, Syracuse University, (315)382-9302      //
//				   yli41@syr.edu								     //
//  Source:        Jim Fawcett, Syracuse University, CST 4-187       //
//                 jfawcett@twcny.rr.com                             //
///////////////////////////////////////////////////////////////////////
/*
Module Purpose:
===============
ITokCollection is an interface that supports substitution of different
types of scanners for parsing.  In this solution, we illustrate that
by binding two different types of collections, SemiExp and XmlParts,
to this interface.  This is illustrated in the test stubs for the
SemiExpression and XmlElementParts modules.

Maintenance History:
====================
ver 1.2 : 07 Feb 2016 by Yilan Li
- Removed merge function
ver 1.1 : 02 Jun 11 by Jim Fawcett
- added merge, remove overload, and default param in get
ver 1.0 : 17 Jan 09 by Jim Fawcett
- first release
*/

struct ITokCollection
{
	virtual bool GetStart(bool clear = true) = 0;
	virtual size_t length() = 0;
	virtual std::string& operator[](size_t n) = 0;
	virtual size_t find(const std::string& tok) = 0;
	virtual void push_back(const std::string& tok) = 0;
	virtual bool remove(const std::string& tok) = 0;
	virtual bool remove(size_t i) = 0;
	virtual void toLower() = 0;
	virtual void trimFront() = 0;
	virtual void clear() = 0;
	virtual std::string show(bool showNewLines = false) = 0;
	virtual ~ITokCollection() {};
};

#endif
