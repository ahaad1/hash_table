// Set.cpp
#include "Set.h"

Set::Set(MemoryManager &mem) : AbstractSet(mem) {
    length = 10000;
    init_table(reinterpret_cast<void**&>(set_data), length);
}

Set::SetIterator::SetIterator(Iterator* iterator, Set* s, int i) : set(s), list_iterator(dynamic_cast<List::Iterator*>(iterator)) {}

Set::SetIterator::SetIterator(Set* s) : set(s) {
    for (size_t i = 0; i < s->length; ++i) {
        if (s->set_data[i] != nullptr && s->set_data[i]->size() > 0) {
            list_iterator = dynamic_cast<List::Iterator*>(s->set_data[i]->newIterator());
            return;
        }
    }
}

size_t Set::SetIterator::get_index_in_array() {
    size_t elem_size;
    void* elem = list_iterator->getElement(elem_size);
    return set->hash_function(elem, elem_size) % set->length;
}

void* Set::SetIterator::getElement(size_t &size) {
    if (list_iterator) {
        return list_iterator->getElement(size);
    } else {
        return nullptr;
    }
}

bool Set::SetIterator::hasNext() {
    if (!list_iterator) return false;

    if (list_iterator->hasNext()) return true;

    for (size_t i = get_index_in_array() + 1; i < set->length; ++i) {
        if (set->set_data[i] != nullptr) return true;
    }
    return false;
}

void Set::SetIterator::goToNext() {
    if (!list_iterator->hasNext()) {
        for (size_t i = get_index_in_array() + 1; i < set->length; ++i) {
            if (set->set_data[i] != nullptr && set->set_data[i]->size() > 0) {
                delete list_iterator;
                list_iterator = dynamic_cast<List::Iterator*>(set->set_data[i]->newIterator());
                return;
            }
        }
    } else {
        list_iterator->goToNext();
    }
}

bool Set::SetIterator::equals(Iterator *right) {
    return list_iterator->equals(dynamic_cast<List::Iterator*>(right));
}

Set::SetIterator::~SetIterator() {
    delete list_iterator;
}

int Set::size() {
    return elem_count;
}

size_t Set::max_bytes() {
    return length * sizeof(List*);
}

Set::Iterator* Set::find(void *elem, size_t size) {
    size_t hash = hash_function(elem, size) % length;

    if (!set_data[hash] || set_data[hash]->empty()) return nullptr;

    Iterator* list_iterator = set_data[hash]->find(elem, size);

    if (!list_iterator) return nullptr;

    return new SetIterator(list_iterator, this, hash);
}

Set::Iterator* Set::newIterator() {
    return new SetIterator(this);
}

void Set::remove(Iterator *iter) {
    Set::SetIterator* casted_iter = dynamic_cast<Set::SetIterator*>(iter);
    size_t size;
    void* elem = casted_iter->getElement(size);

    Set::SetIterator* set_iter = dynamic_cast<Set::SetIterator*>(this->find(elem, size));
    if (!set_iter) return;

    if (!set_iter->list_iterator->hasNext()) iter->goToNext();

    this->set_data[set_iter->get_index_in_array()]->remove(set_iter->list_iterator);
    elem_count--;
    delete set_iter;
}

void Set::clear() {
    free_table(reinterpret_cast<void**&>(set_data), length);
    elem_count = 0;
}

bool Set::empty() {
    return elem_count == 0;
}

Set::~Set() {
    clear();
    _memory.freeMem(set_data);
}

void Set::rehash_function(size_t new_length) {
    List** new_set_data = static_cast<List**>(_memory.allocMem(sizeof(List*) * new_length));
    if (!new_set_data) throw Container::Error("Memory allocation failed during set resizing.");

    for (size_t i = 0; i < new_length; ++i) new_set_data[i] = nullptr;

    for (size_t i = 0; i < length; ++i) {
        if (!set_data[i]) continue;

        List::Iterator* list_iterator = dynamic_cast<List::Iterator*>(set_data[i]->newIterator());
        while (list_iterator->hasNext()) {
            size_t elem_size;
            void* elem = list_iterator->getElement(elem_size);
            size_t hash = hash_function(elem, elem_size) % new_length;
            if (!new_set_data[hash]) new_set_data[hash] = new List(_memory);
            new_set_data[hash]->push_front(elem, elem_size);
            list_iterator->goToNext();
        }
        delete list_iterator;
        delete set_data[i];
    }

    _memory.freeMem(set_data);
    set_data = new_set_data;
    length = new_length;
}

int Set::insert(void *elem, size_t size) {
    size_t hash = hash_function(elem, size) % length;
    if (!set_data[hash]) {
        set_data[hash] = new List(_memory);
        set_data[hash]->push_front(elem, size);
        elem_count++;
        return 0;
    }

    Iterator* list_iterator = set_data[hash]->find(elem, size);
    if (list_iterator) {
        delete list_iterator;
        return 1;
    }

    if (set_data[hash]->size() == 50) {
        rehash_function(length * 2);
        hash = hash_function(elem, size) % length;
    }

    set_data[hash]->push_front(elem, size);
    elem_count++;
    return 0;
}
