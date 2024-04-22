#include "Table.h"

/* hash_table realisation */
hash_table::hash_table(MemoryManager &mem): AbstractTable(mem) {
    this->length = 5;
    this->pair_cnt = 0;
    this->table = (List**)mem.allocMem(sizeof(List*) * this->length);
}
hash_table::~hash_table() {cout << "hash table destruct\n\n";}
size_t hash_table::hash_function(void *key, size_t keySize) { return GroupContainer::get_hash(key, keySize) % this->length;}
int hash_table::insertByKey(void *key, size_t keySize, void *elem, size_t elemSize) { return 0;}
void hash_table::removeByKey(void *key, size_t keySize){}
AbstractTable::Iterator* hash_table::findByKey(void *key, size_t keySize){return nullptr;}
void* hash_table::at(void *key, size_t keySize, size_t &valueSize){ return nullptr; }

int hash_table::size(){
    return (int)this->pair_cnt;
}
size_t hash_table::max_bytes(){
    return sizeof(this->table);
}
Container::Iterator* hash_table::find(void *elem, size_t size) {
    return this->table[this->hash_function(elem, size)]->find(elem, size);
}
Container::Iterator* hash_table::newIterator() {
    this->iterator = new hash_table::ht_iterator();
    return (Container::Iterator*)this->iterator;
}
void hash_table::remove(Iterator *iter) {
    size_t buf_size = -1;;
    this->table[this->hash_function((hash_table::ht_iterator*)iter->getElement(buf_size), buf_size)]->remove(iter);
}
void hash_table::clear() {
    for(int i = 0; i < this->length; i++){
        this->table[i]->clear();
    }
    this->pair_cnt = 0;
}
bool hash_table::empty() {
    return this->pair_cnt == 0;
}
/*--- ====================== ---*/

/* hash_table iterator realization */
void *hash_table::ht_iterator::getElement(size_t &size) {
    if(this->_hash_table->table == nullptr || this->cur_iter_index > this->_hash_table->length){
        size = 0;
        return nullptr;
    }
    else{
        size = sizeof(this->_hash_table->table[this->cur_iter_index]);
        return this->_hash_table->table[this->cur_iter_index];
    }
}
bool hash_table::ht_iterator::hasNext() {
    return cur_iter_index <= this->_hash_table->length - 1;
}
void hash_table::ht_iterator::goToNext() {
    this->cur_iter_index++;
    this->_hash_table->table++;
}
bool hash_table::ht_iterator::equals(Iterator *right) {
    return this->_hash_table == dynamic_cast<ht_iterator*>(right)->_hash_table;
}
/*--- ====================== ---*/