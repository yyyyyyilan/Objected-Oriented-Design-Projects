/////////////////////////////////////////////////////////////////
// MNode.cpp - Node helper for M-ary Tree data structure       //
//                                                             //
// Application: Help for CSE687 Pr#2, Spring 2015              //
// Platform:    Dell XPS 2720, Win 8.1 Pro, Visual Studio 2013 //
// Author:      Jim Fawcett, CST 4-187, 443-3948               //
//              jfawcett@twcny.rr.com                          //
/////////////////////////////////////////////////////////////////

#include "MNode.h"
#include <string>
#include <iostream>

//#define TEST_MNODE
#ifdef TEST_MNODE

using MNodeStr = MNode < std::string >;
using sPtr = std::shared_ptr < MNodeStr >;

int main()
{
	title("Testing class MNode<T>", '=');

	title("creating instance of node");
	sPtr pNode(new MNodeStr("original"));
	std::cout << "\n  " << pNode->value() << "\n";

	title("adding child nodes");
	mtLog.start();
	pNode->addChild("first child");
	pNode->addChild("second child");
	pNode->addChild("third child");

	MNodeStr& node = *pNode;
	node[1]->addChild("second child's child");
	MNodeStr& secondChild = *node[1];
	secondChild[0]->addChild("second child's grandchild");
	node[2]->addChild("third's child");

	// Note that reference qualifiers for node and secondChild are necessary.
	// We don't want to add children to copies of the nodes.

	///////////////////////////////////////////////////////////////////////////
	// if these renaming operations get tedious you can use
	// nested indirections like this:
	//
	// (*pNode)[1]->addChild("second child's child");
	// (*(*pNode)[1])[0]->addChild("second child's grandchild");
	// (*pNode)[2]->addChild("third's child");

	mtLog.show();
	show(*pNode);
	std::cout << "\n";

	title("finding \"third child\" in node");
	std::vector<sPtr> found = pNode->findByValue("third child");
	if (found.size() > 0)
	{
		std::cout << "\n  found:";
		for (auto item : found)
			std::cout << "\n    " << item->value();
	}
	else
		std::cout << "\n  not found";
	std::cout << "\n";

	title("finding \"second child\" in Augmented");
	sPtr pAugmented = pNode->clone();
	pAugmented->addChild("second child");
	found = pAugmented->findByValue("second child");
	if (found.size() > 0)
	{
		std::cout << "\n  found:";
		for (auto item : found)
			std::cout << "\n    " << item->value();
	}
	else
		std::cout << "\n  not found";
	std::cout << "\n";

	title("finding \"no_such_value\" in Augmented");
	found = pAugmented->findByValue("no_such_value");
	if (found.size() > 0)
	{
		std::cout << "\n  found:";
		for (auto item : found)
			std::cout << "\n    " << item->value();
	}
	else
		std::cout << "\n  not found";
	std::cout << "\n";

	title("finding \"grandChildId\" in Augmented");
	MNode<std::string>& Augmented = *pAugmented;
	sPtr pSecondChild = Augmented[1];
	MNode<std::string>& SecondChild = *pSecondChild;
	sPtr pGrandChild = SecondChild[0];
	pGrandChild->id() = "grandChildId";
	sPtr pId = pAugmented->findById("grandChildId");
	if (pId != nullptr)
	{
		std::cout << "\n  found:";
		std::cout << "\n    id = " << pId->id();
		std::cout << "\n    value = " << pId->value();
	}
	else
		std::cout << "\n    not found";
	std::cout << "\n";

	title("finding \"no_such_id\" in Augmented");
	pId = pAugmented->findById("no_such_id");
	if (pId != nullptr)
	{
		std::cout << "\n  found:";
		std::cout << "\n    id = " << pId->id();
		std::cout << "\n    value = " << pId->value();
	}
	else
		std::cout << "\n    not found";
	std::cout << "\n";

	title("testing DFS with functional");

	// making lambda that displays node values

	std::function<void(const MNodeStr&)> f = [](const MNodeStr& node) { std::cout << "\n  " << node.value(); };

	// using in depth first search to show the tree rooted at pNode

	DFS<std::string>(*pNode, f);
	std::cout << "\n";

	title("making clone of original node");

	sPtr pClone = pNode->clone();
	pClone->value() = std::string("clone of ") + pNode->value();
	std::cout << "\n  " << pClone->value() << " size = " << pClone->size();
	std::cout << "\n";

	title("show clone");
	show(*pClone);
	std::cout << "\n";

	title("making copy of original node");
	MNodeStr copy = *pNode;
	copy.value() = std::string("copy of ") + pNode->value();
	show(copy);
	std::cout << "\n";

	title("changing first and third child values in copy");
	copy[0]->value() += " modified";
	copy[2]->value() += " modified";
	show(copy);
	std::cout << "\n";
	title("showing original did not change");
	show(*pNode);
	std::cout << "\n";

	title("testing move construction");
	MNode<std::string> moveTarget = std::move(copy);
	moveTarget.value() = "moveTarget";
	show(moveTarget);
	std::cout << "\n";
	std::cout << "\n  value of move src = " << copy.value();
	std::cout << "\n   size of move src = " << copy.size();
	std::cout << "\n";

	title("testing copy assignment");
	MNodeStr assignTarget;
	assignTarget = moveTarget;
	assignTarget.value() = "assignTarget";
	show(assignTarget);
	std::cout << "\n";
	show(moveTarget);
	std::cout << "\n";

	title("testing move assignment");
	MNodeStr moveAssignTarget;
	moveAssignTarget = std::move(moveTarget);
	moveAssignTarget.value() = "moveAssignTarget";
	show(moveAssignTarget);
	std::cout << "\n";
	show(moveTarget);
	std::cout << "\n";

	title("adding child defined by shared_ptr");
	sPtr pRoot = makeNode<std::string>("root");
	pRoot->value() = "root";
	pRoot->addChild(pNode);
	show(*pRoot);
	std::cout << "\n";
	std::cout << "\n  " << pRoot->value() << " has " << pRoot->size() << " elements";
	std::cout << "\n  the first child of " << pRoot->value() << " is " << (*pRoot)[0]->value();
	std::cout << "\n  size of " << (*pRoot)[0]->value() << " is: " << (*pRoot)[0]->size();
	std::cout << "\n";

	title("defining const node and attempting to change");
	const MNodeStr& rootRef = *pRoot;
	std::cout << "\n  value of const node is " << rootRef.value();
	std::cout << "\n  the first child of const rootRef is: " << rootRef[0]->value();
	std::cout << "\n  size of first child is: " << rootRef[0]->size();

	rootRef.value() += " - modified";  // this just writes to returned value - no effect on rootRef
	std::cout << "\n  const rootRef now is: " << rootRef.value();
	rootRef[0]->value() += " - modified"; // but this does modify child
	std::cout << "\n  the first child of const rootRef now is: " << rootRef[0]->value();
	std::cout << "\n";

	title("showing log");
	mtLog.show();
	mtLog.clear();
	title("showing cleared log");
	mtLog.show();
	std::cout << "\n\n";
}

#endif
