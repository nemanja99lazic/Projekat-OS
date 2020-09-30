#ifndef LIST_H_
#define LIST_H_

class PCB;
extern void init();
extern void killThread();

class List {
public:
	List();
	struct Elem{
		PCB* info;
		Elem* next;

		Elem(PCB* _info){
			info = _info;
			next = 0;
		}
	};

	void add(PCB* pcb);
	void izbaci(PCB* pcb);

	virtual ~List();
private:
	friend class PCB;
	friend void init();
	friend void killThread();
	Elem* head;
};
#endif /* LIST_H_ */
