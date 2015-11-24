#ifndef _COLLIDERLINKEDLIST_
#define _COLLIDERLINKEDLIST_

#include "LinkedList.h"
#include "ColliderNode.h"

class ColliderLinkedList : public LinkedList {
private:
	ColliderNode* head;

public:
	ColliderLinkedList(void);
	~ColliderLinkedList(void);
	bool hasCollided(ColliderLinkedList* otherColliders);
	ColliderNode* getHead();
};

#endif
