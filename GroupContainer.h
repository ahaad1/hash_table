// GroupContainer.h
#pragma once
#include "Container.h"

class GroupContainer : public Container {
protected:
    size_t length;
    size_t elem_count;

    void init_table(void **&table, size_t length) {
        table = static_cast<void **>(_memory.allocMem(length * sizeof(void *)));
        if (!table) throw Container::Error("Memory allocation failed for table.");
        for (size_t i = 0; i < length; ++i) {
            table[i] = nullptr;
        }
    }

    void free_table(void **&table, size_t length) {
        if (table) {
            _memory.freeMem(table);
            table = nullptr;
        }
    }

public:
    GroupContainer(MemoryManager &mem) : Container(mem), length(64), elem_count(0) {}

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
    
    virtual void resize_table(size_t new_length) = 0;
};
