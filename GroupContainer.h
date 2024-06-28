// GroupContainer.h
#pragma once
#include "Container.h"
#include "Pair.h"

class GroupContainer : public Container {
protected:
    size_t container_size_;
    size_t length;
    size_t elem_count;

    void init_table(void **&_container, size_t length) {
        _container = static_cast<void **>(_memory.allocMem(length * sizeof(void *)));
        this->container_size_ = length * sizeof(void*);
        if (!_container) throw Container::Error("Memory allocation failed for table.");
        for (size_t i = 0; i < length; ++i) {
            _container[i] = nullptr;
        }
    }

    void free_table(void **&_container, size_t length) {
        if (_container) {
            _memory.freeMem(_container);
            _container = nullptr;
        }
    }

public:
    GroupContainer(MemoryManager &mem) : Container(mem), length(16), elem_count(0) {
        // container_size_ = sizeof(Pair*)*length;
        // for(size_t i = 0; i < length; ++i) {
        //
        // }
    }

    size_t hash_function(void *key, size_t keySize) {
        if(!key || !keySize) {
            throw Container::Error("bad key or keySize value for hash function");
        }

        const size_t FNV_prime = 16777619;
        const size_t offset_basis = 2166136261;
        const unsigned char *data = static_cast<const unsigned char *>(key);
        size_t hash = offset_basis;

        for (size_t i = 0; i < keySize; ++i) {
            hash ^= data[i];
            hash *= FNV_prime;
        }
        return hash;
    }
    int size() {
        return elem_count;
    }
    size_t max_bytes() {
        return container_size_;
    }
    bool empty() {
       if(elem_count == 0) return true;
        return false;
    };
    virtual void rehash_function(size_t new_length) = 0;
};
