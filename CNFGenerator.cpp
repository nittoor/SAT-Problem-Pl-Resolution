#include "CNFGenerator.h"

int CNFGenerator::globalClauseCount=0;

CNFGenerator::CNFGenerator(int t, int p):tables(t),people(p)
{
	PersonAtTable.resize(tables);
	for (int i=0;i<PersonAtTable.size();i++)
		PersonAtTable[i].resize(people);
	Relationships.resize(people);
	
}

bool CNFGenerator::loadRelationshipFromVector(const vector<string> & relationsMatrix)
{
	short tempRel;
	for (int i=0;i<people;i++)
	{
		stringstream temp (relationsMatrix[i]);
		
		//cout << temp.str() <<endl;
			
		while (1)
		{
			temp >> tempRel;
			if  (!temp)
				break;
			Relationships[i].push_back(tempRel);
		}
		//cout <<  endl;
	}
	cout<<"relationship"<<endl;

	for (int i=0;i<Relationships.size();i++)
	{
		for (int j=0;j<Relationships[i].size();j++)
		{
			cout<<Relationships[i][j]<<"\t";
		}
		cout<<endl;
	}

	for (int i =0;i<people;i++)
	{
		for (int j =0;j<i;j++)
		{
			if (Relationships[i][j]==1)
				Friends.push_back(pair<short,short>(i,j));
			else if (Relationships[i][j]==-1)
				Enemies.push_back(pair<short,short>(i,j));
		}
	}
	return true;
}

bool CNFGenerator::GenerateCNF()
{
	OnePersonOneTable();
	FriendsGenerator();
	EnemiesGenerator();
	printCNF();
	/*
	sort(SetOfClauses.begin(),SetOfClauses.end());
	ClauseSet::iterator lastUniquePoint = unique(SetOfClauses.begin(),SetOfClauses.end());
	SetOfClauses.resize(distance(SetOfClauses.begin(),lastUniquePoint));
	*/

	return true;
}

bool CNFGenerator::FriendsGenerator()
{
	Clause newFriendClause1;
	Clause newFriendClause2;
	for (vector<pair <short,short> >::iterator it = Friends.begin();it!=Friends.end();it++)
	{
		for (int i =0; i< tables;i++)
		{
			newFriendClause1.clear();
			newFriendClause2.clear();
			Literal newLiteral1(it->first,i,true);
			Literal newLiteral2(it->second,i,false);
			newFriendClause1.insert(newLiteral1);
			newFriendClause1.insert(newLiteral2);
			//sort(newFriendClause1.begin(),newFriendClause1.end());
			
			Literal newLiteral3(it->first,i,false);
			Literal newLiteral4(it->second,i,true);
			newFriendClause2.insert(newLiteral3);
			newFriendClause2.insert(newLiteral4);
			//sort(newFriendClause2.begin(),newFriendClause2.end());
			
			SetOfClauses.insert(pair<Clause,int>(newFriendClause1,++CNFGenerator::globalClauseCount));
			SetOfClauses.insert(pair<Clause,int> (newFriendClause2,++CNFGenerator::globalClauseCount));
		}
		
		
	}
	return true;
}

bool CNFGenerator::EnemiesGenerator()
{
	Clause newEnemyClause;
	for (vector<pair <short,short> >::iterator it = Enemies.begin();it!=Enemies.end();it++)
	{
		for (int i =0; i< tables;i++)
		{
			newEnemyClause.clear();
			Literal newLiteral1(it->first,i,true);
			Literal newLiteral2(it->second,i,true);
			newEnemyClause.insert(newLiteral1);
			newEnemyClause.insert(newLiteral2);
			//sort(newEnemyClause.begin(),newEnemyClause.end());
			SetOfClauses.insert(pair<Clause,int>(newEnemyClause,++CNFGenerator::globalClauseCount));
		}
		
	}

	return true;

}

bool CNFGenerator::OnePersonOneTable()
{
	Clause onePersonAtLeastOneTable;
	Clause onePersonAtMaxOneTable;
	for (int i =0;i<people;i++)
	{
		onePersonAtLeastOneTable.clear();
		onePersonAtMaxOneTable.clear();
		//cout <<"atleast 1 =";
		for (int j=0;j<tables;j++)
		{
			Literal newLiteral(i,j,false);
			//cout <<newLiteral;
			onePersonAtLeastOneTable.insert(newLiteral);
			
		}
		//cout<<endl;

		//sort(onePersonAtLeastOneTable.begin(),onePersonAtLeastOneTable.end());
		SetOfClauses.insert(pair<Clause,int> (onePersonAtLeastOneTable,++CNFGenerator::globalClauseCount));
		
		for (int j=0;j<tables;j++)
		{
			for (int k=j+1;k<tables;k++)
			{
				onePersonAtMaxOneTable.clear();
				Literal newLiteral1(i,j,true);
				Literal newLiteral2(i,k,true);
				onePersonAtMaxOneTable.insert(newLiteral1);
				onePersonAtMaxOneTable.insert(newLiteral2);
				//sort(onePersonAtMaxOneTable.begin(),onePersonAtMaxOneTable.end());
				SetOfClauses.insert(pair<Clause,int> (onePersonAtMaxOneTable,++CNFGenerator::globalClauseCount));
			}
		}
		
	}

	return true;
}

bool CNFGenerator::printCNF()
{
	bool firstRecord;
	for (ClauseSet::iterator setIterator= SetOfClauses.begin();setIterator!= SetOfClauses.end();setIterator++)
	{
		firstRecord=true;
		for (Clause::iterator clauseIter = (setIterator->first).begin();clauseIter != (setIterator->first).end(); clauseIter++ )
		{
			if (firstRecord)
			{
				cout<<*clauseIter ;
				firstRecord=false;
			}
			else 
			{
				cout<<" V " << *clauseIter ;
			}
		}
		cout << endl;
	}
	return true;
}



CNFGenerator::~CNFGenerator()
{
}
