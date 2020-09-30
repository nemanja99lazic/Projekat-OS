#ifndef SIGHLIST_H_
#define SIGHLIST_H_

typedef void (*SignalHandler)();

class PCB;

class SignalHandlerList {
public:
	SignalHandlerList();
	SignalHandlerList(const SignalHandlerList &orig); // konstruktor kopije

	virtual ~SignalHandlerList();
	void add(SignalHandler handler);
	void swap(SignalHandler hand1, SignalHandler hand2);

	struct Elem{
			SignalHandler handler;
			Elem* next;

			Elem(SignalHandler _handler)
			{
				handler = _handler;
				next = 0;
			}
		};
private:
	friend class PCB;
	Elem* head, *tail;
};

#endif /* SIGHLIST_H_ */
