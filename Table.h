#include <bits/stdc++.h>
#include "TableAbstract.h"

using namespace std;

class hash_table;
class ht_iterator;
class ht_list;

class hash_table : public AbstractTable
{
private:
    ht_list **table{};
    size_t length{};

public:
    ht_iterator *iterator{};
    explicit hash_table(MemoryManager &mem);
    ~hash_table();

    int insertByKey(void *key, size_t keySize, void *elem, size_t elemSize) override;
    void removeByKey(void *key, size_t keySize) override;
    Iterator* findByKey(void *key, size_t keySize) override;
    void* at(void *key, size_t keySize, size_t &valueSize) override;
    size_t hash_function(void *key, size_t keySize) override;

    size_t max_bytes() override;

    friend class ht_iterator;
};

// friend class Table;
class ht_iterator : public Container::Iterator
{
private:
    hash_table * _hash_table;
    size_t cur_iter_index;
public:
    void* getElement(size_t &size) override;
    bool hasNext() override;
    void goToNext() override;
    bool equals(Iterator *right) override;

    friend class hash_table;
};

class ht_list{
private:
    class node {
    private:
        void *key, *val;
        size_t keySize, valSize;
    public:
        explicit node(void *key, size_t keySize, void *val, size_t valSize): key(key), keySize(keySize), val(val), valSize(valSize), next(nullptr) {}
        ~node();
        size_t getKeySize() const;
        size_t getValSize() const;
        void setKey(void *key, size_t keySize);
        void setVal(void *val, size_t valSize);
        void *getKey() const;
        void *getVal() const;
        void free_key();
        void free_val();

        node *next;
    };
    node *head;
public:
    ht_list();
    ~ht_list();
    int push(void *key, size_t keySize, void *elem, size_t elemSize);
    //ht_list* find(void *key, size_t keySize);
    int pop(void *key, size_t keySize);
};
