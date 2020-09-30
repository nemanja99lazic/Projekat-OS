#include <dos.h>
#include "PCB.h"
#include "Timer.h"
#include "lock.h"
#include "Idle.h"
#include "List.h"
#include "krnlsem.h"
#include "semlist.h"
#include "ivtentry.h"
#include "sighlist.h"
#include "siglist.h"
#include "SCHEDULE.h"
#include "thread.h"

typedef void interrupt (*pInterrupt)(...);
static pInterrupt oldRoutine;

void init()
{
	lock
#ifndef BCC_BLOCK_IGNORE
		oldRoutine = getvect(0x08);
		setvect(0x08, Timer::timer);
		setvect(0x60, oldRoutine);
#endif
	PCB::running = new PCB();
	PCB::makeMainThread();
	Timer::brojac = PCB::running->quantum;
	Timer::zahtevana_promena_konteksta = 0;
	for(int i = 0; i < NUM_OF_SIGNALS; i++)
	{
		PCB::running->handlers[i] = new SignalHandlerList();
		PCB::running->blockedSignal[i] = 0;
		PCB::globallyBlockedSignal[i] = 0;
	}
	PCB::idle = new Idle();
	PCB::idlePCB = PCB::PCBList->head->info;
	PCB::idlePCB->state = PCB::READY;
	SignalHandler funKill = &killThread;
	PCB::running->handlers[0]->add(funKill);
	unlock
}

void reset()
{
	lock
	PCB::running->state = PCB::TERMINATING;
	for(int i = 0; i < NUM_OF_SIGNALS; i++)
		delete PCB::running->handlers[i];
	#ifndef BCC_BLOCK_IGNORE
		setvect(0x08, oldRoutine);
	#endif
	//delete PCB::idle;
	//delete PCB::PCBList;
	delete KernelSem::semaphoresList;
	PCB::idle = 0;
	unlock
}

void killThread()
{
	PCB::running->state = PCB::TERMINATING;

	//Oslobodi sve suspendovane niti koje su cekale da se ovaj proces zavrsi
	List::Elem* elem_suspendovana = PCB::running->niti_koje_me_cekaju->head;
	while(elem_suspendovana != 0)
	{
		PCB* suspendovana = elem_suspendovana->info;
		suspendovana->state = PCB::READY;
		elem_suspendovana = elem_suspendovana->next;
		delete PCB::running->niti_koje_me_cekaju->head;
		PCB::running->niti_koje_me_cekaju->head = elem_suspendovana;
		Scheduler::put(suspendovana);
	}
	PCB::running->parent->signal(2);

	for(int i = 0; i < NUM_OF_SIGNALS; i++)
		delete PCB::running->handlers[i];
	delete PCB::running->listaPristiglihSignala;
	delete PCB::running->niti_koje_me_cekaju;
	delete PCB::running->stack;
	PCB::PCBList->izbaci((PCB*)PCB::running);
	PCB::running->killed = 1;

	asm int 60h;
	KernelSem::tickSemaphore();
	tick();

	lock
	Timer::preemptionFlag = 0;
	unlock
	PCB::dispatch();
}



