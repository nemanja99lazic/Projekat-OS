#ifndef LOCK_H_
#define LOCK_H_
#include <dos.h>

//static unsigned int lockvalue = 0;

#define lock asm{\
	pushf;\
	cli;\
}

#define unlock asm {\
	popf;\
}



#endif /* LOCK_H_ */
