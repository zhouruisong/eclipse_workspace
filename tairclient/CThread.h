#ifndef CTHREAD_H_
#define CTHREAD_H_

#include "Osi.h"
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_PTHREAD_NUM 1024

class CThread
{
public:
	CThread();
	virtual ~CThread();
	virtual void* Run(void * ptr) = 0;
	virtual bool Create(int32_t index);

private:
	int32_t m_pidnum;
	pthread_t m_pids[MAX_PTHREAD_NUM];
};

typedef void *(CThread::*Thread_Fun)(void * ptr);

#endif /* CTHREAD_H_ */
