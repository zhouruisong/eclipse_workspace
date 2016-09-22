/*
 * IProtocol.h
 *
 *  Created on: 2015-4-19
 *      Author: root
 */

#ifndef IPROTOCOL_H_
#define IPROTOCOL_H_

namespace rm {

class IProtocol
{
public:
	virtual ~IProtocol()
	{
	}

	virtual int32_t Create() = 0;
	virtual void InjectProvider(INetProvider *pNetProvider) = 0;
	virtual INetProvider *GetProvider() = 0;

	virtual int8_t parse(const char *addr, const int32_t port,
			const uint8_t *package, uint32_t length) = 0;

    virtual int8_t package(const char *addr, const int32_t port,
			const uint8_t *package, uint32_t length) = 0;

    virtual int8_t GetPackage(const char *addr, const int32_t port,
			const uint8_t *package, uint32_t length) = 0;
};

}

#endif /* IPROTOCOL_H_ */
