/*
 * semqueue.h
 *
 *  Created on: Sep 21, 2020
 *      Author: OS1
 */

#ifndef SEMQUEUE_H_
#define SEMQUEUE_H_

class KernelSem;
class PCB;

class BlockedQueue {
public:
	BlockedQueue();

	struct Elem{
		PCB* info;
		Elem* next;

		Elem(PCB* pcb)
		{
			info = pcb;
			next = 0;
		}
	};

	virtual void insert(PCB* pcb);
	void deleteElem(Elem*);
	virtual ~BlockedQueue();
protected:
	friend class KernelSem;
	Elem *head, *tail;
};

#endif /* SEMQUEUE_H_ */
