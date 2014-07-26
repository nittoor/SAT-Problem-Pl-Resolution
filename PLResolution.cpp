#include "PLResolution.h"

pthread_mutex_t monMutex;
pthread_cond_t condMon;
bool KillAll;
int mFlag=0;

void* callHandle(void *data) {
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  PLResolution *h = static_cast<threadArgument*>(data)->PlResolPtr;
  ClauseSet::iterator i = static_cast<threadArgument*>(data)->iter;
  //cout<< "launched thread" << static_cast<threadArgument*>(data)->threadId<<endl;
  h->launchInThread(i);
  pthread_exit((void*) 0);
}

PLResolution::PLResolution(const ClauseSet & setOfClauses)
{
	SetOfClausesinKB=setOfClauses;
	//sort(SetOfClausesinKB.begin(),SetOfClausesinKB.end());
	//printKB();
	
}

bool PLResolution::printKB()
{
	bool firstRecord;
	for (ClauseSet::iterator setIterator= SetOfClausesinKB.begin();setIterator!= SetOfClausesinKB.end();setIterator++)
	{
		firstRecord=true;
		for (Clause::iterator clauseIter = setIterator->first.begin();clauseIter != setIterator->first.end(); clauseIter++ )
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

bool PLResolution::printNewResolutions()
{
	bool firstRecord;
	for (ClauseSet::iterator setIterator= NewResolvents.begin();setIterator!= NewResolvents.end();setIterator++)
	{
		firstRecord=true;
		for (Clause::iterator clauseIter = setIterator->first.begin();clauseIter != setIterator->first.end(); clauseIter++ )
		{
			if (firstRecord)
			{
				cout << endl;
				cout<<*clauseIter ;
				firstRecord=false;
			}
			else 
			{
				cout<<" V " << *clauseIter ;
			}
		}
	}

return true;
}

void PLResolution::launchInThread( ClauseSet::iterator i)
{
		for (ClauseSet::iterator j=SetOfClausesinKB.begin(); j !=SetOfClausesinKB.end();j++)
			{
				try 
				{
					Clause NewClause;
					if (ResolvedClases.find(pair<int,int>(i->second,j->second)) ==ResolvedClases.end()  && Resolve(i,j,NewClause))
					{
						pthread_mutex_lock (&mutex);
						//cout<<"lock"<<endl;
							++CNFGenerator::globalClauseCount;
							NewResolvents.insert(pair<Clause,int> (NewClause,CNFGenerator::globalClauseCount));
						pthread_mutex_unlock (&mutex);
						//cout<<"unlock"<<endl;
					}
				}
				catch (string x)
				{
					cout << "Not Satisfiable from !! "<< pthread_self() <<endl;
					KillAll=true;
					while(!mFlag);
					pthread_mutex_lock(&monMutex);
					pthread_cond_signal(&condMon);
					cout <<"monitorHadle signalled for"<<endl;
					pthread_mutex_unlock(&monMutex);
					pthread_exit((void*) 0);
				}
			}
}

void* monitorHadle(void* threadArray)
{
pthread_t* temp =static_cast<monitorThreadArg*> (threadArray)->threadArray;
int number =static_cast<monitorThreadArg*> (threadArray)->count;
cout <<"monitorHadle"<<endl;
pthread_mutex_lock(&monMutex);
mFlag=1;
pthread_cond_wait(&condMon,&monMutex);
//cout <<"monitorHadle signalled"<<endl;
pthread_mutex_unlock(&monMutex);
//cout <<"monitorHadle signalled"<<endl;
void * status;
if (KillAll==true)
{
	printf("kill all \n");
	//pthread_mutex_lock (&monMutex);
	for  (int i=0;i<number;i++)
	{
		//cout << "cancelling thread " <<  temp[i].gettid();
		pthread_cancel(temp[i]);
		//pthread_join(temp[i],&status);
	}
	//pthread_mutex_unlock (&monMutex);
}

}

bool PLResolution::ApplyResolution()
{
	int count=0;
	 void *status;
	while (1)
	{
		cout << "iterator =" <<++count<<endl;
		NewResolvents.clear();
		
		int rc;
		int t,s;
		ClauseSet::iterator i=SetOfClausesinKB.begin();
		
		while (i !=SetOfClausesinKB.end())
		{
			t=0;
			pthread_t *threads = (pthread_t*) malloc (THREAD_BUFFER* sizeof(pthread_t));
			pthread_t monitor_thread;
			monitorThreadArg monitorThread;
			threadArgument * argPtr = (threadArgument *) malloc (THREAD_BUFFER* sizeof(threadArgument));
			try
			{
				for (;i !=SetOfClausesinKB.end() && t < THREAD_BUFFER;i++,t++)
				{
					argPtr[t].iter=i;
					argPtr[t].PlResolPtr=this;
					argPtr[t].threadId=t;
					KillAll=false;
					rc = pthread_create(&threads[t], NULL, &(callHandle), (void *)&argPtr[t]);
					//cout<<"launched thread "<<t<<endl;
					if (rc){
						printf("ERROR; return code from pthread_create() is %d\n", rc);
						exit(-1);
					}
					
				}
				monitorThread.count=t;
				monitorThread.threadArray=threads;
				
				rc= pthread_create(&monitor_thread,NULL,&monitorHadle,(void * )(&monitorThread));
					if (rc){
						printf("ERROR; return code from pthread_create() is %d\n", rc);
						exit(-1);
				}
				
				s=0;
				for (s=0; s < t;s++)
				{
					pthread_join(threads[s], &status);
					//cout<<"pthread_createjoined thread" << s<<endl; 
				}

				pthread_cond_signal(&condMon);
				cout << "waiting for monitor thread"<<endl;
				pthread_join(monitor_thread, &status);
				cout << "joined with monitor thread"<<endl;
				if (KillAll==true)
				{
				return false;
				}
				
				KillAll=false;
				
				delete[] threads;
				delete[] argPtr;
			}
			catch (string x)
			{
				cout << "ApplyResolution Not Satisfiable!!"<<endl;
			}
		}
		/*
		t=0;
		for (ClauseSet::iterator i=SetOfClausesinKB.begin(); i !=SetOfClausesinKB.end();i++,t++)
        {
			pthread_join(threads[t], &status);
			//cout<<"joined thread" << t<<endl; 
		}
		*/
		
		/*
		sort(NewResolvents.begin(),NewResolvents.end());
		ClauseSet::iterator lastUniquePoint = unique(NewResolvents.begin(),NewResolvents.end());
		NewResolvents.resize(distance(NewResolvents.begin(),lastUniquePoint));
		*/

		cout<<"New Resolvents!"<<endl;
		//printNewResolutions();
		
		if (NewResolvents.empty())
		{
			cout<<endl<<"No new resolvents!"<<endl;
			return false;
		}
		if (isSubset(NewResolvents,SetOfClausesinKB))
		{
			cout << "SizeFoKB" << SetOfClausesinKB.size()<<endl;
			cout<<"isSubset!!"<<endl;
			return true;
		}
		cout << "SizeFoKB" << SetOfClausesinKB.size()<<endl;
		cout <<endl<< "========================\n";
		//break;
		//printKB();
	}
	return true;
}

bool PLResolution::isSubset( ClauseSet & NewResolvents, ClauseSet& KB)
{
	bool found;
	ClauseSet tempClauseSet;
	int orginalSize=KB.size();
	//cout <<"asdf"<<sizeof(NewResolvents.size())<<endl;
	//printNewResolutions();
	/*
	for (ClauseSet::iterator i =NewResolvents.begin() ; i!=NewResolvents.end();i++)
	{
		//cout <<"asdf"<<endl;
		found=false;
		for (ClauseSet::iterator j=KB.begin();j!=KB.end();j++)
		{
			if (i->first==j->first)
			{
				/*
				for (Clause::iterator clauseIter1 = i->begin(),clauseIter2 = j->begin();clauseIter1 != i->end(); clauseIter1++ )
				{
					
						cout<< *clauseIter1 << "=====" << *clauseIter1  ;
					
				}
				
				
				found=true;
				break;
			}
		}
		if (found==false)
		{
			tempClauseSet.insert(*i);
		}
	}
	*/
	
	KB.insert(NewResolvents.begin(),NewResolvents.end());

	if (orginalSize!=KB.size())
	{
		//KB.insert(tempClauseSet.begin(),tempClauseSet.end());

		//sort(KB.begin(),KB.end());
		//ClauseSet::iterator lastUniquePoint = unique(KB.begin(),KB.end());
		//KB.resize(distance(KB.begin(),lastUniquePoint));


		return false;
	}
	return true;
}

bool PLResolution::checkInKB(const Clause & newClause)
{
	for (ClauseSet::iterator i =SetOfClausesinKB.begin(); i!=SetOfClausesinKB.end();i++)
	{
		if (areEqual((i->first),newClause))
		{
			return true;
		}
	}
	return false;
}

bool PLResolution::areEqual(const Clause& A,const Clause& B)
{
	return A==B;
}

bool PLResolution::Resolve(const ClauseSet::iterator i,const ClauseSet::iterator j,Clause & NewClause)
{	
	//cout<< "Resolve"<<i<<j<<"\n";
	pthread_testcancel();
	Clause tempClauseA, tempClauseB;
	tempClauseA=(i->first);
	tempClauseB=(j->first);
	int numberOfCompliments=0; // should be only one for resolution
	Clause::iterator MatchAtA;
	Clause::iterator MatchAtB;

	try 
	{
		for (Clause::iterator ClauseAIter= tempClauseA.begin();ClauseAIter != tempClauseA.end(); ClauseAIter++)
		{
			for (Clause::iterator ClauseBIter= tempClauseB.begin();ClauseBIter!= tempClauseB.end(); ClauseBIter++)
			{
				if (ClauseAIter->isComplimentary(*ClauseBIter))
				{
					numberOfCompliments++;
					
					if (numberOfCompliments>1)
					{
						throw (numberOfCompliments);
					}

					MatchAtA=ClauseAIter;
					MatchAtB=ClauseBIter;
				}
			}
		}
	}
	
	catch (int x)
	{
		//cout<<"compliemnts crossed one .. cannot resolve"<<endl;
		return false;
	}
	Clause tempClause;
	

	int x=0;
	if (numberOfCompliments==1)
	{
		pthread_mutex_lock(&mutex);
		ResolvedClases.insert(pair<int,int>(i->second,j->second));
		ResolvedClases.insert(pair<int,int>(j->second,i->second));
		pthread_mutex_unlock(&mutex);
		//cout<<endl;
		for (Clause::iterator ClauseAIter= tempClauseA.begin();ClauseAIter != tempClauseA.end(); ClauseAIter++)
		{
			//cout<<" V " << *ClauseAIter ;
			if (ClauseAIter==MatchAtA)
			{
				continue;
			}
			tempClause.insert(*ClauseAIter);
		}

		//cout<< " AND ";
	
		for (Clause::iterator ClauseBIter= tempClauseB.begin();ClauseBIter!= tempClauseB.end(); ClauseBIter++)
		{
			//cout<<" V " << *ClauseBIter ;
			if (ClauseBIter==MatchAtB)
			{
				continue;
			}
			tempClause.insert(*ClauseBIter);
		}

		//cout <<" = ";
	
		NewClause=tempClause;
	
		/*sort clauses */
			if (NewClause.empty())
			{
				string exception="Empty Clause! Not Satisfiable!!";
				throw exception;
			}

		/*remove duplicate literals from clause */
			/*
		sort(NewClause.begin(),NewClause.end());
		Clause::iterator lastUniquePoint = unique(NewClause.begin(),NewClause.end());
		NewClause.resize(distance(NewClause.begin(),lastUniquePoint),Literal(100,100,false));
		*/
		
	
	}
		/*
		bool firstRecord=true;
		for (Clause::iterator clauseIter = NewClause.begin();clauseIter != NewClause.end(); clauseIter++ )
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
		*/
		

		return true;
}

PLResolution::~PLResolution()
{
}
