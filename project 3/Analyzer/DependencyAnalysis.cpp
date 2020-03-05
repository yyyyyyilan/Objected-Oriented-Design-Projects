/////////////////////////////////////////////////////////////////////////////////
// DependencyAnalysis.cpp - Finds all file dependencies for a file             //
// ver 1.2                                                                     //
// Application: Evaluate specified package                                     //
//              CSE687 - Object Oriented Design                                //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			           //
// Author:		Yilan Li, Syracuse University, (315)382-9302				   //
//				yli41@syr.edu												   //
/////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <map>
#include "DependencyAnalysis.h"
#include "../Parser/RulesAndActions.h"
#include "../Parser/ConfigureParser.h"

using Vector = std::vector<std::string>;
using Table = std::map<std::string, Vector>;

//----< test stub >--------------------------------------------------

#ifdef TEST_DEPENDENCYANALYSIS
#include "../FileMgr/FileSystem.h"

int main()
{
	std::cout << "\n  Testing DependencyAnalysis class\n ";
	std::string firstfile = "../Test/RequirementSeven.cpp";
	std::string secondfile = "../Test/RequirementSeven.h";
		try {
			std::cout << "\n --- Processing file " << firstfile;
			TypeAnalysis typeAnalysis(firstfile);
			Repository repository = typeAnalysis.parse(firstfile);
			Table symboltable = typeAnalysis.getTable();
			std::vector<TypeInfo> tempstruct;
			for (Table::iterator it1 = symboltable.begin(); it1 != symboltable.end(); ++it1)
			{
				TypeInfo _struct;
				_struct._name = it1->first;
				_struct._file = firstfile;
				_struct._namespace = it1->second[1];
				_struct._type = it1->second[0];
				tempstruct.push_back(_struct);
			}
			std::map<std::string, std::vector<TypeInfo>> symbol;
			symbol.insert(std::pair<std::string, std::vector<TypeInfo>>(firstfile, tempstruct));
			DependencyAnalysis<TypeInfo> dependencyanalysis(secondfile);
			dependencyanalysis.DependencyTask(secondfile, &symbol);
			dependencyanalysis.displayTable();
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
	return 0;
}

#endif