#pragma once
#include "ListAbstract.h"



class List :public AbstractList {

private:
	Node* head;
	int count;

public:

	class Iterator :public Container::Iterator {//
	public:

		Iterator(Node* p) { this->my_pos = p; }
		~Iterator();
		void setPosition(Node* new_pos) {
			my_pos = new_pos;
		}
		void* getElement(size_t &size);
		bool hasNext();
		void goToNext();
		bool equals(Container::Iterator *right);
		friend List;

	private:
		Node* my_pos;
		Node* prev_my_pos;


	};//

	List(MemoryManager &mem); //: AbstractList(mem) {}
	~List();
	int push_front(void *elem, size_t elemSize);
	void pop_front();
	void* front(size_t &size);
	int insert(Container::Iterator *iter, void *elem, size_t elemSize);
	Iterator begin() { return head; }

	int size();
	size_t max_bytes();
	Iterator* find(void *elem, size_t size);
	Iterator* newIterator();
	void remove(Container::Iterator *iter);
	void clear();
	bool empty();

};
