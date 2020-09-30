/*
 * sleepq.h
 *
 *  Created on: Sep 22, 2020
 *      Author: OS1
 */

#ifndef SLEEPQ_H_
#define SLEEPQ_H_

#include "semqueue.h"

class SleepingQueue: public BlockedQueue {
public:
	SleepingQueue();
	void insert(PCB* pcb);
	~SleepingQueue();
};

#endif /* SLEEPQ_H_ */
