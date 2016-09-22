/*
 * CMutexLock.h
 *
 *  Created on: 2015-4-22
 *      Author: root
 */

#ifndef THREAD_CMUTEXLOCK_H_
#define THREAD_CMUTEXLOCK_H_

#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>

class ILock
{
public:
	virtual ~ILock() {}
	virtual int8_t Create() = 0;
protected:
	virtual void Lock() = 0;
	virtual void UnLock() = 0;
	friend class CLockGuard;
};

class CMutexCondition;

class CMutexLock: public ILock
{
public:
	CMutexLock();
	~CMutexLock();
	int8_t Create();
protected:
	CMutexLock(const CMutexLock &other);
	CMutexLock& operator =(const CMutexLock &other);
	void Lock();
	void UnLock();
private:
	pthread_mutex_t m_mutex;
	friend class CMutexCondition;
};

//互斥锁
class CLockGuard
{
public:
	explicit CLockGuard(CMutexLock *pLock);
	~CLockGuard();
	void Lock();
	void Unlock();
protected:
	CLockGuard(const CLockGuard &other);
	CLockGuard& operator =(const CLockGuard &other);
private:
	ILock *m_pLock;
	bool m_isLock;
};

// 条件锁
class CMutexCondition
{
public:
	CMutexCondition();
	explicit CMutexCondition(CMutexLock *pLock);
	~CMutexCondition();
	void signal();
	void Wait();
	int32_t TimeWait(int32_t seconds);
private:
	pthread_mutex_t *p_mutex;
	pthread_cond_t m_cond;
};

#endif /* THREAD_CMUTEXLOCK_H_ */
