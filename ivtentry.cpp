#include "ivtentry.h"
#include "lock.h"
#include "evlist.h"
#include <dos.h>

IVTEntry* IVTEntry::entries[NUM_OF_ENTRIES] = {0};

IVTEntry::IVTEntry(IVTNo _brUlaza, pInterrupt _newRoutine) : numOfEntry(_brUlaza), newRoutine(_newRoutine) {
	lock
	eventList = new EventList();

	pInterrupt old;
#ifndef BCC_BLOCK_IGNORE
	old = getvect(_brUlaza);
	setvect(_brUlaza, _newRoutine);
#endif
	this->oldRoutine = old;
	IVTEntry::entries[numOfEntry] = this;
	unlock
}

void IVTEntry::callOld()
{
	this->oldRoutine();
}

void IVTEntry::signal()
{
	lock
	eventList->signal();
	unlock
}

IVTEntry::~IVTEntry() {
	lock
	int entry = (int)numOfEntry;
	pInterrupt old = this->oldRoutine;
	#ifndef BCC_BLOCK_IGNORE
		setvect(entry, old);
	#endif
	//delete eventList;
	//IVTEntry::entries[numOfEntry] = 0;
	unlock
	oldRoutine();
}

