/*
 * sighlist.cpp
 *
 *  Created on: Sep 27, 2020
 *      Author: OS1
 */

#include "sighlist.h"
#include "lock.h"

SignalHandlerList::SignalHandlerList() {
	lock
	head = 0;
	tail = 0;
	unlock
}

SignalHandlerList::SignalHandlerList(const SignalHandlerList& orig)
{
	lock
	head = 0;
	tail = 0;
	SignalHandlerList::Elem* cur = orig.head;
	if(cur == 0)
	{
		unlock
		return;
	}

	tail = head = new SignalHandlerList::Elem(cur->handler);
	cur = cur->next;
	SignalHandlerList::Elem* temp = 0;
	while(cur != 0)
	{
		temp = new SignalHandlerList::Elem(cur->handler);
		tail -> next = temp;
		tail = temp;
		cur = cur -> next;
	}
	unlock
	return;
}

void SignalHandlerList::add(SignalHandler handler)
{
	lock
	SignalHandlerList::Elem* temp = new SignalHandlerList::Elem(handler);
	if(head == 0 && tail == 0)
	{
		head = tail = temp;
		unlock
		return;
	}
	tail->next = temp;
	tail = temp;
	unlock
	return;
}

void SignalHandlerList::swap(SignalHandler hand1, SignalHandler hand2)
{
	lock
	Elem* cur1 = head;
	Elem* cur2 = head;
	while(cur1 != 0 && cur1->handler != hand1)
		cur1 = cur1 -> next;
	while(cur2 != 0 && cur2->handler != hand2)
		cur2 = cur2 -> next;
	if(cur1 != 0 && cur2 != 0)
	{
		cur2->handler = hand1;
		cur1->handler = hand2;
	}
	unlock
}

SignalHandlerList::~SignalHandlerList() {
	lock
	SignalHandlerList::Elem* cur = head;
	while(cur != 0)
	{
		head = head->next;
		delete cur;
		cur = head;
	}
	unlock
}

