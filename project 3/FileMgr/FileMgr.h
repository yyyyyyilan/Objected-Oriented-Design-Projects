#ifndef FILEMGR_H
#define FILEMGR_H
/////////////////////////////////////////////////////////////////////
// FileMgr.h - find files matching specified patterns              //
//             on a specified path                                 //
// ver 2.1                                                         //
// Application: Help Session Demo, Spring 2015                     //
//              CSE687 - Object Oriented Design                    //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education		   //
// Author:		Yilan Li, Syracuse University, (315)382-9302	   //
//				yli41@syr.edu									   //
// Source:      Jim Fawcett, CST 4-187, Syracuse University        //
//              (315) 443-3948, jfawcett@twcny.rr.com              //
/////////////////////////////////////////////////////////////////////

/*
 * Module Operations:
 * ==================
 * This module defines a FileMgr class. It navigates through a directory tree rooted
 * at some specified path and returns names of all the files and/or directories matching
 * a pattern.
 *
 * Build Command:
 * ====================
 * - devenv Project_3.sln /rebuild debug
 *
 * Maintenance History :
 * == == == == == == == == == ==
 * ver 2.1 : 27 Mar 2016 by Yilan Li
 * - added datamemeber BlockingQueue<std::string> blockingqueue and a public function 
 *   FileMgr::BlockingQueue<std::string> createQueue() to save file specs
 * ver 2.0 : 12 Mar 2016 by Jim Fawcett
 * - fixes bug in directory recursion in find(path)
 * - reduces scope of for loop in find for efficiency
 * ver 1.0 : 19 Feb 2016 by Jim Fawcett
 * - first release
 */
#include "FileSystem.h"
#include "../DataStore/DataStore.h"
#include "../BlockingQueue/BlockingQueue.h"
#include <iostream>

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
    std::string fpath = FileSystem::Path::getFullFileSpec(path);
    //---------------------------------------------------------
    // The statement above is not necessary, but helps to
    // make the processing clearer.  To show that, comment
    // it out and uncomment the following line.
    //
    // std::string fpath = path;
    //
    // Remove the std::cout statements below after you're 
    // convinced that everything works as it should.
    //---------------------------------------------------------

    for (auto patt : patterns_)  // the scope of this for loop was
    {                            // changed to improve efficiency
      std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt);
      for (auto f : files)
      {
		std::string filePath = fpath + "\\" + f;
		store_.save(filePath);
      }
    }
    std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fpath);
    for (auto d : dirs)
    {
      if (d == "." || d == "..")
        continue;
      std::string dpath = fpath + "\\" + d;  // here's the fix

      find(dpath);
    }
  }
	 
  BlockingQueue<std::string> createQueue(DataStore d)
  {
	  for (DataStore::iterator iter = d.begin(); iter != d.end(); ++iter)
	  {
		  blockingqueue.enQ(*iter);
	  }	  
	  return blockingqueue;
  }

private:
  std::string path_;
  DataStore& store_;
  patterns patterns_;
  BlockingQueue<std::string> blockingqueue;
};

#endif
