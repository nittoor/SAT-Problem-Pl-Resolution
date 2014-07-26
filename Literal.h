#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>

#pragma once

using namespace std;

class Literal
{
	
	short person;
	short table;
	bool  isNegated;

	public:
	Literal(short p,short t,bool isNeg):person(p),table(t),isNegated(isNeg){}

	friend ostream& operator << (ostream & out, const Literal & lit);
	bool isComplimentary( Literal & lit);
	bool isComplimentary(const Literal & lit) const;
	friend bool operator == (const Literal & LitA,const Literal & LitB);
	friend bool operator < (const Literal & LitA,const Literal & LitB);
	
} ;

