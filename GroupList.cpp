#include "GroupList.h"

GroupList::Node* GroupList::newNode(void * elem, size_t elemSize)
{
	Node* new_node = (Node*)_memory.allocMem(sizeof(Node));
	new_node->val = new char[elemSize];
	memcpy(new_node->val, elem, elemSize);
	new_node->valSize = elemSize;
	return new_node;
}

void GroupList::deleteNode(Node * tmp)
{
	_memory.freeMem(tmp->val);
	_memory.freeMem(tmp);
}