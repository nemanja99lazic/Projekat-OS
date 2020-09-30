/*
 * ivtentry.h
 *
 *  Created on: Sep 24, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#define NUM_OF_ENTRIES 256

typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;

class EventList;
class KernelEv;

extern void reset();

class IVTEntry {
public:
	IVTEntry(IVTNo brUlaza, pInterrupt newRoutine);
	~IVTEntry();
	void signal();
	void callOld();
private:
	friend class KernelEv;
	friend void reset();
	EventList* eventList;
	static IVTEntry* entries[NUM_OF_ENTRIES];
	IVTNo numOfEntry;
	pInterrupt oldRoutine;
	pInterrupt newRoutine;
};

#endif /* IVTENTRY_H_ */
