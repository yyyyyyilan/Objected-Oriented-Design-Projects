/////////////////////////////////////////////////////////////////////////////////
// TypeAnalysis.h - Finds all the types defined in a C++ file                  //
// ver 1.0                                                                     //
// Application: Evaluate specified package                                     //
//              CSE687 - Object Oriented Design                                //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			           //
// Author:		Yilan Li, Syracuse University, (315)382-9302				   //
//				yli41@syr.edu												   //
/////////////////////////////////////////////////////////////////////////////////

#include "TypeAnalysis.h"
#include "../Parser/RulesAndActions.h"
#include "../Parser/ConfigureParser.h"


//----< Default TypeAnalysis constructor >---------------------------
TypeAnalysis::TypeAnalysis()
{
	symbolTable = new std::map<std::string, std::vector<std::string>>;
}
//----< construct TypeAnalysis from file name >----------------------

TypeAnalysis::TypeAnalysis(std::string f)
{
	pRepo = nullptr;
}

//----< Destructor >--------------------------------------------------
TypeAnalysis::~TypeAnalysis()
{}

//----< parse the input file >----------------------------------------
Repository TypeAnalysis::parse(std::string path)
{
	ConfigParseToConsole configure;
	Parser* pParser = configure.Build();
	if (pParser)
	{
		if (!configure.Attach(path))
		{
			std::cout << "\n  could not open file " << path << std::endl;
		}
	}
	else
		std::cout << "\n\n  Parser not built\n\n";
	while (pParser->next())
	{
		pParser->parse();
	}
	std::cout << "\n";
	pRepo = configure.getRepo();
	symbolTable = pRepo->getsymbolTable();
	return *pRepo;
}

//----< return symbol table of each file >---------------------------
Table TypeAnalysis::getTable()
{
	return *symbolTable;
}

//----< helper function to demo partial table >----------------------
void TypeAnalysis::displayTable()
{
	Table st = getTable();
	for (Table::iterator it = st.begin();
	it != st.end(); ++it)
	{
		std::cout << "[ TYPE, NAME, NAMESPACE ] : ";
		std::cout << it->first << "  ";
		std::vector<std::string> tn = it->second;
		for (size_t i = 0; i < tn.size(); i++)
		{
			std::cout << tn[i] <<"  ";
		}
		std::cout << "\n\n";
	}
}
//----< test stub >--------------------------------------------------

#ifdef TEST_TYPEANALYSIS

#include "../FileMgr/FileSystem.h"

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing TypeAnalysis class\n "
		<< std::string(32, '=') << std::endl;

	if (argc < 3)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}

	try {
		std::string file = "../Parser/ConfigureParser.h";
		std::cout << "\n  Processing file " << file;

		//create partial symbol table for each file
		TypeAnalysis typeAnalysis(file);
		Repository repository = typeAnalysis.parse(file);
		std::cout << "*********************\n";
		std::cout << "Partial Symbol Table" << "\n";
		std::cout << "*********************\n\n";
		typeAnalysis.displayTable();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
}

#endif