#pragma once
#include <bits/stdc++.h>
#include "List.h"
#include "TableAbstract.h"
#include "Pair.h"

class hash_table;

class hash_table : public AbstractTable
{
private:
    class ht_iterator : public Container::Iterator
    {
    private:
        hash_table * _hash_table{};
        size_t cur_iter_index{};
    public:
        void* getElement(size_t &size) override;
        bool hasNext() override;
        void goToNext() override;
        bool equals(Iterator *right) override;

        friend class hash_table;
    };

    List **table{};
    size_t length{};
    size_t pair_cnt{};

    void compress_table();
    void expand_table();
    void cpex_event(); // compression - extension table event

public:
    explicit hash_table(MemoryManager &mem);
    ~hash_table() override;

    hash_table::ht_iterator* iterator{};

    int insertByKey(void *key, size_t keySize, void *elem, size_t elemSize) override;
    void removeByKey(void *key, size_t keySize) override;
    Iterator* findByKey(void *key, size_t keySize) override;
    void* at(void *key, size_t keySize, size_t &valueSize) override;
    size_t hash_function(void *key, size_t keySize) override;

    int size() override;
    size_t max_bytes() override;
    Iterator* find(void *elem, size_t size) override;
    Iterator* newIterator() override;
    void remove(Iterator *iter) override;
    void clear() override;
    bool empty() override;
};
