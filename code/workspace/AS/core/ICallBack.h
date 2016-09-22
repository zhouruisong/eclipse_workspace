/*
 * ICallBack.h
 *
 *  Created on: 2015-4-16
 *      Author: root
 */

#ifndef ICALLBACK_H_
#define ICALLBACK_H_

namespace rm {

class ICallBack {
public:
	ICallBack();
	virtual ~ICallBack();

	virtual int8_t Doaction(void *pctx) = 0;
	virtual int8_t CallBack(void *pctx) = 0;
};

}

#endif /* ICALLBACK_H_ */
