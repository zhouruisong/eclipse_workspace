/*
 * INetProvider.h
 *
 *  Created on: 2015-4-19
 *      Author: root
 */

#ifndef INETPROVIDER_H_
#define INETPROVIDER_H_

#include "IProtocol.h"

namespace rm
{

class INetProvider
{
public:
	INetProvider();
	virtual ~INetProvider();

	virtual int8_t Create(const char *addr, const int32_t  port) = 0;
	virtual int32_t Send(const int8_t *buff, const int32_t len, const char *addr = NULL,
			const int32_t port = 0) = 0;

	virtual int32_t Recive(void *buff, const size_t len, struct sockaddr *addr,
			socklen_t *addlen) = 0;

	virtual int32_t Getfd();
	virtual void SetBlock(bool block) = 0;
	virtual void Flush() = 0;
	virtual IProtocol *GetProtocol() = 0;

protected:
	int32_t m_fd;
};

}

#endif /* INETPROVIDER_H_ */
