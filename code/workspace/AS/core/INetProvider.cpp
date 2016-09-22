/*
 * INetProvider.cpp
 *
 *  Created on: 2015-4-19
 *      Author: root
 */

#include "INetProvider.h"

namespace rm {

INetProvider::INetProvider()
{
	m_fd = -1;
}

INetProvider::~INetProvider()
{
}

int32_t INetProvider::Getfd()
{
	return m_fd;
}

}
