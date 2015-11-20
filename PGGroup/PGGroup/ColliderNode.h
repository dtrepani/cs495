#ifndef _COLLIDERNODE_
#define _COLLIDERNODE_

#include "Node.h"
#include "ColliderEntity.h"

class ColliderNode : public Node {
private:
	ColliderEntity* entity;
	ColliderNode* next;

	bool hasCollidedWhileIteratingThroughOtherList(ColliderNode* otherCollider);
	float distanceBetweenColliders(ColliderNode* otherCollider);
	int sumOfRadii(ColliderNode* otherCollider);

public:
	ColliderNode(ColliderEntity* anEntity, ColliderNode* aNext);
	~ColliderNode(void);
	bool hasCollidedWhileIteratingThroughThisList(ColliderNode* otherColliderHead);
	ColliderNode* getNext();
	ColliderEntity* getEntity();
};

#endif
