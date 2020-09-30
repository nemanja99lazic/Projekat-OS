/*
 * evlist.h
 *
 *  Created on: Sep 24, 2020
 *      Author: OS1
 */

#ifndef EVLIST_H_
#define EVLIST_H_

class IVTEntry;
class KernelEv;

class EventList {
public:
	struct Elem{
			KernelEv* info;
			Elem* next;

			Elem(KernelEv* _info){
				info = _info;
				next = 0;
			}
		};

	EventList();
	void insert(KernelEv*);
	void deleteEvent(KernelEv*);
	~EventList();
private:
	void signal();
	friend class IVTEntry;
	Elem* head;
};

#endif /* EVLIST_H_ */
