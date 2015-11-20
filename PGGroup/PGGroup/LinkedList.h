#ifndef _LINKEDLIST_
#define _LINKEDLIST_

#include "Node.h"

class Node;
class LinkedList {
private:
	Node *head;
	// player entity reference?

public:
	LinkedList(void);
	~LinkedList(void);
	void add(Entity *anEntity);
	bool hasCollided(LinkedList* otherColliders);

	Node* getHead();
};

#endif
