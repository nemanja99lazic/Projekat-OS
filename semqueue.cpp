/*
 * semqueue.cpp
 *
 *  Created on: Sep 21, 2020
 *      Author: OS1
 */

#include "semqueue.h"
#include "lock.h"

BlockedQueue::BlockedQueue() {
	lock
	head = 0;
	tail = 0;
	unlock
}

void BlockedQueue::insert(PCB* pcb)
{
	lock
	BlockedQueue::Elem* temp = new BlockedQueue::Elem(pcb);

	if(head == 0)
		head = temp;

	if(tail != 0)
		tail->next = temp;

	tail = temp;
	unlock
}

void BlockedQueue::deleteElem(Elem* elem)
{
	lock
	if(head == 0 && tail == 0)
	{
		unlock
		return;
	}
	if(head == elem)
	{
		head = head->next;
		delete elem;
		unlock
		return;
	}
	BlockedQueue::Elem* prev = head;
	while(prev->next != elem)
		prev = prev -> next;
	prev->next = elem -> next;
	if(tail == elem)
			tail = prev;
	delete elem;
	unlock
}

BlockedQueue::~BlockedQueue() {
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
	return;
}

