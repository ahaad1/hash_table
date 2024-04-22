#include "Pair.h"

/*--- ht_pair realization ---*/
void * Pair::getKey() const {return this->key;}
void * Pair::getVal() const {return this->val;}
void Pair::setKey(void *key, size_t keySize) {this->key = key; this->keySize = keySize;}
void Pair::setVal(void *val, size_t valSize) {this->val = val; this->valSize = valSize;}
size_t Pair::getKeySize() const {return this->keySize;}
size_t Pair::getValSize() const {return this->valSize;}
void Pair::free_key() {
    delete this->key;
    this->key = nullptr;
}
void Pair::free_val() {
    delete this->val;
    this->val = nullptr;
}
Pair::Pair(void *key, size_t keySize, void *val, size_t valSize) {
    this->key = key;
    this->keySize = keySize;
    this->val = val;
    this->valSize = valSize;
}
Pair::~Pair() {
    this->free_key();
    this->free_val();
}
/*--- ====================== ---*/
