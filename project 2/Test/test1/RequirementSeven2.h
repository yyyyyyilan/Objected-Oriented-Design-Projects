#include <iostream>
#include <string>

//----< parse the SemiExp by applying all rules to it >--------
bool Parser::parse()
{
	bool succeeded = false;
	for (size_t i = 0; i < rules.size(); ++i)
	{
		if (rules[i]->doTest(pTokColl))
		{
			succeeded = true;
		}
	}
	return succeeded;
}

int main(int argc, char* argv[])
{
  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
	for (int i = 0; i < 10; i++)
	{
		i = i * 2;
	}
    return 1;
  }
}

#endif
