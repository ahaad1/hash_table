#pragma once
#include "Container.h"

class GroupList : public Container
{

protected:
	struct Node {
		void* val;
		size_t valSize = 0;
		Node* next;
		Node* prev;
	};

	Node* newNode(void * elem, size_t elemSize);
	void deleteNode(Node* tmp);



public:
	GroupList(MemoryManager &mem) : Container(mem) {}

};