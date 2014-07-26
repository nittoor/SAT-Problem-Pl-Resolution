#include "Literal.h"

ostream& operator << (ostream & out, const Literal & lit)
{
		if(lit.isNegated)
			out << "~";
		out<<"X"<<lit.person+1<<lit.table+1;
		return out;
}

bool operator < (const Literal & LitA,const Literal & LitB)
{
	if (LitA.isNegated !=LitB.isNegated  && LitA.isNegated== true)
		return true;
	else if  (LitA.isNegated ==LitB.isNegated && LitA.person<LitB.person  )
		return true;
	else if (LitA.person==LitB.person  && LitA.isNegated ==LitB.isNegated  && LitA.table <LitB.table  )
		return true;
	else
		return false;
}

bool operator == (const Literal & LitA,const Literal & LitB)
{
	if (LitA.person==LitB.person && LitA.table ==LitB.table && LitA.isNegated == LitB.isNegated)
		return true;
	else
		return false;
}

bool Literal::isComplimentary(Literal & lit)
{
	if (this->person==lit.person && this->table==lit.table && (this->isNegated != lit.isNegated))
		return true;
	else
		return false;
}

bool Literal::isComplimentary(const Literal & lit) const
{
	if (this->person==lit.person && this->table==lit.table && (this->isNegated != lit.isNegated))
		return true;
	else
		return false;
}
