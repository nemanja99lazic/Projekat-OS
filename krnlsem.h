/*
 * krnlsem.h
 *
 *  Created on: Sep 21, 2020
 *      Author: OS1
 */

#ifndef KRNLSEM_H_
#define KRNLSEM_H_

class SemaphoreList;
class BlockedQueue;
class SleepingQueue;

class KernelSem {
public:
	KernelSem(int init);
	int wait(int maxTimeToWait);
	int signal(int n);
	static void tickSemaphore();
	int getVal() const;

	static int semIDgenerator;
	int myID;
	int val;
	static SemaphoreList* semaphoresList;
	SleepingQueue* blockedWaitingForTime;
	BlockedQueue* blockedNegativeValue;

	~KernelSem();

private:
	void blockOnNegativeValue();
	void blockWaitingForTime(int timeToWait);
	int  unblockOnNegativeValue(int n);
};

#endif /* KRNLSEM_H_ */
