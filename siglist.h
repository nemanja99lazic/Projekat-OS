/*
 * siglist.h
 *
 *  Created on: Sep 27, 2020
 *      Author: OS1
 */

#ifndef SIGLIST_H_
#define SIGLIST_H_

typedef unsigned SignalId;

class PCB;

class SignalList {
public:
	SignalList();
	void add(SignalId signal);
	virtual ~SignalList();

	struct Elem{
		SignalId signal;
		Elem* next;

		Elem(SignalId _signal)
		{
			signal = _signal;
			next = 0;
		}
	};

	void deleteSignal(SignalId signal);
private:
	friend class PCB;
	Elem* head;
};

#endif /* SIGLIST_H_ */
