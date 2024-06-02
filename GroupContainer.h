#pragma once
#include "Container.h"

// Базовый класс для некоторой группы абстрактных контейнеров
class GroupContainer: public Container
{
public:
    GroupContainer(MemoryManager &mem): Container(mem) {}

    /* function generating hash value for any obj
    returns hash value or NULL */ 
    static int64_t get_hash(void *key, size_t keySize);
};

//rolling hash ?
inline int64_t GroupContainer::get_hash(void *key, size_t keySize){
    if(key == nullptr){
        return -1;
    }
    auto hash = keySize;
    const auto *p = static_cast<const unsigned char *>(key);
    while (*p != '\0') {
        hash ^= ((hash << keySize) + hash) + (*p);
        p++;
    }

    return static_cast<int64_t>(hash);
}
