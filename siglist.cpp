/*
 * siglist.cpp
 *
 *  Created on: Sep 27, 2020
 *      Author: OS1
 */

#include "siglist.h"
#include "lock.h"

SignalList::SignalList() {
	lock
	head = 0;
	unlock
}

void SignalList::add(SignalId signal)
{
	lock
	if(head == 0)
	{
		SignalList::Elem* temp = new Elem(signal);
		head = temp;
		unlock
		return;
	}
	for(SignalList::Elem* cur = head; cur -> next != 0 && cur->signal != signal; cur = cur->next);
	if(cur->signal == signal)
	{
		unlock
		return;
	}
	else
	{
		SignalList::Elem* temp = new Elem(signal);
		cur->next = temp;
		unlock
		return;
	}
	unlock
	return;
}

SignalList::~SignalList() {
	lock
	SignalList::Elem* cur = head;
	while(cur != 0)
	{
		head = head -> next;
		delete cur;
		cur = head;
	}
	unlock
	return;
}

void SignalList::deleteSignal(SignalId signal)
{
	lock
	if(head == 0)
	{
		unlock
		return;
	}
	if(head -> signal == signal)
	{
		SignalList::Elem* cur = head;
		head = head -> next;
		delete cur;
		unlock
		return;
	}
	else
	{
		SignalList::Elem* prev = head, *cur = head->next;
		while(cur != 0 && cur -> signal != signal)
		{
			prev = cur;
			cur = cur -> next;
		}
		if(cur != 0 && cur -> signal == signal)
		{
			prev->next = cur->next;
			delete cur;
		}
	}
	unlock
	return;
}

