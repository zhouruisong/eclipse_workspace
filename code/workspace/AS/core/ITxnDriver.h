/*
 * ITxnDriver.h
 *
 *  Created on: 2015-4-21
 *      Author: root
 */

#ifndef ITXNDRIVER_H_
#define ITXNDRIVER_H_

namespace rm {

class ITxnDriver {
public:
	virtual ~ITxnDriver() {
	}

	virtual uint32_t DoTranaction(const TxnContext *context,
			const uint8_t *package, const uint32_t length) = 0;
};

}

#endif /* ITXNDRIVER_H_ */
