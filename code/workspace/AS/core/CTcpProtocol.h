/*
 * CTcpProtocol.h
 *
 *  Created on: 2015-4-20
 *      Author: root
 */

#ifndef CTCPPROTOCOL_H_
#define CTCPPROTOCOL_H_

#include "IProtocol.h"

namespace rm
{

class CTcpProtocol: public IProtocol
{
public:
	CTcpProtocol();
	virtual ~CTcpProtocol();

	virtual int32_t Create();
	virtual void InjectProvider(INetProvider *pNetProvider);
	virtual INetProvider *GetProvider();

	virtual int8_t parse(const char *addr, const int32_t port,
			const uint8_t *package, uint32_t length);

    virtual int8_t package(const char *addr, const int32_t port,
			const uint8_t *package, uint32_t length);

    virtual int8_t GetPackage(const char *addr, const int32_t port,
			const uint8_t *package, uint32_t length);

private:

    INetProvider *m_pProvider;
    ITxnDriver *m_pTxnDriver;

};

}

#endif /* CTCPPROTOCOL_H_ */
