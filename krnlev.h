/*
 * krnlev.h
 *
 *  Created on: Sep 24, 2020
 *      Author: OS1
 */

#ifndef KRNLEV_H_
#define KRNLEV_H_

typedef unsigned char IVTNo;

class PCB;
class IVTEntry;
//class Event;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();
private:
	//friend class Event;
	PCB* blocked;
	IVTNo numOfEntry;
	PCB* owner;
	IVTEntry* myEntry;
};

#endif /* KRNLEV_H_ */
