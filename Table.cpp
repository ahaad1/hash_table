// table.cpp
#include "Table.h"

hash_table::hash_table(MemoryManager &mem) : AbstractTable(mem) {
    length = 64;
    table = static_cast<Pair **>(mem.allocMem(length * sizeof(Pair *)));
    if (!table) throw Container::Error("Memory allocation failed for hash table.");
    for (size_t i = 0; i < length; ++i) table[i] = nullptr;
    elem_count = 0;
}

hash_table::~hash_table() {
    clear();
    _memory.freeMem(table);
}

size_t hash_table::hash_function(void *key, size_t keySize) {
    return GroupContainer::hash_function(key, keySize);
}


int hash_table::insertByKey(void *key, size_t keySize, void *elem, size_t elemSize) {
    size_t hash = hash_function(key, keySize) % length;
    Pair *newPair = new Pair(key, keySize, elem, elemSize);
    if (!newPair) throw Container::Error("Memory allocation failed for Pair.");

    Pair *current = table[hash];
    Pair *prev = nullptr;
    while (current != nullptr) {
        if (memcmp(current->getKey(), key, keySize) == 0) {
            delete newPair;
            return 1;
        }
        prev = current;
        current = current->next;
    }

    newPair->next = table[hash];
    table[hash] = newPair;
    ++elem_count;

    if (elem_count > length) resize_table(length * 2);

    return 0;
}

void hash_table::removeByKey(void *key, size_t keySize) {
    size_t hash = hash_function(key, keySize) % length;
    Pair *current = table[hash];
    Pair *prev = nullptr;

    while (current != nullptr) {
        if (memcmp(current->getKey(), key, keySize) == 0) {
            if (prev == nullptr) {
                table[hash] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            --elem_count;

            if (elem_count < length / 4 && length > 64) resize_table(length / 2);

            return;
        }
        prev = current;
        current = current->next;
    }
}

Container::Iterator *hash_table::findByKey(void *key, size_t keySize) {
    size_t hash = hash_function(key, keySize) % length;
    Pair *current = table[hash];

    while (current != nullptr) {
        if (memcmp(current->getKey(), key, keySize) == 0) {
            return new ht_iterator(this, current);
        }
        current = current->next;
    }
    return nullptr;
}

void *hash_table::at(void *key, size_t keySize, size_t &valueSize) {
    size_t hash = hash_function(key, keySize) % length;
    Pair *current = table[hash];

    while (current != nullptr) {
        if (memcmp(current->getKey(), key, keySize) == 0) {
            valueSize = current->getValSize();
            return current->getVal();
        }
        current = current->next;
    }
    valueSize = 0;
    throw Container::Error("Key not found.");
}

void hash_table::resize_table(size_t new_length) {
    Pair **new_table = static_cast<Pair **>(_memory.allocMem(new_length * sizeof(Pair *)));
    if (!new_table) throw Container::Error("Memory allocation failed during table resizing.");

    for (size_t i = 0; i < new_length; ++i) new_table[i] = nullptr;

    for (size_t i = 0; i < length; ++i) {
        Pair *current = table[i];
        while (current != nullptr) {
            Pair *next = current->next;
            size_t new_hash = hash_function(current->getKey(), current->getKeySize()) % new_length;
            current->next = new_table[new_hash];
            new_table[new_hash] = current;
            current = next;
        }
    }

    _memory.freeMem(table);
    table = new_table;
    length = new_length;
}

int hash_table::size() {
    return static_cast<int>(elem_count);
}

size_t hash_table::max_bytes() {
    return length * sizeof(Pair *);
}

Container::Iterator *hash_table::newIterator() {
    for (size_t i = 0; i < length; ++i) {
        if (table[i] != nullptr) {
            return new ht_iterator(this, table[i]);
        }
    }
    return nullptr;
}

void hash_table::remove(Container::Iterator *iter) {
    auto *ht_iter = dynamic_cast<ht_iterator *>(iter);
    if (ht_iter != nullptr) {
        size_t size;
        void *element = ht_iter->getElement(size);
        if (element != nullptr) {
            removeByKey(static_cast<Pair *>(element)->getKey(), static_cast<Pair *>(element)->getKeySize());
        }
    }
}

Container::Iterator *hash_table::find(void *elem, size_t size) {
    for (size_t i = 0; i < length; ++i) {
        Pair *current = table[i];
        while (current != nullptr) {
            if (memcmp(current->getVal(), elem, size) == 0) {
                return new ht_iterator(this, current);
            }
            current = current->next;
        }
    }
    return nullptr;
}

void hash_table::clear() {
    for (size_t i = 0; i < length; ++i) {
        Pair *current = table[i];
        while (current != nullptr) {
            Pair *next = current->next;
            delete current;
            current = next;
        }
        table[i] = nullptr;
    }
    elem_count = 0;
}

bool hash_table::empty() {
    return elem_count == 0;
}

/* ========== ht_iterator ========== */
hash_table::ht_iterator::ht_iterator(hash_table *ht, Pair *current) : _hash_table_iter(ht), _current(current), _bucket_index(0) {
    moveToNextValid();
}

void *hash_table::ht_iterator::getElement(size_t &size) {
    if (_current == nullptr) {
        size = 0;
        return nullptr;
    }
    size = sizeof(Pair);
    return _current;
}

bool hash_table::ht_iterator::hasNext() {
    return _current != nullptr && _current->next != nullptr;
}

void hash_table::ht_iterator::goToNext() {
    if (_current != nullptr) {
        _current = _current->next;
    }
    moveToNextValid();
}

bool hash_table::ht_iterator::equals(Container::Iterator *right) {
    auto *right_iter = dynamic_cast<ht_iterator *>(right);
    return _current == right_iter->_current;
}

void hash_table::ht_iterator::moveToNextValid() {
    while (_current == nullptr && _bucket_index < _hash_table_iter->length) {
        _current = _hash_table_iter->table[_bucket_index];
        _bucket_index++;
    }
}
