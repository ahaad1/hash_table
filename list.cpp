#include "list.h"
#include "GroupList.h"


List::List(MemoryManager &mem) : AbstractList(mem) {
	this->head = nullptr;
}

List::~List() {
	clear();
}

int List::push_front(void * elem, size_t elemSize)
{
	if (elem == NULL || elemSize <= 0)
		return 1;
	Node* new_head = newNode(elem, elemSize);
	/*Node* new_head = (Node*)_memory.allocMem(sizeof(Node));
	new_head->val = new char[elemSize];//
	memcpy(new_head->val, elem, elemSize);
	new_head->valSize = elemSize;*/
	new_head->next = head;
	head = new_head;
	count++;
	return 0;
}

void List::pop_front()
{
	if (!this->empty()) {
		Node* tmp = head;
		if (count != 1) {
			head = head->next;
		}
		deleteNode(tmp);
		/*_memory.freeMem(tmp->val);
		_memory.freeMem(tmp);*/

		count--;

	}
	else { throw Container::Error("Empty list"); }

}

void * List::front(size_t & size)
{
	size = head->valSize;
	if (head == NULL) {
		return NULL;
	}
	return head->val;
}

int List::insert(Container::Iterator * iter, void * elem, size_t elemSize)
{
	//преведение типа
	List::Iterator* iterator = (List::Iterator*)iter;
	Node* tmp = head;
	Node* prev = NULL;

	while (tmp != NULL) {
		if (tmp == iterator->my_pos->next) {
			prev = tmp;
			tmp = prev->next;
			break;
		}
		tmp = tmp->next;
	}
	Node* new_node = newNode(elem, elemSize);
	/*Node* new_node = (Node*)_memory.allocMem(sizeof(Node));//
	new_node->val = new char[elemSize];
	memcpy(new_node->val, elem, elemSize);
	new_node->valSize = elemSize;//оформить как в функцию и перенести в grouplist
	*/
	new_node->next = tmp;
	prev->next = new_node;
	count++;

	return 0;
}


bool List::empty()
{
	if (count == 0) {
		return true;
	}
	return false;
}

void List::clear()
{
	while (count) {
		pop_front();
	}
}

int List::size()
{
	return count;
}

size_t List::max_bytes()
{
	return size_t();
}

List::Iterator* List::find(void * elem, size_t size)
{
	List::Iterator* newIter = newIterator();
	Node* tmp = head;
	int find = 0;

	while (tmp != NULL) {
		if (memcmp(tmp->val, elem, size) == 0) {	
			newIter->setPosition(tmp);
			find = 1;
			break;
		}
		tmp = tmp->next;

	}
	if (find == 0) {
		newIter = NULL;
	}
	return newIter;
}

List::Iterator* List::newIterator()
{
	if (this->empty()) {
		List::Iterator * newIter = new List::Iterator(nullptr);
		return newIter;
	}
	List::Iterator * newIter = new List::Iterator(head);

	return newIter;

}

void List::remove(Container::Iterator * iter)
{
	List::Iterator* thisIter = dynamic_cast<List::Iterator*>(iter);

	if (thisIter->my_pos == head) {
		pop_front();
		thisIter->my_pos = head;
	}
	else {
		thisIter->prev_my_pos->next = thisIter->my_pos->next;
		thisIter->my_pos->next = NULL;
		_memory.freeMem(thisIter->my_pos->val);
		_memory.freeMem(thisIter->my_pos);
		count--;
		thisIter->my_pos = thisIter->prev_my_pos->next;
	}
}





List::Iterator::~Iterator()
{
}

void * List::Iterator::getElement(size_t & size)
{
	if (my_pos == NULL) {
		size = 0;
		return NULL;
	}
	else {
		size = my_pos->valSize;
		return my_pos->val;
	}
}

bool List::Iterator::hasNext()
{
	if (my_pos != NULL && my_pos->next == NULL) {
		return false;
	}
	return true;
}

void List::Iterator::goToNext()
{
	if (my_pos->next != NULL) {
		prev_my_pos = my_pos;
		my_pos = my_pos->next;
	}
}

bool List::Iterator::equals(Container::Iterator * right)
{
	//приведение типов
	List::Iterator* iterator = (List::Iterator*)right;
	if (iterator->my_pos == this->my_pos) {
		return true;
	}
	return false;
}
