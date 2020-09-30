/*
 * krnlsem.cpp
 *
 *  Created on: Sep 21, 2020
 *      Author: OS1
 */

#include "krnlsem.h"
#include "lock.h"
#include "PCB.h"
#include "semqueue.h"
#include "SCHEDULE.H"
#include "semlist.h"
#include "sleepq.h"

int KernelSem::semIDgenerator = 0;
SemaphoreList* KernelSem::semaphoresList = 0;

KernelSem::KernelSem(int init) : val(init){
	lock
		this->myID = semIDgenerator++;
		this->blockedNegativeValue = new BlockedQueue();
		this->blockedWaitingForTime = new SleepingQueue();
		if(KernelSem::semaphoresList == 0)
		{
			KernelSem::semaphoresList = new SemaphoreList();
		}
		KernelSem::semaphoresList->add(this);
	unlock
}

int KernelSem::wait(int maxTimeToWait)
{
	lock
	val = val - 1;
	if(maxTimeToWait == 0)
	{
		if(val < 0)
			blockOnNegativeValue();
	}
	else
	{
		blockWaitingForTime(maxTimeToWait);
	}

	if(PCB::running->unblockedFromTimer)
	{
		PCB::running->unblockedFromTimer = 0;
		unlock
		return 0;
	}
	else
	{
		unlock
		return 1;
	}
}

void KernelSem::blockOnNegativeValue()
{
	lock
	blockedNegativeValue->insert((PCB*)PCB::running);
	PCB::running->state = PCB::SUSPENDED;
	PCB::dispatch();
	unlock
}

void KernelSem::blockWaitingForTime(int timeToWait)
{
	lock
	PCB::running->timeToWait = timeToWait;
	blockedWaitingForTime->insert((PCB*)PCB::running);
	PCB::running->state = PCB::SUSPENDED;
	PCB::dispatch();
	unlock
}

int KernelSem::signal(int n)
{
	lock
	if(n < 0)
	{
		unlock
		return n;
	}
	if(n == 0)
	{
		//syncPrintf("NIT KOJA DEBLOKIRA: %d\n", PCB::running->myID);
		val = val + 1;
		if(val <= 0)
		{
			int ret = unblockOnNegativeValue(1);
			unlock
			return 0;
		}
	}

	int ret = unblockOnNegativeValue(n);
	val += ret;
	unlock
	return ret;
}

int KernelSem::unblockOnNegativeValue(int n)
{
	lock
	int numOfUnblockedThreads = 0;
	BlockedQueue::Elem* cur = 0;
	while(blockedNegativeValue->head != 0 && n > 0)
	{
		cur = blockedNegativeValue->head;
		cur->info->state = PCB::READY;
		cur->info->unblockedFromTimer = 0;
		Scheduler::put(cur->info);

		blockedNegativeValue->head = blockedNegativeValue->head->next;
		if(blockedNegativeValue->tail == cur)
			blockedNegativeValue->tail = 0;
		delete cur;
		n--;
		numOfUnblockedThreads++;
	}
	while(n > 0 && blockedWaitingForTime->head != 0)
	{
		cur = blockedWaitingForTime->head;
		cur->info->state = PCB::READY;
		cur->info->unblockedFromTimer = 0;
		if(cur->next != 0 && cur->next->info->timeToWait == 0)
			cur->next->info->timeToWait = cur->info->timeToWait;
		cur->info->timeToWait = 0;
		Scheduler::put(cur->info);

		blockedWaitingForTime->head = blockedWaitingForTime->head->next;
		if(blockedWaitingForTime->tail == cur)
			blockedWaitingForTime->tail = 0;
		delete cur;
		n--;
		numOfUnblockedThreads++;
	}
	unlock
	return numOfUnblockedThreads;
}

void KernelSem::tickSemaphore()
{
	//lock
	if(KernelSem::semaphoresList == 0)
	{
		//unlock
		return;
	}
	for(SemaphoreList::Elem* curSemElem = KernelSem::semaphoresList->head; curSemElem != 0; curSemElem = curSemElem -> next)
	{
		BlockedQueue::Elem* curBlockedPCBElem = curSemElem->info->blockedWaitingForTime->head;
		if(curBlockedPCBElem == 0)
			continue;

		curBlockedPCBElem->info->timeToWait--;
		if(curBlockedPCBElem->info->timeToWait == 0)
		{
			while(curBlockedPCBElem != 0 && curBlockedPCBElem->info->timeToWait == 0)
			{
				curBlockedPCBElem->info->unblockedFromTimer = 1;
				curBlockedPCBElem->info->state = PCB::READY;
				Scheduler::put(curBlockedPCBElem->info);

				//Izbaci element iz liste
				curBlockedPCBElem = curBlockedPCBElem -> next;

				delete curSemElem->info->blockedWaitingForTime->head;

				curSemElem->info->blockedWaitingForTime->head = curBlockedPCBElem;
				if(curBlockedPCBElem == 0)
					curSemElem->info->blockedWaitingForTime->tail = 0;
			}
			//curSemElem->info->blockedWaitingForTime->deleteElem(curBlockedPCBElem);
		}
	}
	//unlock
}

int KernelSem::getVal() const
{
	return val;
}

KernelSem::~KernelSem() {
	lock
	delete blockedWaitingForTime;
	delete blockedNegativeValue;
	semaphoresList->izbaci(this);
	unlock
}

