#include "Table.h"
/* hash_table realisation */
hash_table::hash_table(MemoryManager &mem): AbstractTable(mem) {
    this->length = 5;
    this->pair_cnt = 0;
    this->table = (ht_list**)mem.allocMem(sizeof(ht_list*) * this->length);
}
hash_table::~hash_table() {cout << "hash table destruct\n\n";}
size_t hash_table::hash_function(void *key, size_t keySize) { return GroupContainer::get_hash(key, keySize) % this->length;}
int hash_table::insertByKey(void *key, size_t keySize, void *elem, size_t elemSize) {return 0;}
void hash_table::removeByKey(void *key, size_t keySize){}
AbstractTable::Iterator* hash_table::findByKey(void *key, size_t keySize){return nullptr;}
void* hash_table::at(void *key, size_t keySize, size_t &valueSize){ return nullptr; }
/*--- ====================== ---*/

/* hash_table iterator realization */
void *ht_iterator::getElement(size_t &size) {
    if(this->_hash_table->table == nullptr || this->cur_iter_index > this->_hash_table->length){
        size = 0;
        return nullptr;
    }
    else{
        size = sizeof(this->_hash_table->table[this->cur_iter_index]);
        return this->_hash_table->table[this->cur_iter_index];
    }
}
bool ht_iterator::hasNext() {
    return cur_iter_index <= this->_hash_table->length - 1;
}
void ht_iterator::goToNext() {
    this->cur_iter_index++;
    this->_hash_table->table++;
}
bool ht_iterator::equals(Iterator *right) {
    return this->_hash_table == dynamic_cast<ht_iterator*>(right)->_hash_table;
}
/*--- ====================== ---*/

/*--- list's iterator realization ---*/
void *ht_list_iterator::getElement(size_t &size){
    if (iter == end) return nullptr;
    size = 0;
    return *iter;
}
bool ht_list_iterator::hasNext() {
    auto temp = iter;
    return ++temp != end;
}
void ht_list_iterator::goToNext() {
    if (iter != end) ++iter;
}
bool ht_list_iterator::equals(Iterator *right) {
    ht_list_iterator* rhs = dynamic_cast<ht_list_iterator*>(right);
    if (rhs == nullptr) return false;
    return iter == rhs->iter;
}
/*--- ====================== ---*/

/*--- ht_pair realization ---*/
void * ht_pair::getKey() const {return this->key;}
void * ht_pair::getVal() const {return this->val;}
void ht_pair::setKey(void *key, size_t keySize) {this->key = key; this->keySize = keySize;}
void ht_pair::setVal(void *val, size_t valSize) {this->val = val; this->valSize = valSize;}
size_t ht_pair::getKeySize() const {return this->keySize;}
size_t ht_pair::getValSize() const {return this->valSize;}
void ht_pair::free_key() {
    delete this->key;
    this->key = nullptr;
}
void ht_pair::free_val() {
    delete this->val;
    this->val = nullptr;
}
ht_pair::ht_pair(void *key, size_t keySize, void *val, size_t valSize) {
    this->key = key;
    this->keySize = keySize;
    this->val = val;
    this->valSize = valSize;
}
ht_pair::~ht_pair() {
    this->free_key();
    this->free_val();
}
/*--- ====================== ---*/

/* linked list realization */
ht_list::ht_list(MemoryManager &mem) : AbstractList(mem), lst() {}
ht_pair* ht_list::find_p(void *elem, size_t size) {
    for(auto x : this->lst){
        if(static_cast<ht_pair*>(x)->getKeySize() == size &&
           memcmp(static_cast<ht_pair*>(x)->getKey(), static_cast<ht_pair*>(elem)->getKey(), size) == 0){
            return static_cast<ht_pair*>(x);
        }
    }
    return nullptr;
}
void ht_list::pop(void* elem){
    this->lst.remove(static_cast<ht_pair*>(elem)->getKey());
}
int ht_list::push_front(void *elem, size_t elemSize) {
    if (elem == nullptr) { return 0; }
    this->lst.push_front(elem);
    return 0;
}
void ht_list::pop_front() {
    this->lst.pop_front();
}
void* ht_list::front(size_t &size) {
    if (this->lst.empty()) { return nullptr; }
    size = sizeof(this->lst.front());
    return this->lst.front();
}
int ht_list::insert(Iterator *iter, void *elem, size_t elemSize) {
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
int ht_list::size(){
    return static_cast<int>(this->lst.size());
}
void ht_list::remove(Iterator *iter) {
    ht_list_iterator* listIter = dynamic_cast<ht_list_iterator*>(iter);
    if (listIter == nullptr || listIter->getIter() == lst.end()) {
        return;
    }
    listIter->setIter(lst.erase(listIter->getIter()));
}
void ht_list::clear() {
    for(auto l : lst){
        static_cast<ht_pair*>(l)->free_val();
        static_cast<ht_pair*>(l)->free_key();
        delete l;
    }
    lst.clear();
}
bool ht_list::empty() {
    return lst.empty();
}
size_t ht_list::max_bytes(){
    return sizeof(this->lst);
}
Container::Iterator* ht_list::find(void *elem, size_t size) {
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        if (memcmp(static_cast<ht_pair*>(*it)->getKey(), elem, size) == 0) {
            return (Container::Iterator*)(new ht_list_iterator(it, lst.end()));
        }
    }
    return nullptr;
}
Container::Iterator* ht_list::newIterator() {
    return  (Container::Iterator*)(new ht_list_iterator(lst.begin(), lst.end()));
}
