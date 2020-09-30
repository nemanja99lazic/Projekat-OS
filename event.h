/*
 * event.h
 *
 *  Created on: Sep 24, 2020
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "ivtentry.h"

#define PREPAREENTRY(brUlaza, flag) \
	void interrupt inter##brUlaza(...); \
	IVTEntry newEntry##brUlaza(brUlaza, inter##brUlaza);\
	void interrupt inter##brUlaza(...){ \
		newEntry##brUlaza.signal(); \
		if(flag == 1) \
			newEntry##brUlaza.callOld(); \
		dispatch();\
	}

typedef unsigned char IVTNo;
class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
private:
	KernelEv* myImpl;
};

#endif /* EVENT_H_ */
