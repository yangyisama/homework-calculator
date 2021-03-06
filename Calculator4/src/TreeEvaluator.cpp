#include "TreeEvaluator.h"

#include <cmath> // cos


Evaluation floatEval(float f)
{
	return Evaluation{ -69, f, false };
}

Evaluation intEval(int i)
{
	return Evaluation{ i, (float)i, true };
}

float toFraction(int t, int leadingZeros) // takes t and divides it by 10 until it's smaller than 1.
{
	float f = (float)t;
	while (f >= 1.0)
		f /= 10.0;
	for (int i = 0; i < leadingZeros; i++)
		f /= 10.0;
	return f;
}

int factorial(int i)
{
	int r = 1;
	for (int j = 2; j <= i; j++)
		r *= j;
	return r;
}

Evaluation eval(Tree* t)
{
	Enums::SymbolAndValue node = t->getPayload();
	Tree** children = t->getChildren();
	Evaluation arg[3]; // 3 is guaranteed to be the size of children
	float farg[3];
	int iarg[3];

	for (int i = 0; i < 3; i++)
	{
		if (children[i])
		{
			arg[i] = eval(children[i]);
			farg[i] = arg[i].fval;
			iarg[i] = arg[i].ival;
		}
	}

	switch (node.symbol)
	{ // EXTENDED_START not needed here, right?

	case Enums::GrammarSymbol::NUM: // LEAF CASE aka RECURSION BASE
		return intEval(node.ivalue);

	case Enums::GrammarSymbol::START:
		return floatEval(farg[0]);

	case Enums::GrammarSymbol::E:
		return floatEval(farg[0]);

	case Enums::GrammarSymbol::F:
		if (children[1]) // F + F1
		{
			if (arg[0].itsAnInt && arg[2].itsAnInt)
				return intEval(iarg[0] + iarg[2]);
			return floatEval(farg[0] + farg[2]);
		}
		else // F1
		{
			if (arg[0].itsAnInt)
				return intEval(iarg[0]);
			return floatEval(farg[0]);
		}

	case Enums::GrammarSymbol::F1:
		if (children[1]) // F1 - F2
		{
			if (arg[0].itsAnInt && arg[2].itsAnInt)
				return intEval(iarg[0] - iarg[2]);
			return floatEval(farg[0] - farg[2]);
		}
		else // F2
		{
			if (arg[0].itsAnInt)
				return intEval(iarg[0]);
			return floatEval(farg[0]);
		}

	case Enums::GrammarSymbol::F2:
		if (children[1]) // F2 * F3
		{
			if (arg[0].itsAnInt && arg[2].itsAnInt)
				return intEval(iarg[0] * iarg[2]);
			return floatEval(farg[0] * farg[2]);
		}
		else // F2
		{
			if (arg[0].itsAnInt)
				return intEval(iarg[0]);
			return floatEval(farg[0]);
		}

	case Enums::GrammarSymbol::F3:
		if (children[1]) // case cos F3
			return floatEval(cos(farg[1]));
		else // F4
		{
			if (arg[0].itsAnInt)
				return intEval(iarg[0]);
			return floatEval(farg[0]);
		}

	case Enums::GrammarSymbol::F4:
		if (children[2]) // case ( F )
		{
			if (arg[1].itsAnInt)
				return intEval(iarg[1]);
			return floatEval(farg[1]);
		}
		else if (children[1]) // case F4 !
		{
			if (arg[0].itsAnInt)
				return intEval(factorial(iarg[0]));
			else
			{
				throw std::exception("Can't take factorial of a float!");
			}
		}
		else // case FLOAT | INT
		{
			if (arg[0].itsAnInt)
				return intEval(iarg[0]);
			return floatEval(farg[0]);
		}

	case Enums::GrammarSymbol::FLOAT:
		//case num.num
		if (children[2])
			return floatEval((float)(iarg[0] >> 4) + toFraction(iarg[2] >> 4, iarg[2] % (1 << 4)));
		//case num.
		else if (children[1]->getPayload().symbol == Enums::GrammarSymbol::DOT)
			return floatEval((float)(iarg[0] >> 4));
		//case .num
		else
			return floatEval(toFraction(iarg[1] >> 4, iarg[1] % (1 << 4)));
		
	case Enums::GrammarSymbol::INT:
		return intEval(iarg[0] >> 4); // the last 4 bits encoded the #leading zeros

	default:
		/*
		This dummy evaluation will never be used, 
		it is here for cases such as LEFTPAREN
		because we evaluate all children before we look at
		what production we have
		For example for node I3 with children ( I ), we evalute LEFTPAREN
		and then don't look at the meaningless result.

		The programmer has to program the cases above correctly and avoid using
		the dummy!
		*/
		return Evaluation{ 0, 0, 0 };
	}
}

float final_eval(Tree *t)
{
	return eval(t).fval;
}