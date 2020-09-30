#ifndef SEMLIST_H_
#define SEMLIST_H_

class KernelSem;

class SemaphoreList {
public:
	SemaphoreList();

	struct Elem{
		KernelSem* info;
		Elem* next;

		Elem(KernelSem* _info){
			info = _info;
			next = 0;
		}
	};

	void add(KernelSem* sem);
	void izbaci(KernelSem* sem);

	~SemaphoreList();
private:
	friend class KernelSem;
	Elem* head;
};

#endif /* SEMLIST_H_ */
