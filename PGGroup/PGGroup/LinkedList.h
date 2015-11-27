#ifndef _LINKEDLIST_
#define _LINKEDLIST_

#include <stdio.h>
#include "Node.h"

class Entity;
class LinkedList {
private:
	Node *head;

public:
	LinkedList(void);
	~LinkedList(void);
	virtual void add(Entity *anEntity);
	virtual Node* getHead();
};

#endif
