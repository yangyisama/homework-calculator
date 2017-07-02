#include "Item.h"

Item::Item(int pos, Production p)
	:
	dotPos (pos),
	production (p)
{ }

Item Item::getAdvanced()
{
	if (isItemOver())
		std::cerr << "\nTrying to advance an item that's over...";
	return Item(dotPos + 1, production);
}

int Item::getDotPos()
{
	return dotPos;
}

Production Item::getProduction()
{
	return production;
}

Enums::GrammarSymbol Item::symbolAfterDot()
{
	if (dotPos == production.getBodySize())
		return Enums::GrammarSymbol::EPSILON; // TODO is this a good idea?
	else if (dotPos < production.getBodySize())
		return production.getBodyCopy()[dotPos];
	else
	{
		std::cerr << "\nItem error: Production body access out of bounds!";
		std::cerr << "\n" << production.toString() << " at pos " << dotPos;
		return Enums::GrammarSymbol::START;
	}
}

bool Item::isTheNextSymbolThis(Enums::GrammarSymbol sym)
{
	return symbolAfterDot() == sym;
}

bool Item::isItemOver()
{
	return dotPos == production.getBodySize();
}
