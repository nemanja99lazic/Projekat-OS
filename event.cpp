/*
 * event.cpp
 *
 *  Created on: Sep 24, 2020
 *      Author: OS1
 */

#include "event.h"
#include "krnlev.h"

Event::Event(IVTNo ivtNo) {
	myImpl = new KernelEv(ivtNo);
}

Event::~Event() {
	delete myImpl;
}

void Event::wait()
{
	myImpl->wait();
}

void Event::signal()
{
	myImpl->signal();
}

