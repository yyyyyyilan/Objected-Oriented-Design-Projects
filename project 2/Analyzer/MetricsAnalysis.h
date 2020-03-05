#ifndef METRICSANALYSIS_H
#define METRICSANALYSIS_H
/////////////////////////////////////////////////////////////////////////////////
// MetricsAnalysis.h - evaluate and display AST package                        //
// ver 1.1                                                                     //
// Application: Evaluate specified package, CSE687 - Object Oriented Design    //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			           //
// Author:		Yilan Li, Syracuse University, (315)382-9302				   //
//				yli41@syr.edu												   //
/////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
*   This module evaluates and displays information that is stored in the AST, 
*   including the size and complexity of functions in each of a set of specified packages.
*
* Required Files:
* ===============
*   Tokenizer.h Tokenizer.cpp SemiExp.h SemiExp.cpp Parser.h Parser.cpp
*	RulesAndActions.h RulesAndActions.cpp ConfigureParser.h ConfigureParser.cpp 
*	MNode.h MNode.cpp MTree.h MTree.cpp
*
* Build Process:
* ==============
* - devenv Project_2.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.1 : 02 Mar 2016
* - Added void void printTree() to the whole structure of one AST tree
* - Added Repository* parse(std::string pa) function to parse the file
* ver 1.0 : 29 Feb 2016 
* - first release
*/

#include "../AbstractSyntaxTree/MNode.h"
#include "../AbstractSyntaxTree/MTree.h"
#include "../Parser/RulesAndActions.h"

/////////////////////////////////////////////////////////////////////////////
// MetricsAnalysis<T> class

template <typename T>
class MetricsAnalysis
{
public:
	using sPtr = std::shared_ptr < MNode<T> >;
	MetricsAnalysis();
	~MetricsAnalysis();
	void treeAnalysis();
	void showFunctionInfo();
	void printTree();
	Repository parse(std::string pa);
private:
	void calComplexity(sPtr node);
	void nodeAnalysis(sPtr pNode);
	bool isFunction(sPtr node);
	std::vector<sPtr> getFunctionNode();
	void printNode(sPtr n);
	void findPrintPosition(sPtr n);
	std::vector<sPtr> functionCollection;
	MTree<T> mTree_;			
	Repository* pRepo;
};

//----< Default MetricsAnalysis constructor >----------------------------------
template <typename T>
MetricsAnalysis<T>::MetricsAnalysis() { pRepo = nullptr; }

//----< Destructor >-----------------------------------------------------------
template <typename T>
MetricsAnalysis<T>::~MetricsAnalysis() { }

//----< walk through AST and show nodes information >--------------------------
template <typename T>
void MetricsAnalysis<T>::treeAnalysis()
{
	MTree<std::string>* tempTree = pRepo->getTree();
	std::shared_ptr<MNode<std::string>> start = tempTree->root();
	nodeAnalysis(start);
	showFunctionInfo();
}

//----< Display Function node complexity >-------------------------------------
template <typename T>
void MetricsAnalysis<T>::showFunctionInfo()
{	
	std::vector<sPtr> tempFunc = getFunctionNode();
	for (size_t i = 0; i < tempFunc.size(); i++)
	{
		std::shared_ptr<MNode<std::string>> temp = tempFunc.at(i);
		std::cout << "Function : " << temp->value();
		std::cout << "\t Complexity = " << temp->getComplexity();
		size_t size = temp->getEndLine() - temp->getStartLine() + 1;
		std::cout << "\t Size = " << size;
		std::cout << "\n";
	}
}

//----< display AST >----------------------------------------------------------
template<typename T>
void MetricsAnalysis<T>::printTree()
{
	sPtr r = pRepo->getTree()->root();
	std::cout << "[ ";
	std::cout << r->getType();
	std::cout << "   " << r->value();
	std::cout << "   " << r->getStartLine();
	std::cout << "   " << r->getEndLine();
	std::cout << " ]";
	std::cout << "\n";
	printNode(r);
}

//----< display node >---------------------------------------------------------
template <typename T>
void MetricsAnalysis<T>::printNode(sPtr n)
{
	for (auto pChild : n->nodeChildren())
	{
		//std::shared_ptr<MNode<std::string>> t = n;
		findPrintPosition(pChild);
		std::cout << "[ ";
		std::cout << pChild->getType();
		std::cout << "   " << pChild->value();
		std::cout << "   " << pChild->getStartLine();
		std::cout << "   " << pChild->getEndLine();
		if ((pChild->getType()) == "function")
			std::cout << "   " << "complexity = " << pChild->getComplexity();
		std::cout << " ]";
		std::cout << "\n";
		printNode(pChild);
	}
}

//----< find right place to print scope node >---------------------------------
template<typename T>
void MetricsAnalysis<T>::findPrintPosition(sPtr n)
{
	std::shared_ptr<MNode<std::string>> temp = n;
	size_t l = temp->getLevel();
	for (size_t i = 0; i < l; i++)
	{
		std::cout << "\t";
	}
}

//----< calculate complexity of each node >------------------------------------
template <typename T>
void MetricsAnalysis<T>::calComplexity(sPtr node)
{
	for (auto pChild : node->nodeChildren())
	{
		calComplexity(pChild);
		size_t t = node->getComplexity();
		t = t + pChild->getComplexity();
		node->setComplexity(t);
	}
}

//----< detect whether or not it's a function node >---------------------
template <typename T>
bool MetricsAnalysis<T>::isFunction(sPtr node)
{
	std::shared_ptr<MNode<std::string>> n = node;
	if (n->getType() == "function")
		return true;
	return false;
}

//----< recursive left-to-right depth first search >---------------------------
template <typename T>
void MetricsAnalysis<T>::nodeAnalysis(sPtr pNode)
{
	//std::cout << "\n  " << pNode->name();
	for (auto pChild : pNode->nodeChildren())
	{
		calComplexity(pChild);
		if (pChild->getType() == "function")
		{
			functionCollection.push_back(pChild);
		}
		nodeAnalysis(pChild);
	}
}

//----< get collection of function nodes >-------------------------------------
template <typename T>
std::vector<std::shared_ptr <MNode<T>>> MetricsAnalysis<T>::getFunctionNode()
{
	return functionCollection;
}

//----< parse the input file >-------------------------------------------------
template <typename T>
Repository MetricsAnalysis<T>::parse(std::string pa)
{
	ConfigParseToConsole configure;
	Parser* pParser = configure.Build();
	if (pParser)
		{
			if (!configure.Attach(pa))
			{
				std::cout << "\n  could not open file " << pa << std::endl;
			}
		}
	else
		std::cout << "\n\n  Parser not built\n\n";
		// now that parser is built, use it
	while (pParser->next())
		pParser->parse();
	std::cout << "\n\n";
	pRepo = configure.getRepo();
	pRepo->setInitial(true);
	configure.getToker()->setInitial(true, true);
	return *pRepo;
}

#endif
