#ifndef FILEMGR_H
#define FILEMGR_H
/////////////////////////////////////////////////////////////////////
// FileMgr.h - find files matching specified patterns              //
//             on a specified path                                 //
// ver 1.1                                                         //
// Language:  C++, Visual Studio 2015 Community                    //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education		   //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:		Yilan Li, Syracuse University, (315)382-9302       //
//				yli41@syr.edu									   //
// Source:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
//               Help Session Demo, Spring 2015                    //
/////////////////////////////////////////////////////////////////////

/*
* Build Command:
* ====================
* - devenv Project_2.sln /rebuild debu
* Maintenance History:
* ====================
* ver 1.1 11 Mar 2016 by Yilan Li
* - Fixed the bug by modifying void find(const std::string& path) function 
*   that can't find the whole files in sub-directories
*
*/

#include "FileSystem.h"
#include "../DataStore/DataStore.h"

class FileMgr
{
public:
  using iterator = DataStore::iterator;
  using patterns = std::vector<std::string>;

  FileMgr(const std::string& path, DataStore& ds) : path_(path), store_(ds)
  {
    patterns_.push_back("*.*");
  }

  void addPattern(const std::string& patt)
  {
    if (patterns_.size() == 1 && patterns_[0] == "*.*")
      patterns_.pop_back();
    patterns_.push_back(patt);
  }

  void search()
  {
    find(path_);
  }

  void find(const std::string& path)
  {
	  for (auto patt : patterns_)
	  {
		  std::vector<std::string> files = FileSystem::Directory::getFiles(path, patt);
		  for (auto f : files)
		  {
			  store_.save(path + "//" + f);
			  //store_.save(f);
		  }
	  }
		  std::vector<std::string> dirs = FileSystem::Directory::getDirectories(path);
		  for (auto d : dirs)
		  {
			  if (d == "." || d == "..")
				  continue;
			  find(path + "//" + d);
			  //find(d);
		  }

  }
private:
  std::string path_;
  DataStore& store_;
  patterns patterns_;
};

#endif
