/*
 * Idle.h
 *
 *  Created on: Sep 20, 2020
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include"thread.h"

class Idle : public Thread{
public:
	Idle();
	~Idle();
protected:
	void run();
};

#endif /* IDLE_H_ */
