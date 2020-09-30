#include "SCHEDULE.H"
#include "Timer.h"
#include "PCB.h"
#include "lock.h"
#include "krnlsem.h"

unsigned volatile tss, tsp;
volatile PCB* oldRunning;
volatile int Timer::brojac = 0;
volatile int Timer::zahtevana_promena_konteksta = 0;
volatile int Timer::preemptionFlag = 0;

void interrupt Timer::timer(...) {
	if(!Timer::zahtevana_promena_konteksta && !PCB::running->izvrsava_se_neograniceno)
			Timer::brojac--;
	if((Timer::brojac == 0 && !PCB::running->izvrsava_se_neograniceno) || Timer::zahtevana_promena_konteksta) {
		if(preemptionFlag == 0){
			asm {
				// cuva sp
				mov tsp, sp
				mov tss, ss
			}

			PCB::running->sp = tsp;
			PCB::running->ss = tss;

			oldRunning = PCB::running;

			if(oldRunning->state != PCB::TERMINATING && oldRunning->state != PCB::SUSPENDED)
			{
				oldRunning->state = PCB::READY;
				if(oldRunning != PCB::idlePCB)
					Scheduler::put((PCB*)oldRunning);
			}

			PCB::running = Scheduler::get();	// Scheduler

			if(PCB::running == 0) // pusti idle nit
			{
				PCB::running = PCB::idlePCB;
			}

			PCB::running->state = PCB::RUNNING;

			tsp = PCB::running->sp;
			tss = PCB::running->ss;

			Timer::brojac = PCB::running->quantum;

			asm {
				mov sp, tsp   // restore sp
				mov ss, tss
			}
			PCB::running->obradiSignale();
	}
	if(!Timer::zahtevana_promena_konteksta)
	{
		asm int 60h;
		tick();
		KernelSem::tickSemaphore();
	}

	Timer::zahtevana_promena_konteksta = 0;

	}
}
