#pragma once
#include "TableAbstract.h"
#include "Pair.h"

class hash_table : public AbstractTable {
private:
    class ht_iterator : public Container::Iterator {
    private:
        hash_table *_hash_table_iter;
        Pair *_current;
        size_t _bucket_index;

        void moveToNextValid();

    public:
        explicit ht_iterator(hash_table *ht, Pair *current);

        void *getElement(size_t &size) override;

        bool hasNext() override;

        void goToNext() override;

        bool equals(Iterator *right) override;

        friend class hash_table;
    };

    Pair **table1;
    Pair **table2;
    size_t length;
    size_t pair_cnt;

    size_t hash_function1(void *key, size_t keySize);
    size_t hash_function2(void *key, size_t keySize);
    void rehash();

public:
    explicit hash_table(MemoryManager &mem);

    ~hash_table() override;

    size_t hash_function(void *key, size_t keySize) override;

    int insertByKey(void *key, size_t keySize, void *elem, size_t elemSize) override;

    void removeByKey(void *key, size_t keySize) override;

    Iterator *findByKey(void *key, size_t keySize) override;

    void *at(void *key, size_t keySize, size_t &valueSize) override;

    int size() override;

    size_t max_bytes() override;

    Iterator *find(void *elem, size_t size) override;

    void remove(Iterator *iter) override;

    void clear() override;

    bool empty() override;

    Iterator *newIterator() override;
};
