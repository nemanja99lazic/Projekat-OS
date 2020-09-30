/*
 * List.cpp
 *
 *  Created on: Sep 6, 2020
 *      Author: OS1
 */

#include "List.h"
#include "lock.h"

void List::add(PCB* _pcb)
{
	lock
	if(head == 0)
	{
		head = new Elem(_pcb);
	}
	else
	{

		Elem* temp = new Elem(_pcb);
		temp->next = head;
		head = temp;
	}
	unlock
}

List::List() {
	lock
	head = 0;
	unlock
}

List::~List() {
	lock
	List::Elem* cur;
	while(head)
	{
		cur = head;
		head = head->next;

		delete cur;

	}
	unlock
}

void List::izbaci(PCB* pcb)
{
	lock
	if(pcb == 0 || head == 0)
	{
		unlock
		return;
	}
	List::Elem* cur = head, *prev = head;
	if(cur->info == pcb)
	{
		head = head->next;
		delete cur;
		unlock
		return;
	}
	else
	{
		cur = cur->next;
		while(cur != 0 && cur->info != pcb)
		{
			prev = cur;
			cur = cur->next;
		}
		if(cur != 0 && cur->info == pcb)
		{
			prev->next = cur->next;
			delete cur;
		}
	}
	unlock
}
