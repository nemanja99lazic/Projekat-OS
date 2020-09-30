/*
 * semlist.cpp
 *
 *  Created on: Sep 21, 2020
 *      Author: OS1
 */

#include "semlist.h"
#include "lock.h"

SemaphoreList::SemaphoreList() {
	lock
	head = 0;
	unlock
}

SemaphoreList::~SemaphoreList() {
	lock
	SemaphoreList::Elem* cur;
	while(head)
	{
		cur = head;
		head = head->next;
		delete cur;
	}
	unlock
}

void SemaphoreList::izbaci(KernelSem* sem)
{
	lock
	if(sem == 0 || head == 0)
	{
		unlock
		return;
	}
	SemaphoreList::Elem* cur = head, *prev = head;
	if(cur->info == sem)
	{
		head = head -> next;
		delete cur;
		unlock
		return;
	}
	else
	{
		cur = cur->next;
		while(cur != 0 && cur->info != sem)
		{
			prev = cur;
			cur = cur->next;
		}
		if(cur != 0 && cur->info == sem)
		{
			prev->next = cur->next;
			delete cur;
		}
	}
	unlock
}

void SemaphoreList::add(KernelSem* sem)
{
	lock
	SemaphoreList::Elem* temp = new SemaphoreList::Elem(sem);
	temp->next = head;
	head = temp;
	unlock
}

