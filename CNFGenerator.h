#include "Literal.h"
#include <map>

using namespace std;
#pragma once

typedef set<Literal> Clause;
typedef map<Clause,int>  ClauseSet;

class CNFGenerator
{
public:
	CNFGenerator(int t, int p);
	~CNFGenerator();
	bool GenerateCNF();
	bool OnePersonOneTable();
	bool FriendsGenerator();
	bool EnemiesGenerator();
	bool loadRelationshipFromVector(const vector<string> & relationsMatrix);
	bool printCNF();
	const ClauseSet getSetOfClauses() {return SetOfClauses;}
	static int globalClauseCount;

private:
	int tables;
	int people;
	vector <vector <bool> > PersonAtTable;
	vector <vector <short> > Relationships;
	vector <pair <short,short> > Friends;
	vector <pair <short,short> > Enemies;
	ClauseSet SetOfClauses;
};
