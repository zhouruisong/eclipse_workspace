/*
 * CTcpProtocol.cpp
 *
 *  Created on: 2015-4-20
 *      Author: root
 */

#include "CTcpProtocol.h"

namespace rm {

CTcpProtocol::CTcpProtocol()
{
	m_pProvider = NULL;
	m_pTxnDriver = NULL;
}

CTcpProtocol::~CTcpProtocol()
{
}

int32_t CTcpProtocol::Create()
{
    return 0;
}

void CTcpProtocol::InjectProvider(INetProvider *pNetProvider)
{
	m_pProvider = pNetProvider;
}

INetProvider *CTcpProtocol::GetProvider()
{
    return m_pProvider;
}

int8_t CTcpProtocol::parse(const char *addr, const int32_t port,
		const uint8_t *package, uint32_t length)
{
	if (NULL == m_pProvider)
	{
		return -1;
	}

	return 0;
}

int8_t CTcpProtocol::package(const char *addr, const int32_t port,
		const uint8_t *package, uint32_t length)
{
	return 0;
}

int8_t CTcpProtocol::GetPackage(const char *addr, const int32_t port,
		const uint8_t *package, uint32_t length)
{
	return 0;
}



}
