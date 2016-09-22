/*
 * IAsync.h
 *
 *  Created on: 2015-4-16
 *      Author: root
 */

#ifndef IASYNC_H_
#define IASYNC_H_

namespace rm {

class IAsync {
public:
	IAsync();
	virtual ~IAsync();

	virtual int8_t SendMessage(void *pctx) = 0;
};

}

#endif /* IASYNC_H_ */
