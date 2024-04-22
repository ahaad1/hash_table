#pragma once
#include <bits/stdc++.h>
#include <list>
#include "ListAbstract.h"
#include "Pair.h"

class List;
class list_iterator;

class List : AbstractList {
private:
    std::list<void*> lst;
public:
    explicit List(MemoryManager &mem);
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
    Pair* find_p(void *key, size_t keySize);

    friend class list_iterator;
};

//abstract list
class list_iterator : Container::Iterator{
private:
    std::list<void*>::iterator iter;
    std::list<void*>::iterator end;
public:
    list_iterator(std::list<void*>::iterator it, std::list<void*>::iterator endIt)
            : iter(it), end(endIt) {}
    void* getElement(size_t &size) override;
    bool hasNext() override;
    void goToNext() override;
    bool equals(Iterator *right) override;

    std::list<void*>::iterator getIter() const {return iter;}
    void setIter(std::list<void*>::iterator it) {iter = it;}
};
