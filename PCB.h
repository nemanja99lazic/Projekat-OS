#ifndef PCB_H_
#define PCB_H_

#include "List.h"

#define NUM_OF_SIGNALS 16

typedef unsigned long StackSize;
typedef unsigned int Time; // time, x 55ms
typedef int ID;

typedef void (*SignalHandler)();
typedef unsigned SignalId;

class Thread;
class Idle;
class SignalHandlerList;
class SignalList;

class PCB {
public:
	enum ThreadState{INIT, TERMINATING, READY, RUNNING, SUSPENDED};
	static ID IDGenerator;

	PCB(StackSize stackSize, Time timeSlice, Thread* _myThread, PCB* _parent = 0);
	PCB();
	~PCB();

	static void wrapper();
	static void dispatch();

	void start();
	void waitToComplete();
	ID getID();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

	friend class Thread;

	static void makeMainThread();

	Thread* myThread;
	PCB* parent;

	unsigned ss;
	unsigned sp;
	unsigned* stack;
	int quantum;
	int timeToWait;
	int unblockedFromTimer;
	int izvrsava_se_neograniceno;
	ThreadState state;
	ID myID;
	List* niti_koje_me_cekaju;

	static volatile PCB* running;
	static Thread* mainThread;
	static Idle* idle;
	static PCB* idlePCB;
	static List* PCBList;

	//signali
	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

	int brojPristiglihSignala;
	void obradiSignale();

	SignalHandlerList* handlers[NUM_OF_SIGNALS];
	static int globallyBlockedSignal[NUM_OF_SIGNALS];
	int blockedSignal[NUM_OF_SIGNALS];
	//int pristigaoSignal[NUM_OF_SIGNALS];
	SignalList* listaPristiglihSignala;

	int killed;

};

#endif /* PCB_H_ */
