/////////////////////////////////////////////////////////////////////
// FileMgr.cpp - find files matching specified patterns            //
//               on a specified path                               //
// ver 2.1                                                         //
// Jim Fawcett, Help Session Demo, Spring 2015                     //
/////////////////////////////////////////////////////////////////////

#include "FileMgr.h"
#include "../DataStore/DataStore.h"
#include <iostream>

#ifdef TEST_FILEMGR

int main()
{
  std::cout << "\n  Testing FileMgr";
  std::cout << "\n =================";

  DataStore ds;
  FileMgr fm("..", ds);
  fm.addPattern("*.h");
  //fm.addPattern("*.c*");
  //fm.addPattern("*.partial");
  fm.search();

  std::cout << "\n\n  contents of DataStore";
  std::cout << "\n -----------------------";
  for (auto fs : ds)
  {
    std::cout << "\n  " << fs;
  }
  std::cout << "\n\n";

  std::cout << "\n\n  contents of blocking queue";
  std::cout << "\n -----------------------";
  BlockingQueue<std::string> bq;
  bq = fm.createQueue(ds);
  for (size_t i = 0; i < bq.size(); i++)
  {
	  std::string filepath = bq.deQ();
	  std::cout << "\n  File: " << filepath;
  }
  std::cout << "\n\n";
  return 0;
}
#endif
