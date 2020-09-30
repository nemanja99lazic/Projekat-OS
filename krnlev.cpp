/*
 * krnlev.cpp
 *
 *  Created on: Sep 24, 2020
 *      Author: OS1
 */

#include "krnlev.h"
#include "ivtentry.h"
#include "evlist.h"
#include "PCB.h"
#include "lock.h"
#include "SCHEDULE.h"

KernelEv::KernelEv(IVTNo ivtNo) : numOfEntry(ivtNo), blocked(0) {

	lock
	owner = (PCB*)PCB::running;
	myEntry = IVTEntry::entries[ivtNo];
	IVTEntry::entries[ivtNo]->eventList->insert(this);
	unlock
}

KernelEv::~KernelEv() {
	lock
	owner = 0;
	IVTEntry::entries[numOfEntry]->eventList->deleteEvent(this);
	myEntry = 0;
	unlock
}

void KernelEv::signal()
{
	lock
	if(blocked != 0)
	{
		blocked->state = PCB::READY;
		Scheduler::put(blocked);
		blocked = 0;
	}
	unlock
}

void KernelEv::wait()
{
	lock
	if(PCB::running == owner)
	{
		//syncPrintf("USLO U WAIT\n");
		blocked = (PCB*)PCB::running;
		PCB::running->state = PCB::SUSPENDED;
		PCB::dispatch();
		//syncPrintf("UNSUSPENDED\n");
		}
	unlock
}

