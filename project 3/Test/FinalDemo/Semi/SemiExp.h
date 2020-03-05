#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H

#include <vector>
#include <string>
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/itokcollection.h"

namespace Scanner
{
	class SemiExp : public ITokCollection
	{
	public:
		SemiExp(Toker* pToker = nullptr);
		SemiExp(const SemiExp&) = delete;
		SemiExp& operator=(const SemiExp&) = delete;
		bool get(bool clear = true);
		size_t find(const std::string& tok);
		void trimFront();
		void toLower();
		bool remove(const std::string& tok);
		bool remove(size_t n);
		void push_back(const std::string& tok);
		void clear();
		bool isComment(const std::string& tok);
		size_t currentLineCount();
	private:
		bool isTerminator(const std::string& tok);
		bool getHelper(bool clear = false);
		bool isSemiColonBetweenParens();
		bool hasFor = false;
		std::vector<std::string> _tokens;
		Toker* _pToker;
	};
}
#endif
