// GroupContainer.h
#pragma once
#include "Container.h"

class GroupContainer : public Container {
protected:
    size_t length;
    size_t elem_count;

public:
    GroupContainer(MemoryManager &mem) : Container(mem), length(64), elem_count(0) {}

    size_t hash_function(void *key, size_t keySize) {
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
