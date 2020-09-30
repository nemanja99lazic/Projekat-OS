/*
 * Idle.cpp
 *
 *  Created on: Sep 20, 2020
 *      Author: OS1
 */
#include "Idle.h"
#include "thread.h"

Idle::Idle() : Thread(defaultStackSize, 1) {}

Idle::~Idle() {
}

void Idle::run()
{
	while(1){

	}
}

