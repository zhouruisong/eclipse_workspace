/*
 * CAsyncController.h
 *
 *  Created on: 2015-4-16
 *      Author: root
 */

#ifndef CASYNCCONTROLLER_H_
#define CASYNCCONTROLLER_H_

#include <sys/types.h>

#include "IAsync.h"
#include "CThread.h"
#include "ICallBack.h"
#include "CStreamPipe.h"

class CAsyncController: public IAsync, public CThread
{
public:
	explcite CAsyncController(ICallBack *pCallBack, u_int32_t count);

	virtual ~CAsyncController();

	int8_t Write(u_int64_t des, u_int64_t len);
	int8_t Read(u_int64_t des, u_int64_t len);

	virtual int8_t SendMessage(void *pMessage);
	virtual int8_t Run(const int8_t thread_index);
	virtual int8_t OnCreate();

	int8_t CreateThreatPool();

private:
	CAsyncController(const rm::CAsyncController&);

	CStreamPipe *m_pipe;
	ICallBack *m_pCallBack;
	u_int32_t m_Count;
	//pthread_mutex_t m_lock;
};

#endif /* CASYNCCONTROLLER_H_ */
