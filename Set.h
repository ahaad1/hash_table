// Set.h
#pragma once
#include "SetAbstract.h"
#include "list.h"

class Set : public AbstractSet {
private:
    List **set_data;

public:
    Set(MemoryManager &mem);

    ~Set();

    class SetIterator : public Container::Iterator {
    private:
        int index_in_array;
        Set* set;
        List::Iterator* list_iterator;
        size_t get_index_in_array();
        SetIterator(Iterator* iterator, Set* s, int i);
        SetIterator(Set* s);

    public:
        void* getElement(size_t &size);

        bool hasNext();

        void goToNext();

        bool equals(Iterator *right);

        ~SetIterator();

        friend class Set;
    };

    int size();

    size_t max_bytes();

    Iterator* find(void *elem, size_t size);

    Iterator* newIterator();

    void remove(Iterator *iter);

    void clear();

    bool empty();

    int insert(void *elem, size_t size);

    void rehash_function(size_t new_length) override;
};
