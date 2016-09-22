/*
 * CStreamPipe.h
 *
 *  Created on: 2015-4-12
 *      Author: root
 */

#ifndef CSTREAMPIPE_H_
#define CSTREAMPIPE_H_

#include <sys/types.h>

namespace rm {

class CStreamPipe
{
public:
	CStreamPipe();
	virtual ~CStreamPipe();

	int8_t OnCreate();
	int32_t GetSocket();
	int32_t GetPeerSocket();
	void SetPeerSocket(int32_t fd);

private:
	int32_t m_fd;
	CStreamPipe *m_peer;
};

}

#endif /* CSTREAMPIPE_H_ */
