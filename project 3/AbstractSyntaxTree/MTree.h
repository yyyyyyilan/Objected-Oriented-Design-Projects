#ifndef MTREE_H
#define MTREE_H
///////////////////////////////////////////////////////////////////////
// MTree.h - M-ary Tree data structure                               //
// ver 1.1                                                           //
// Language:  C++, Visual Studio 2015 Community                      //
// Application: Help for CSE687 Pr#2, Spring 2015                    //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education			 //
// Author:		Yilan Li, Syracuse University, (315)382-9302         //
//				yli41@syr.edu									     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package supports building an abstract syntax tree. It also provides
* interface to add a scope node to AST.
* This package contains classes MTree<T> and MNode<T>.
* MTree<T> is an M-ary tree, e.g., each MNode<T> has zero or more
* children.  Note that this is not a balanced binary tree.  No
* order is imposed on the children of any node.  They simply appear
* in the order they were added.
*
* M-ary trees are often used to represent parse trees where the
* ordering of nodes depends on the structure of the entity being
* represented, e.g., source code, XML, or HTML.
*
* MTree<T>'s nodes are members of class MNode<T>.  each MNode<T>
* holds a vector of std::shared_prt's to it's children, if any.
* the value of the template parameter T represents what each node
* of the tree holds, e.g., a string representing a grammatical
* constructor, or XML or HTML element.
*
* Required Files:
* ---------------
*   - MTree.h and MTree.cpp
*
* Build Process:
* --------------
* - devenv Project_2.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.1: 29 Feb 16 by Yilan Li
* - Added following functions:
*		void setRootNode(MTree<T> t)
*		sPtr makeNode(std::string name, std::string type, size_t start_l, size_t end_l)
*		void addNode(sPtr node)
* - Set show function as a member function
* By Jim Fawcett
* This is a complete redesign of an earlier MTree class that uses
* C++11 constructs, most noteably std::shared_ptr.
*
* Ver 1 : 4 Feb 15
* - first release
*
*/
#include <vector>
#include <memory>
#include <functional>
#include <string>
#include "MNode.h"
#include "Utilities.h"

/////////////////////////////////////////////////////////////////////////////
// MTree<T> class

template<typename T>
class MTree
{
public:
	using sPtr = std::shared_ptr < MNode<T> >;

	MTree(sPtr& pRoot);
	MTree(const T& t = T());
	MTree(const MTree<T>& tree);
	MTree(MTree<T>&& tree);
	~MTree();

	MTree<T>& operator=(const MTree<T>& tree);
	MTree<T>& operator=(MTree<T>&& tree);

	sPtr root();
	sPtr findById(const std::string& id);
	std::vector<sPtr> findByValue(const T& value);

	std::vector<sPtr> children(const T& value, size_t count = 1);
	std::vector<sPtr> descendents(const T& value, size_t count = 1);

	void setRootNode();
	sPtr makeNode(std::string name, std::string type, size_t start_l, size_t end_l,size_t l);
	void addNode(sPtr node, sPtr pnode);
	void show(MNode<T>& node);

private:
	void decendentsHelper(sPtr ptr);
	sPtr pRoot_ = nullptr;
	std::vector<sPtr> collection_;

	sPtr makeNodeHelper(const T& t);
	void showHelper(sPtr pNode, size_t levelCount);
	bool isRoot(std::shared_ptr<MNode<std::string>> n);

	sPtr pParent = nullptr;
	sPtr pCurrent;
};
//----< construct tree from root node >--------------------------------------

template<typename T>
MTree<T>::MTree(sPtr& pRoot) : pRoot_(pRoot) 
{
	pCurrent = pRoot;
}

//----< construct tree from value for new root node >------------------------

template<typename T>
MTree<T>::MTree(const T& t) : pRoot_(sPtr(new MNode<T>(t))) { }

//----< destructor >---------------------------------------------------------

template<typename T>
MTree<T>::~MTree() {}

//----< copy constructor >---------------------------------------------------

template<typename T>
MTree<T>::MTree(const MTree<T>& tree)
{
	pRoot_ = tree.pRoot_->clone();
}
//----< move constructor >---------------------------------------------------

template<typename T>
MTree<T>::MTree(MTree<T>&& tree)
{
	pRoot_ = tree.pRoot_;
	tree.pRoot_ = nullptr;
}
//----< copy assignment >----------------------------------------------------

template<typename T>
MTree<T>& MTree<T>::operator=(const MTree<T>& tree)
{
	if (this == &tree) return *this;
	pRoot_ = tree.pRoot_->clone();
	return *this;
}
//----< move assignment >----------------------------------------------------

template<typename T>
MTree<T>& MTree<T>::operator=(MTree<T>&& tree)
{
	if (this == &tree) return *this;
	pRoot_ = tree.pRoot_;
	tree.pRoot_ = nullptr;
	return *this;
}
//----< return tree root node >----------------------------------------------

template<typename T>
std::shared_ptr<MNode<T>> MTree<T>::root() { return pRoot_; }

//----< find tree node by id >-----------------------------------------------

template<typename T>
std::shared_ptr<MNode<T>> MTree<T>::findById(const std::string& id)
{
	if (pRoot_->id() == id)
		return pRoot_;
	return pRoot_->findById(id);
}
//----< find tree nodes by value >-------------------------------------------

template<typename T>
std::vector<std::shared_ptr<MNode<T>>> MTree<T>::findByValue(const T& value)
{
	std::vector<sPtr> temp = pRoot_->findByValue(value);
	if (pRoot_->value() == value)
		temp.insert(begin(temp), pRoot_);
	return temp;
}
//----< find children of node with specified value >-------------------------

template<typename T>
std::vector<std::shared_ptr<MNode<T>>> MTree<T>::children(const T& value, size_t count = 1)
{
	std::vector<std::shared_ptr<MNode<T>>> temp = findByValue(value);
	if (temp.size() < count)
	{
		temp.clear();
		return temp;
	}
	for (auto pChild : temp[count - 1]->nodeChildren())
		collection_.push_back(pChild);
	return std::move(collection_);
}
//----< find descendents of node with specified value >----------------------

template<typename T>
void MTree<T>::decendentsHelper(std::shared_ptr<MNode<T>> ptr)
{
	collection_.push_back(ptr);
	for (auto pChild : ptr->nodeChildren())
		decendentsHelper(pChild);
}

template<typename T>
std::vector<std::shared_ptr<MNode<T>>> MTree<T>::descendents(const T& value, size_t count = 1)
{
	std::vector<std::shared_ptr<MNode<T>>> temp = findByValue(value);
	if (temp.size() < count)
	{
		temp.clear();
		return temp;
	}
	for (auto pChild : temp[count - 1]->nodeChildren())
		decendentsHelper(pChild);
	return std::move(collection_);
}

//----< set default value of root node >---------------------------------------
template <typename T>
void MTree<T>::setRootNode()
{
	std::shared_ptr <MNode<std::string>> tempNode(new MNode<T>("Global Namespace"));
	MTree<std::string> tempTree(tempNode);
	tempTree.root()->value() = "Global";
	tempTree.root()->setType("Namespace");
	tempTree.root()->setStartLine(0);
	tempTree.root()->setEndLine(0);
	pRoot_ = tempTree.root();
	pCurrent = pRoot_;
}

//----< make AST node >--------------------------------------------------------
template <typename T>
std::shared_ptr < MNode<T> > MTree<T>::makeNode(std::string name, std::string type, size_t start_l, size_t end_l,size_t l)
{
	//sPtr tempnode = makeNodeHelper<std::string>(name);
	std::shared_ptr <MNode<std::string>> tempnode(new MNode<T>(name));
	tempnode->value() = name;
	tempnode->setType(type);
	tempnode->setStartLine(start_l);
	tempnode->setEndLine(end_l);
	tempnode->setLevel(l);
	return tempnode;
}

//----< detect if a node is root >---------------------------------------------
template <typename T>
bool MTree<T>::isRoot(std::shared_ptr<MNode<std::string>> n)
{
	if (n == pRoot_)
		return true;
	return false;
}

//----< add a Node to AST >----------------------------------------------------
template <typename T>
void MTree<T>::addNode(sPtr node, sPtr pnode)
{
	std::string pname = pnode->value();
	std::vector<sPtr> ch = findByValue(pname);
	/*if (ch[0]->addChild(node))
	{
		ch[0]->addChild(node->value());
	}*/
	ch[0]->addChild(node);
}

//---- < helper function to make a scope node >--------------------------------
template <typename T>
std::shared_ptr<MNode<T>> MTree<T>::makeNodeHelper(const T& t)
{
	return sPtr(new MNode<T>(t));
}

//---- < show tree node structure >--------------------------------------------
template <typename T>
void MTree<T>::showHelper(sPtr pNode, size_t levelCount)
{
	std::string spacer(levelCount, ' ');
	levelCount += 2;
	std::cout << "\n  " << spacer << pNode->value();
	for (auto pChild : *pNode)
		showHelper(pChild, levelCount);
	levelCount -= 2;
}

template <typename T>
void MTree<T>::show(MNode<T>& node)
{
	std::cout << "\n  " << node.value();
	//std::cout << "\n" << node.getComplexity();
	for (auto& pChild : node)
		show(*pChild);
}

//----< show tree node structure >-------------------------------------------

template<typename T>
void showHelper(std::shared_ptr<MNode<T>> pNode, size_t levelCount)
{
	std::string spacer(levelCount, ' ');
	levelCount += 2;
	std::cout << "\n  " << spacer << pNode->value();
	//std::cout << spacer << "cc = " << pNode->getComplexity();
	for (auto pChild : *pNode)
		showHelper(pChild, levelCount);
	levelCount -= 2;
}

template <typename T>
void show(MTree<T>& tree)
{
	if (tree.root() != nullptr)
		showHelper(tree.root(), 0);
}
#endif
