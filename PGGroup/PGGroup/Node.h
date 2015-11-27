#ifndef _NODE_
#define _NODE_

class Entity;
class Node {
private:
	Entity* entity;
	Node* next;

public:
	Node(Entity* anEntity, Node* aNext);
	~Node(void);
	virtual Node* getNext();
	virtual Entity* getEntity();
};

#endif
