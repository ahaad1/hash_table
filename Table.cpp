#include "Table.h"

/* linked list realization */

/*--- list's node realization ---*/
ht_list::ht_list() : head(nullptr) {}
ht_list::~ht_list() { cout << "list destruct\n\n"; }
void * ht_list::node::getKey() const {return this->key;}
void * ht_list::node::getVal() const {return this->val;}
void ht_list::node::setKey(void *key, size_t keySize) {this->key = key; this->keySize = keySize;}
void ht_list::node::setVal(void *val, size_t valSize) {this->val = val; this->valSize = valSize;}
size_t ht_list::node::getKeySize() const {return this->keySize;}
size_t ht_list::node::getValSize() const {return this->valSize;}
/*--- ====================== ---*/

/*--- linked list methods ---*/
int ht_list::push(void *key, size_t keySize, void *elem, size_t elemSize) {
    for (node * node_iter = head; node_iter != nullptr; node_iter = node_iter->next) {
        if (node_iter->getKeySize() == keySize && memcmp(node_iter->getKey(), key, keySize) == 0) {
            // if key already exists in list - replacing value
            node_iter->setVal(elem, elemSize);
            return 0;
        }
    }
    node *newNode = new node(key, keySize, elem, elemSize);
    newNode->next = head; 
    head = newNode;
    return 0;
}
//???
int ht_list::pop(void *key, size_t keySize) {
    node *prev = nullptr;
    for (node *node_iter = head; node_iter != nullptr; prev = node_iter, node_iter = node_iter->next) {
        if (node_iter->getKeySize() == keySize && memcmp(node_iter->getKey(), key, keySize) == 0) {
            prev == nullptr ? head = node_iter->next : prev->next = node_iter->next;
            delete node_iter;
            return 0;
        }
    }
    return -1;
}

/* hash_table realisation */
hash_table::hash_table(MemoryManager &mem): AbstractTable(mem) {cout << "hash table construct\n\n";}
hash_table::~hash_table() {cout << "hash table destruct\n\n";}
size_t hash_table::hash_function(void *key, size_t keySize) { return GroupContainer::get_hash(key, keySize) % this->length;}
int hash_table::insertByKey(void *key, size_t keySize, void *elem, size_t elemSize) {return 0;}
void hash_table::removeByKey(void *key, size_t keySize){}
AbstractTable::Iterator* hash_table::findByKey(void *key, size_t keySize){return nullptr;}
void* hash_table::at(void *key, size_t keySize, size_t &valueSize){ return nullptr; }

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

