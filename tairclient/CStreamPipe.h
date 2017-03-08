#ifndef CSTREAMPIPE_H_
#define CSTREAMPIPE_H_

#include <sys/types.h>

class CStreamPipe
{
public:
	CStreamPipe();
	virtual ~CStreamPipe();

	int8_t OnCreate();

	void SetReadSocket(int32_t fd);
	int32_t GetWriteSocket();
	int32_t GetReadSocket();

private:
	int32_t m_Read;
	int32_t m_Write;
};

#endif /* CSTREAMPIPE_H_ */
