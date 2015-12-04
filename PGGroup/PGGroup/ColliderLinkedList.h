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
	void add(ColliderEntity* head);
	ColliderNode* getHead();
	bool hasCollided(ColliderLinkedList* otherColliders);
	bool withinPlaneBoundaries(Orientation orientation, float biggestPositionVal1, float smallestPositionVal1, float biggestPositionVal2, float smallestPositionVal2);
};

#endif
