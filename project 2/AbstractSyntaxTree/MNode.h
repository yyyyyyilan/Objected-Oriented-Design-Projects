#ifndef MNODE_H
#define MNODE_H
///////////////////////////////////////////////////////////////////////
// MNode.cpp - Node helper for M-ary Tree data structure             //
// ver 1.3                                                           //
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
*   devenv Project_2.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.3 : 01 Mar 16 By Yilan Li
*   Added get&set functions of level to track the level of each node in one AST,
	which is futhured used to clearly show the tree structure in MetrcisAnalysis package.
* ver 1.2 : 26 Feb 16 By Yilan Li
*   Added function getChild(size_t i) to get the i'th child of a scope node
*   Added set & get functions of node's complexity to record number of node's children
*   Added set & get functions of node's type
* ver 1.1 : 24 Feb 16 By Yilan Li
*	Added set&get functions of
*		StartLine: the start line of current node of scope
*		EndLine: the end line of current node of scope
*
* By Jim Fawcett
* This is a complete redesign of an earlier MTree class that uses
* C++11 constructs, most noteably std::shared_ptr.
*
* Ver 1 : 4 Feb 15 by Jim Fawcett
* - first release
*
*/

#include <vector>
#include <memory>
#include <functional>
#include <string>
#include "Utilities.h"


/////////////////////////////////////////////////////////////////////////////
// MNode<T> class

template<typename T>
class MNode {
public:
	using sPtr = std::shared_ptr < MNode<T> >;
	using Children = std::vector <sPtr>;
	using iterator = typename Children::iterator;

	MNode();
	MNode(const T& t);
	MNode(const MNode<T>& node);
	MNode(MNode<T>&& node);
	MNode<T>& operator=(const MNode<T>& node);
	MNode<T>& operator=(MNode<T>&& node);
	sPtr clone() const;
	virtual ~MNode();

	T& value();
	T value() const;
	std::string& id();
	std::string id() const;
	bool addChild(sPtr pChild);  // fails if already has parent
	void addChild(const T& t);   // makes new node so has no parent
	std::vector<sPtr> nodeChildren();
	size_t size() const;

	void setParent(sPtr p);

	// note iterator points to std::shared_ptr<MNode<T>>, e.g., a pointer to a pointer
	iterator begin();
	iterator end();

	// note indexers return std::shared_ptr<MNode<T>>
	sPtr& operator[](size_t n);
	const sPtr& operator[](size_t n) const;

	sPtr findById(const std::string& id);
	std::vector<sPtr> findByValue(const T& t);

	void setStartLine(const size_t i);
	size_t getStartLine();
	void setEndLine(const size_t i);
	size_t getEndLine();

	void setComplexity(size_t i);
	size_t getComplexity();
	void setType(const std::string _type);
	std::string getType();
	void setLevel(size_t l);
	size_t getLevel();
		
private:
	void MNode<T>::cloneHelper(const MNode<T>& src, sPtr& dst) const;
	void MNode<T>::findByValueHelper(sPtr ptr, const T& t);		
	void MNode<T>::findByIdHelper(sPtr ptr, const std::string& id);
	Children children_;
	T value_;
	std::string id_;         // MTree expects all its nodes to have unique ids or empty ids 
	bool hasParent = false;  // set to true when added to a node's children_ collection
	std::vector<sPtr> foundValues_;
	sPtr foundId_;
	sPtr parent;
	size_t StartLine;
	size_t EndLine;
	size_t Complexity =1 ;
	std::string Type;
	size_t level;
};

//----< default constructor >-------------------------------------------------

template<typename T>
MNode<T>::MNode() : value_("default"), hasParent(false)
{
	mtLog << std::string("\n  constructing default MNode<T>");
}

//----< promotion constructor taking value >----------------------------------

template<typename T>
MNode<T>::MNode(const T& t) : value_(t), hasParent(false)
{
	mtLog << std::string("\n  constructing MNode<T>(T)");
}

//----< copy constructor >----------------------------------------------------

template<typename T>
MNode<T>::MNode(const MNode<T>& node) : value_(node.value_), hasParent(false)
{
	mtLog << std::string("copy construction from ") << node.value_;
	for (auto pChild : node.children_)
		children_.push_back(pChild->clone());
}
//----< move constructor >----------------------------------------------------

template<typename T>
MNode<T>::MNode(MNode<T>&& node) : value_(node.value_), hasParent(false)
{
	mtLog << std::string("\n  move construction from ") << value_;
	children_ = std::move(node.children_); // causes children_.swap()
	node.value_ = T();
}
//----< destructor just used to enunciate >-----------------------------------

template<typename T>
MNode<T>::~MNode()
{
}
//----< access children collection >------------------------------------------

template<typename T>
std::vector<std::shared_ptr<MNode<T>>> MNode<T>::nodeChildren() { return children_; }

//----< copy assignment operator >--------------------------------------------

template<typename T>
MNode<T>& MNode<T>::operator=(const MNode<T>& node)
{
	mtLog << std::string("\n  copy assignment from ") << node.value_;
	if (this == &node) return *this;
	// don't change hasParent
	value_ = node.value_;
	children_ = node.children_;
	return *this;
}
//----< move assignment operator >--------------------------------------------

template<typename T>
MNode<T>& MNode<T>::operator=(MNode<T>&& node)
{
	mtLog << std::string("\n  move assignment from ") << node.value_;
	if (this == &node) return *this;
	// don't change hasParent
	value_ = node.value_;
	children_ = std::move(node.children_);
	return *this;
}
//----< get iterator pointing to std::shared_ptr to first child >------------

template<typename T>
typename MNode<T>::iterator MNode<T>::begin() { return children_.begin(); }

//----< get iterator pointing to std::shared_ptr to one past last child >-----

template<typename T>
typename MNode<T>::iterator MNode<T>::end() { return children_.end(); }

//----< index through std::shared_ptrs to children >--------------------------

template<typename T>
std::shared_ptr<MNode<T>>& MNode<T>::operator[](size_t n)
{
	if (size() <= n)
		throw(std::invalid_argument("index out of range"));
	return children_[n];
}
//----< index through std::shared_ptrs to children >--------------------------

template<typename T>
const std::shared_ptr<MNode<T>>& MNode<T>::operator[](size_t n) const
{
	if (size() <= n)
		throw(std::invalid_argument("index out of range"));
	return children_[n];
}
//----< retrieve or set id >--------------------------------------------------

template<typename T>
std::string& MNode<T>::id() { return id_; }

//----< retrieve id >---------------------------------------------------------

template<typename T>
std::string MNode<T>::id() const { return id_; }

//----< retrieve or set value >-----------------------------------------------

template<typename T>
T& MNode<T>::value() { return value_; }

//----< retrieve value >------------------------------------------------------

template<typename T>
T MNode<T>::value() const { return value_; }

//----< private helper function for clone >-----------------------------------

template<typename T>
void MNode<T>::cloneHelper(const MNode<T>& src, sPtr& dst) const
{
	for (auto& pChild : src.children_)
	{
		sPtr pdstChild(new MNode<T>(pChild->value()));
		dst->addChild(pdstChild);
		cloneHelper(*pChild, pdstChild);
	}
}
//----< return clone of self >------------------------------------------------

template<typename T>
std::shared_ptr<MNode<T>> MNode<T>::clone() const
{
	mtLog << std::string("\n  cloning ") << value_;
	auto pReplica = sPtr(new MNode(this->value()));
	cloneHelper(*this, pReplica);
	return pReplica;
}
//----< add child std::shared_ptr to existing node >--------------------------
/*
*  Each node of tree must have only one parent (otherwise not a tree),
*  so add child only if child has no other parent.
*/
template<typename T>
bool MNode<T>::addChild(sPtr pChild)
{
	mtLog << std::string("\n  adding existing child ") << pChild->value_
		<< std::string(" to ") << value_;
	if (!pChild->hasParent)
	{
		pChild->hasParent = true;
		children_.push_back(pChild);
		return true;
	}
	return false;
}
//----< add child std::shared_ptr to new node with specified value >----------

template<typename T>
void MNode<T>::addChild(const T& t)
{
	mtLog << std::string("\n  adding new child ") << t << std::string(" to ") << value_;
	std::shared_ptr<MNode<T>> pToAdd(new MNode<T>(t));
	pToAdd->hasParent = true;
	//children_.push_back(pToAdd);
}
//----< return number of children >-------------------------------------------

template<typename T>
size_t MNode<T>::size() const { return children_.size(); }


//----< set parent node to current node >-------------------------------------
template <typename T>
void MNode<T>::setParent(sPtr p)
{
	parent = p;
}

//----< make new MNode<T> with specified value >------------------------------

template<typename T>
std::shared_ptr<MNode<T>> makeNode(const T& t)
{
	return std::shared_ptr<MNode<T>>(new MNode<T>(t));
}
//----< global function to display contents of MNode<T> >---------------------

template<typename T>
void show(MNode<T>& node)
{
	std::cout << "\n  " << node.value();
	for (auto& pChild : node)
		show(*pChild);
}
//----< Depth First Search with pluggable operation >-------------------------
/*
* apply callable object f on every node of tree during depth first search
*/
template<typename T>
void DFS(MNode<T>& node, std::function<void(const MNode<T>&)>& f)
{
	f(node);
	for (auto& pChild : node)
		DFS(*pChild, f);
}
//----< search children for id >----------------------------------------------

template<typename T>
void MNode<T>::findByIdHelper(sPtr ptr, const std::string& id)
{
	if (foundId_ != nullptr)
		return;
	if (ptr->id() == id)
		foundId_ = ptr;
	else
	{
		for (auto pChild : ptr->children_)
			findByIdHelper(pChild, id);
	}
}
//----< search children for id >----------------------------------------------

template<typename T>
std::shared_ptr<MNode<T>> MNode<T>::findById(const std::string& id)
{
	foundId_ = nullptr;
	for (auto pChild : children_)
	{
		findByIdHelper(pChild, id);
	}
	return foundId_;  // will be nullptr if not found
}
//----< search children for value >-------------------------------------------

template<typename T>
void MNode<T>::findByValueHelper(sPtr ptr, const T& value)
{
	if (ptr->value() == value)
		foundValues_.push_back(ptr);
	for (auto pChild : ptr->children_)
		findByValueHelper(pChild, value);
}
//----< search children for value >-------------------------------------------

template<typename T>
std::vector<std::shared_ptr<MNode<T>>> MNode<T>::findByValue(const T& value)
{
	for (auto pChild : children_)
		findByValueHelper(pChild, value);
	std::vector<std::shared_ptr<MNode<T>>> temp = std::move(foundValues_);
	return temp;
}
//----< set StartLine of a scope node >---------------------------------------

template <typename T>
void MNode<T>::setStartLine(const size_t i)
{
	StartLine = i;
}
//----< get StartLine of a scope node >---------------------------------------

template <typename T>
size_t MNode<T>::getStartLine()
{
	return StartLine;
}

//----< set EndLine of a scope node >-----------------------------------------
template <typename T>
void MNode<T>::setEndLine(const size_t i)
{
	EndLine = i;
}

//----< get EndLine of a scope node >-----------------------------------------
template <typename T>
size_t MNode<T>::getEndLine()
{
	return EndLine;
};
//----< function to set complexity of current scope node >--------------------

template <typename T>
void MNode<T>::setComplexity(size_t i)
{
	//Complexity = children_.size();
	Complexity = i;
}
//----< function to get complexity of current scope node >--------------------

template <typename T>
size_t MNode<T>::getComplexity()
{
	return Complexity;
}
//----< function to set type of current scope node >--------------------------

template <typename T>
void MNode<T>::setType(const std::string _type)
{
		Type = _type;
}
//----< function to get type of current scope node >--------------------------

template <typename T>
std::string MNode<T>::getType()
{
		return Type;
}
//----< function to set level of node >---------------------------------------

template <typename T>
void MNode<T>::setLevel(size_t l) { level = l; }
//----< function to get level of node >---------------------------------------

template <typename T>
size_t MNode<T>::getLevel() { return level; }

#endif
