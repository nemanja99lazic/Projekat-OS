/*
 * evlist.cpp
 *
 *  Created on: Sep 24, 2020
 *      Author: OS1
 */

#include "evlist.h"
#include "lock.h"
#include "krnlev.h"

EventList::EventList() : head(0) {
}

void EventList::deleteEvent(KernelEv* event)
{
	lock
	if(event == 0 || head == 0)
	{
		unlock
		return;
	}
	EventList::Elem* cur = head, *prev = head;
	if(cur->info == event)
	{
		head = cur->next;
		delete cur;
		unlock
		return;
	}
	else
	{
		cur = cur->next;
		while(cur != 0 && cur->info != event)
		{
			prev = cur;
			cur = cur->next;
		}
		if(cur != 0 && cur->info == event)
		{
			prev->next = cur->next;
			delete cur;
		}
	}
	unlock
}

void EventList::insert(KernelEv* event)
{
	lock
	EventList::Elem* temp = new Elem(event);
	temp->next = head;
	head = temp;
	unlock
}

void EventList::signal()
{
	lock
	EventList::Elem* cur = head;
	while(cur != 0)
	{
		cur->info->signal();
		cur = cur->next;
	}
	unlock
}

EventList::~EventList() {
	lock
	EventList::Elem* cur = this->head;
	while(head != 0)
	{
		head = head->next;
		delete cur;
		cur = head;
	}
	head = 0;
	unlock
}

