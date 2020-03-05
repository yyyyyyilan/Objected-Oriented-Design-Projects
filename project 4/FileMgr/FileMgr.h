#ifndef FILEMGR_H
#define FILEMGR_H
/////////////////////////////////////////////////////////////////////
// FileMgr.h - find files matching specified patterns              //
//             on a specified path                                 //
// ver 2.0                                                         //
// Jim Fawcett, MidtermCodeSupplement, Spring 2016                 //
/////////////////////////////////////////////////////////////////////
/*
* ver 2.0 : 12 Mar 2016
* - fixes bug in directory recursion in find(path)
* - reduces scope of for loop in find for efficiency
* ver 1.0 : 11 Mar 2016
* - first release
*/

#include "IFileMgr.h"
#include "FileSystem.h"

namespace FileManager
{
  class FileMgr : public IFileMgr
  {
  public:
    using patterns = std::vector<std::string>;
    using fileSubscribers = std::vector<IFileEventHandler*>;
    using dirSubscribers = std::vector<IDirEventHandler*>;

    FileMgr(const std::string& path) : path_(path)
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
      for (auto pEvtHandler : dirSubscribers_)
      {
        pEvtHandler->execute(fpath);
      }
      for (auto patt : patterns_)
      {
        std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt);
        for (auto f : files)
        {
          for (auto pEvtHandler : fileSubscribers_)
          {
            pEvtHandler->execute(f);
          }
        }
      }
      std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fpath);
      for (auto d : dirs)
      {
        if (d == "." || d == "..")
          continue;
        std::string dpath = fpath + "\\" + d;
        find(dpath);
      }
    }

    void regForFiles(IFileEventHandler* pHandler)
    {
      fileSubscribers_.push_back(pHandler);
    }

    void regForDirs(IDirEventHandler* pHandler)
    {
      dirSubscribers_.push_back(pHandler);
    }
  private:
    std::string path_;
    patterns patterns_;
    fileSubscribers fileSubscribers_;
    dirSubscribers dirSubscribers_;
  };

  inline IFileMgr* FileMgrFactory::create(const std::string& path)
  {
    return new FileMgr(path);
  }
}
#endif
