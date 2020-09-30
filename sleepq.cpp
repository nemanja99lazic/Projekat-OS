/*
 * sleepq.cpp
 *
 *  Created on: Sep 22, 2020
 *      Author: OS1
 */

#include "sleepq.h"
#include "semqueue.h"
#include "PCB.h"
#include "lock.h"

SleepingQueue::SleepingQueue() : BlockedQueue() {}

SleepingQueue::~SleepingQueue() {
	lock
	BlockedQueue::Elem* cur = head;
	while(head != 0)
	{
		head = head -> next;
		delete cur;
		cur = head;
	}
	head = 0;
	tail = 0;
	unlock
}

void SleepingQueue::insert(PCB* pcb)
{
	lock
	BlockedQueue::Elem* temp = new BlockedQueue::Elem(pcb);
	if(head == 0)
	{
		head = tail = temp;
	}
	else
	{
		BlockedQueue::Elem* cur = head;
		BlockedQueue::Elem* prev = head;
		if(pcb->timeToWait <= cur->info->timeToWait)
		{
			temp->next = head;
			head = temp;
		}
		else
		{
			pcb->timeToWait -= cur->info->timeToWait;
			cur = cur->next;
			while(pcb->timeToWait >= cur->info->timeToWait && cur != 0)
			{
				pcb->timeToWait -= cur->info->timeToWait;
				prev = cur;
				cur = cur->next;
			}
			prev->next = temp;
			temp->next = cur;
		}

		Elem* greaterTimeElem = temp->next;
		while(greaterTimeElem != 0)
		{
			if(greaterTimeElem->info->timeToWait != 0 && greaterTimeElem->info->timeToWait > pcb->timeToWait)
				greaterTimeElem->info->timeToWait -= pcb->timeToWait;
			greaterTimeElem = greaterTimeElem->next;
		}
		if(temp->next == 0)
			tail = temp;
	}
	unlock
}

