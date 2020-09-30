#ifndef TIMER_H_
#define TIMER_H_

#include <dos.h>

extern void tick(void);

class Timer {
public:
	static void interrupt timer(...);
	static volatile int brojac;
	static volatile int zahtevana_promena_konteksta;
	static volatile int preemptionFlag;
};

#endif /* TIMER_H_ */
