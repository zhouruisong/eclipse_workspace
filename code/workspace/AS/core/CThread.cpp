/*
 * CThread.cpp
 *
 *  Created on: 2015-4-12
 *      Author: root
 */

#include "CThread.h"

namespace rm
{

#ifdef __cplusplus
extern "C" {
#endif

typedef struct thread_fun_arg
{
	CThread *pThread;
	Thread_Fun func;
	int8_t index;
}thread_fun_arg_t;


static void *thread_fun(void *arg)
{
	thread_fun_arg_t *pFun = static_cast<thread_fun_arg_t *>(*arg);
	if (NULL == pFun)
	{
		return -1;
	}

	(pFun->pThread)->Run(pFun->index);
	free(pFun);
    printf("new thread: ");
    return 0;
}

#ifdef __cplusplus
}
#endif

CThread::CThread()
{
}

CThread::~CThread()
{
}

bool CThread::Create(int8_t index)
{
	if (MAX_PTHREAD_NUM < m_pidnum)
	{
		return false;
	}

	thread_fun_arg_t *pFun = static_cast<thread_fun_arg_t *>(malloc(sizeof(thread_fun_arg_t)));
	if (NULL == pFun)
	{
		return false;
	}

	pFun->func = CThread::&Run;
	pFun->pThread = this;
	pFun->index = index;

	pthread_t pid;
	int8_t ret = -1;
	ret = OsiPthreadCreate(&pid, thread_fun, pFun);
	if (0 == ret)
	{
		pthread_t pid[m_pidnum] = pid;
		++m_pidnum;
	}

	return true;
}

}
