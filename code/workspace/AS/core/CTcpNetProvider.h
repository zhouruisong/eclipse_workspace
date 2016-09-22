/*
 * CTcpNetProvider.h
 *
 *  Created on: 2015-4-21
 *      Author: root
 */

#ifndef CTCPNETPROVIDER_H_
#define CTCPNETPROVIDER_H_

namespace rm {

class CTcpNetProvider: public INetProvider
{
public:
	CTcpNetProvider();
	virtual ~CTcpNetProvider();

	virtual int8_t Create(const char *addr, const int32_t  port);
	virtual int32_t Send(const int8_t *buff, const int32_t len, const char *addr = NULL,
			const int32_t port = 0);

	virtual int32_t Recive(void *buff, const size_t len, struct sockaddr *addr,
			socklen_t *addlen);

	int32_t Accept(const size_t len, struct sockaddr *addr, socklen_t *addlen);

	virtual void SetBlock(bool block);
	virtual void Flush() = 0;
	virtual IProtocol *GetProtocol();

private:
	struct sockaddr m_addr;
	IProtocol *m_pProtocol;
	int32_t m_socket;
};

}

#endif /* CTCPNETPROVIDER_H_ */
