/*
 * IController.h
 *
 *  Created on: 2015-4-17
 *      Author: root
 */

#ifndef ICONTROLLER_H_
#define ICONTROLLER_H_

namespace rm {

class IController
{
public:
	virtual ~IController()
	{
	}

	virtual void Run() = 0;
	virtual bool InjectEvents() = 0;
	virtual bool Init();
};

}

#endif /* ICONTROLLER_H_ */
