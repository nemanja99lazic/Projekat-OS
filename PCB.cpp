#include "PCB.h"
#include <dos.h>
#include "Thread.h"
#include "lock.h"
#include "Timer.h"
#include "SCHEDULE.h"
#include "Idle.h"
#include "sighlist.h"
#include "siglist.h"

List* PCB::PCBList = 0;
ID PCB::IDGenerator = 0;
volatile PCB* PCB::running = 0;
Idle* PCB::idle = 0;
PCB* PCB::idlePCB = 0;
Thread* PCB::mainThread = 0;

int PCB::globallyBlockedSignal[NUM_OF_SIGNALS] = {0};

#define MAX_STACK_SIZE 65534

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* _myThread, PCB* _parent) : timeToWait(0), unblockedFromTimer(0), brojPristiglihSignala(0), parent(_parent)
{
	lock
	killed = 0;
	this->myThread = _myThread;

	if(PCBList == 0)
	{
		PCBList = new List();
	}

	stackSize = stackSize / sizeof(unsigned);
	if(stackSize * sizeof(unsigned) > MAX_STACK_SIZE)
		stackSize = MAX_STACK_SIZE / sizeof(unsigned);

	unsigned* st1 = new unsigned[stackSize];

	st1[stackSize - 1] = 0x200; //vrednost PSW (I = 1)

#ifndef BCC_BLOCK_IGNORE
	//cuvaj CS:IP
	st1[stackSize - 2] = FP_SEG(PCB::wrapper);
	st1[stackSize - 3] = FP_OFF(PCB::wrapper);

	//ostavljanje prostora zaa nedefinisani pocetni kontekst
	this->sp = FP_OFF(st1 + stackSize - 12);
	this->ss = FP_SEG(st1 + stackSize - 12);
#endif
	this->stack = st1;
	this->quantum = timeSlice; // pri promeni konteksta obrati paznju ako je timeSlice == 0

	if(this->quantum == 0)
		this->izvrsava_se_neograniceno = 1;
	else
		this->izvrsava_se_neograniceno = 0;

	this->state = INIT;

	myID = IDGenerator++;

	niti_koje_me_cekaju = new List();

	PCBList->add(this);

	for(int i = 0; i < NUM_OF_SIGNALS; i++)
	{
		blockedSignal[i] = parent->blockedSignal[i];
		handlers[i] = new SignalHandlerList(*parent->handlers[i]);
	}

	listaPristiglihSignala = new SignalList();
	unlock
}

PCB::PCB() : timeToWait(0), unblockedFromTimer(0), brojPristiglihSignala(0), parent(0) // Za kreiranje main niti
{
	lock
	killed = 0;
	this->myThread = 0;

	if(PCBList == 0)
		{
			PCBList = new List();
		}

	myID = -1;
	this->stack = 0;
	this->quantum = 1; // MOZDA MAINu TREBA 0
	this->izvrsava_se_neograniceno = 0;
	this->state = RUNNING;
	niti_koje_me_cekaju = new List();
	PCBList->add(this);

	for(int i = 0; i < NUM_OF_SIGNALS; i++)
	{
		blockedSignal[i] = 0;
	}

	listaPristiglihSignala = new SignalList();
	unlock
}

void PCB::makeMainThread()
{
lock
	PCB::mainThread = new Thread();
	PCB* oldpcb = PCB::mainThread->myPCB;
	PCB::mainThread->myPCB = (PCB*)PCB::running;
	PCB::running->myThread = PCB::mainThread;
	delete oldpcb;
unlock
}

void PCB::start()
{
	lock
	if(state == INIT)
	{
		state = READY;
		Scheduler::put(this);
	}
	unlock
}

void PCB::dispatch()
{
	//lock
	Timer::zahtevana_promena_konteksta = 1;
	Timer::timer();
	//unlock
}

void PCB::waitToComplete()
{
	lock
	if(state == TERMINATING || state == INIT || PCB::running == this)
	{
		unlock
		return;
	}
	PCB::running->state = SUSPENDED;
	//ubaci proveru da li vec postoji u listi
	//syncPrintf("Uslo u waitToComplete!!!!!!!!");
	niti_koje_me_cekaju->add((PCB*)PCB::running);
	PCB::dispatch();
	unlock
}

ID PCB::getID()
{
	return myID;
}

ID PCB::getRunningId()
{
	lock
	ID runningID = PCB::running->myID;
	unlock
	return runningID;
}

Thread* PCB::getThreadById(ID id)
{
	lock
	List::Elem* cur = PCB::PCBList->head;
	while(cur != 0)
	{
		if(cur->info->myID == id)
		{
			unlock
			return cur->info->myThread;
		}
		else
			cur = cur->next;
	}
	unlock
	return 0;
}

void PCB::signal(SignalId signal)
{
	lock
	if((signal == 1 || signal == 2) && PCB::running->state != PCB::TERMINATING)
	{
		unlock
		return;
	}
	listaPristiglihSignala->add(signal);
	unlock
}

void PCB::registerHandler(SignalId signal, SignalHandler handler)
{
	lock
	handlers[signal]->add(handler);
	unlock
}

void PCB::unregisterAllHandlers(SignalId id)
{
	lock
	delete handlers[id];
	handlers[id] = new SignalHandlerList();
	unlock
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2)
{
	lock
	handlers[id]->swap(hand1, hand2);
	unlock
}

void PCB::blockSignal(SignalId signal)
{
	lock
	blockedSignal[signal] = 1;
	unlock
}

void PCB::blockSignalGlobally(SignalId signal)
{
	lock
	PCB::globallyBlockedSignal[signal] = 1;
	unlock
}

void PCB::unblockSignal(SignalId signal)
{
	lock
	blockedSignal[signal] = 0;
	unlock
}

void PCB::unblockSignalGlobally(SignalId signal)
{
	lock
	PCB::globallyBlockedSignal[signal] = 0;
	unlock
}

void PCB::obradiSignale()
{
	lock
	Timer::preemptionFlag = 1;
	unlock

	SignalList::Elem* cur = this->listaPristiglihSignala->head;
	while(cur != 0)
	{
		if(globallyBlockedSignal[cur->signal] == 0 && blockedSignal[cur->signal] == 0)
		{
			for(SignalHandlerList::Elem* handlerElem = handlers[cur->signal]->head; handlerElem != 0; handlerElem = handlerElem -> next)
			{
				handlerElem->handler();
			}
			SignalId signalZaBrisanje = cur->signal;
			cur = cur->next;
			listaPristiglihSignala->deleteSignal(signalZaBrisanje);
		}
		else
			cur = cur -> next;
	}

	lock
	Timer::preemptionFlag = 0;
	unlock
}

PCB::~PCB() {
	lock
	if(!killed){
	for(int i = 0; i < NUM_OF_SIGNALS; i++)
		delete handlers[i];
	delete listaPristiglihSignala;
	delete niti_koje_me_cekaju;
	delete stack;
	PCB::PCBList->izbaci(this);
	}
	unlock
}

void PCB::wrapper() {
	PCB::running->myThread->run();
	lock
	PCB::running->state = PCB::TERMINATING;

	//Oslobodi sve suspendovane niti koje su cekale da se ovaj proces zavrsi
	List::Elem* elem_suspendovana = PCB::running->niti_koje_me_cekaju->head;
	while(elem_suspendovana != 0)
	{
		PCB* suspendovana = elem_suspendovana->info;
		suspendovana->state = READY;
		elem_suspendovana = elem_suspendovana->next;
		delete PCB::running->niti_koje_me_cekaju->head;
		PCB::running->niti_koje_me_cekaju->head = elem_suspendovana;
		Scheduler::put(suspendovana);
	}
	PCB::running->parent->signal(2);
	PCB::running->signal(1);
	unlock
	PCB::running->obradiSignale();
	dispatch();
}

