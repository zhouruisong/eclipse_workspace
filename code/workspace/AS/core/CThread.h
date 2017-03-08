/*
 * CThread.h
 *
 *  Created on: 2015-4-12
 *      Author: root
 */

#ifndef CTHREAD_H_
#define CTHREAD_H_

#include <pthread.h>

namespace rm
{

#define MAX_PTHREAD_NUM 1024

class CThread
{
public:
	CThread();
	virtual ~CThread();
	virtual bool Create(int8_t index);

	virtual int8_t Run(const int8_t thread_index) = 0;

private:
	int32_t m_pidnum;
	pthread_t pid[MAX_PTHREAD_NUM];
};

typedef void *(CThread::*Thread_Fun)(void *ptr);
}

#endif /* CTHREAD_H_ */
