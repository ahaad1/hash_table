#include <bits/stdc++.h>
#include "TableAbstract.h"
#include "ListAbstract.h"
#include <list>
#include <algorithm>

class hash_table;
class ht_iterator;
class ht_list;
class ht_list_iterator;

class hash_table : public AbstractTable
{

private:

    ht_list **table{};
    size_t length{};
    size_t pair_cnt{};

public:
    ht_iterator *iterator{};
    explicit hash_table(MemoryManager &mem);
    ~hash_table();

    int insertByKey(void *key, size_t keySize, void *elem, size_t elemSize) override;
    void removeByKey(void *key, size_t keySize) override;
    Iterator* findByKey(void *key, size_t keySize) override;
    void* at(void *key, size_t keySize, size_t &valueSize) override;
    size_t hash_function(void *key, size_t keySize) override;
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

class ht_pair {
private:
    void *key, *val;
    size_t keySize, valSize;
public:
    ht_pair(void *key, size_t keySize, void *val, size_t valSize);
    ~ht_pair();
    size_t getKeySize() const;
    size_t getValSize() const;
    void setKey(void *key, size_t keySize);
    void setVal(void *val, size_t valSize);
    void *getKey() const;
    void *getVal() const;
    void free_key();
    void free_val();
};

//abstract list
class ht_list_iterator : Container::Iterator{
private:
    std::list<void*>::iterator iter;
    std::list<void*>::iterator end;
public:
    ht_list_iterator(std::list<void*>::iterator it, std::list<void*>::iterator endIt)
            : iter(it), end(endIt) {}
    void* getElement(size_t &size) override;
    bool hasNext() override;
    void goToNext() override;
    bool equals(Iterator *right) override;

    std::list<void*>::iterator getIter() const {return iter;}
    void setIter(std::list<void*>::iterator it) {iter = it;}
};
class ht_list : AbstractList{
private:
    std::list<void*> lst;
public:
    explicit ht_list(MemoryManager &mem);
    int push_front(void *elem, size_t elemSize) override;
    void pop_front() override;
    void* front(size_t &size) override;
    int insert(Iterator *iter, void *elem, size_t elemSize) override;

    int size() override;
    size_t max_bytes() override;
    Iterator* find(void *elem, size_t size) override;
    Iterator* newIterator() override;
    void remove(Iterator *iter) override;
    void clear() override;
    bool empty() override;

    void pop(void* elem);
    ht_pair* find_p(void *key, size_t keySize);

    friend class ht_list_iterator;
};
