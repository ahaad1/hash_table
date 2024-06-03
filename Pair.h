#pragma once
#include "MemoryManager.h"

class Pair {
private:
    void *key, *val;
    size_t keySize, valSize;
public:
    Pair *next;

    Pair(void *key, size_t keySize, void *val, size_t valSize);
    ~Pair();
    size_t getKeySize() const;
    size_t getValSize() const;
    void setKey(void *key, size_t keySize);
    void setVal(void *val, size_t valSize);
    void *getKey() const;
    void *getVal() const;
    void free_key(MemoryManager &mem);
    void free_val(MemoryManager &mem);
};
