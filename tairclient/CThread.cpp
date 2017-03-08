#include "CThread.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct thread_fun_arg
{
	CThread *pThread;
	Thread_Fun func;
	int8_t index;
} thread_fun_arg_t;

static void *thread_fun(void *arg)
{
	thread_fun_arg_t *pFun = static_cast<thread_fun_arg_t *>(arg);
	if (NULL == pFun)
	{
		return NULL;
	}

	(pFun->pThread)->Run((void *)pFun);
	free(pFun);
	return NULL;
}

#ifdef __cplusplus
}
#endif

CThread::CThread()
{
	m_pidnum = 0;
}

CThread::~CThread()
{
}

bool CThread::Create(int32_t index)
{
	if (MAX_PTHREAD_NUM < m_pidnum)
	{
		return false;
	}

	thread_fun_arg_t *pFun = static_cast<thread_fun_arg_t *>(malloc(
			sizeof(thread_fun_arg_t)));
	if (NULL == pFun)
	{
		return false;
	}

	pFun->func = &CThread::Run;
	pFun->pThread = this;
	pFun->index = index;

	pthread_t t_pid;
	int8_t ret = -1;
	ret = OsiPthreadCreate(&t_pid, thread_fun, pFun);
	if (0 == ret)
	{
		m_pids[m_pidnum] = t_pid;
		++m_pidnum;
	}

	return true;
}
