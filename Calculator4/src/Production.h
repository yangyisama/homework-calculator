#pragma once
#include "Enums.h"

#include <vector>
#include <string>

Enums::GrammarSymbol stringToSymbol(std::string s);

class Production
{
public:
	Production(std::vector<Enums::GrammarSymbol> b, Enums::GrammarSymbol h);
	int getBodySize();
	Enums::GrammarSymbol getHead();
	Production(std::string s); // generates a production from 1-line text description
	Production(); // dummy production
	std::vector<Enums::GrammarSymbol> getBodyCopy();

	// we overload < to work as != for purposes of storing these in sets
	inline bool operator < (const Production &p1) const
	{
		if (p1.head != head) return true;
		if (p1.body.size() != body.size()) return true;
		for (int i = 0; i < p1.body.size(); i++)
			if (p1.body[i] != body[i]) return true;
		return false;
	}	
	
	inline bool operator == (const Production &p1) const
	{
		return !(*this < p1);
	}

private:
	std::vector<Enums::GrammarSymbol> body;
	Enums::GrammarSymbol head;
};