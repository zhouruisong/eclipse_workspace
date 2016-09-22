/*
 * CMathndrControllor.h
 *
 *  Created on: 2015-4-10
 *      Author: root
 */

#ifndef CMATHNDRCONTROLLOR_H_
#define CMATHNDRCONTROLLOR_H_

#include "IController.h"

namespace rm
{

class CMathndrControllor: public IController
{
public:
	CMathndrControllor();
	virtual ~CMathndrControllor();

	virtual bool InjectEvents();
	virtual bool Init();
	virtual bool Run();
	static CMathndrControllor *GetInstance();
//	void PutMessage();
private:
	static CMathndrControllor *s_Instance;
	IController *m_pController;
	CommonEvent m_comm[1];
};

}

#endif /* CMATHNDRCONTROLLOR_H_ */
