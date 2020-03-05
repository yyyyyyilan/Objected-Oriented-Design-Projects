#define TEST_SCOPETEST
#ifdef TEST_SCOPETEST

int main()
{
	int number = 4;
	if( number < 2)
	{
		std::cout << "This is a if loop" << std::endl;
		return 1;
	 }

	int sum = 0;
	for(int i=1; i<3; ++i)
	{
		sum = sum + i;
		std::cout << "This is a for loop." << std::endl;
		try
		{
			std::cout << "This is a try scope." << std::endl;
			if(true)
			{
				if(false)
				{
					std::cout << "This is another if scope."<< std::endl;
					continue;
				}
			}	
			else
			{
				std::cout << "This is an else scope. " << std::endl;
				return 1;
			}	
			while (number < 10)
			{
				number++;
			}
			std::cout << "While scope ends";

			int loop = 3;
			do { 
				std::cout << "This is a do while scope." << std::endl;
				loop++;
			}while (loop<=5)
		}
		catch(std::exception& ex)
		{
		std::cout << "This is catch scope. " << "\n";
		}
	 }
}

#endif
