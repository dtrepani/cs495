#ifndef _NODE_
#define _NODE_

#include "Entity.h"
#include "ColliderEntity.h"

class Node {
private:
	Entity* entity;
	Node* next;

public:
	Node(Entity* anEntity, Node* aNext);
	~Node(void);
	bool hasCollided(Node* otherCollider);

	Entity* getEntity();
};

#endif
