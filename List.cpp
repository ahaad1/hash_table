#include "List.h"

/* linked list realization */
List::List(MemoryManager &mem) : AbstractList(mem), lst() {}
Pair* List::find_p(void *elem, size_t size) {
    for(auto x : this->lst){
        if(static_cast<Pair*>(x)->getKeySize() == size &&
           memcmp(static_cast<Pair*>(x)->getKey(), static_cast<Pair*>(elem)->getKey(), size) == 0){
            return static_cast<Pair*>(x);
        }
    }
    return nullptr;
}
void List::pop(void* elem){
    this->lst.remove(static_cast<Pair*>(elem)->getKey());
}
int List::push_front(void *elem, size_t elemSize) {
    if (elem == nullptr) { return 0; }
    this->lst.push_front(elem);
    return 0;
}
void List::pop_front() {
    this->lst.pop_front();
}
void* List::front(size_t &size) {
    if (this->lst.empty()) { return nullptr; }
    size = sizeof(this->lst.front());
    return this->lst.front();
}
int List::insert(Iterator *iter, void *elem, size_t elemSize) {
    if (iter->hasNext()) {
        this->lst.insert(this->lst.begin(), elem);
        return 0;
    }
    else {
        this->lst.push_back(elem);
        return 0;
    }
    return 1;
}
int List::size(){
    return static_cast<int>(this->lst.size());
}
void List::remove(Iterator *iter) {
    list_iterator* listIter = dynamic_cast<list_iterator*>(iter);
    if (listIter == nullptr || listIter->getIter() == lst.end()) {
        return;
    }
    listIter->setIter(lst.erase(listIter->getIter()));
}
void List::clear() {
    for(auto l : lst){
        static_cast<Pair*>(l)->free_val();
        static_cast<Pair*>(l)->free_key();
        delete l;
    }
    lst.clear();
}
bool List::empty() {
    return lst.empty();
}
size_t List::max_bytes(){
    return sizeof(this->lst);
}
Container::Iterator* List::find(void *elem, size_t size) {
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        if (memcmp(static_cast<Pair*>(*it)->getKey(), elem, size) == 0) {
            return (Container::Iterator*)(new list_iterator(it, lst.end()));
        }
    }
    return nullptr;
}
Container::Iterator* List::newIterator() {
    return  (Container::Iterator*)(new list_iterator(lst.begin(), lst.end()));
}

/*--- list's iterator realization ---*/
void *list_iterator::getElement(size_t &size){
    if (iter == end) return nullptr;
    size = 0;
    return *iter;
}
bool list_iterator::hasNext() {
    auto temp = iter;
    return ++temp != end;
}
void list_iterator::goToNext() {
    if (iter != end) ++iter;
}
bool list_iterator::equals(Iterator *right) {
    list_iterator* rhs = dynamic_cast<list_iterator*>(right);
    if (rhs == nullptr) return false;
    return iter == rhs->iter;
}
/*--- ====================== ---*/
