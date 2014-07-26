#include "CNFGenerator.h"
#include <pthread.h>
#define THREAD_BUFFER 100

class PLResolution
{
public:
	PLResolution(const ClauseSet & );
	~PLResolution();
	bool ApplyResolution();
	bool Resolve(const ClauseSet::iterator i,const ClauseSet::iterator j,Clause & NewClause);
	bool checkInKB(const Clause & newClause);
	bool areEqual(const Clause& A,const Clause& B);
	bool isSubset( ClauseSet & NewResolvents, ClauseSet& KB);
	bool printKB();
	bool printNewResolutions();
	void launchInThread(ClauseSet::iterator i);
private:
	ClauseSet SetOfClausesinKB;
	ClauseSet NewResolvents;
	set <pair<int,int> > ResolvedClases;
	pthread_mutex_t mutex;
	//pthread_mutex_t mapMutex;
};

typedef struct threadArgument_t{
ClauseSet::iterator iter;
PLResolution * PlResolPtr;
int threadId;
}threadArgument;

typedef struct monitorThreadArg_t{
int count;
pthread_t * threadArray;
}monitorThreadArg;

void* callHandle(void *data);
void* monitorHadle(void* threadArray);