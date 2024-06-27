#include "Table.h"
#include <cstdlib>
#include <cstring>
#include <ctime>

hash_table::hash_table(MemoryManager &mem) : AbstractTable(mem), length(64), pair_cnt(0) {
    table1 = static_cast<Pair **>(mem.allocMem(length * sizeof(Pair *)));
    table2 = static_cast<Pair **>(mem.allocMem(length * sizeof(Pair *)));
    if (!table1 || !table2) throw Container::Error("Memory allocation failed for hash table.");
    for (size_t i = 0; i < length; ++i) {
        table1[i] = nullptr;
        table2[i] = nullptr;
    }
    srand(static_cast<unsigned int>(time(nullptr)));  // Для случайного выбора таблицы при вставке
}

hash_table::~hash_table() {
    clear();
    _memory.freeMem(table1);
    _memory.freeMem(table2);
}

size_t hash_table::hash_function1(void *key, size_t keySize) {
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

size_t hash_table::hash_function2(void *key, size_t keySize) {
    const size_t prime = 31;
    size_t hash = 0;
    const unsigned char *data = static_cast<const unsigned char *>(key);

    for (size_t i = 0; i < keySize; ++i) {
        hash = data[i] + (hash * prime);
    }
    return hash;
}

size_t hash_table::hash_function(void *key, size_t keySize) {
    return hash_function1(key, keySize);
}

void hash_table::rehash() {
    size_t new_length = length * 2;
    Pair **new_table1 = static_cast<Pair **>(_memory.allocMem(new_length * sizeof(Pair *)));
    Pair **new_table2 = static_cast<Pair **>(_memory.allocMem(new_length * sizeof(Pair *)));
    if (!new_table1 || !new_table2) throw Container::Error("Memory allocation failed during table resizing.");

    for (size_t i = 0; i < new_length; ++i) {
        new_table1[i] = nullptr;
        new_table2[i] = nullptr;
    }

    for (size_t i = 0; i < length; ++i) {
        if (table1[i]) {
            size_t new_hash = hash_function1(table1[i]->getKey(), table1[i]->getKeySize()) % new_length;
            new_table1[new_hash] = table1[i];
        }
        if (table2[i]) {
            size_t new_hash = hash_function2(table2[i]->getKey(), table2[i]->getKeySize()) % new_length;
            new_table2[new_hash] = table2[i];
        }
    }

    _memory.freeMem(table1);
    _memory.freeMem(table2);
    table1 = new_table1;
    table2 = new_table2;
    length = new_length;
}

int hash_table::insertByKey(void *key, size_t keySize, void *elem, size_t elemSize) {
    if (pair_cnt > length * 0.9) {
        rehash();
    }

    Pair *newPair = new Pair(key, keySize, elem, elemSize);
    if (!newPair) throw Container::Error("Memory allocation failed for Pair.");

    size_t hash1 = hash_function1(key, keySize) % length;
    size_t hash2 = hash_function2(key, keySize) % length;

    for (int attempts = 0; attempts < length; ++attempts) {
        if (table1[hash1] == nullptr) {
            table1[hash1] = newPair;
            ++pair_cnt;
            return 0;
        } else if (table2[hash2] == nullptr) {
            table2[hash2] = newPair;
            ++pair_cnt;
            return 0;
        } else {
            if (rand() % 2) {
                std::swap(newPair, table1[hash1]);
                hash1 = hash_function1(newPair->getKey(), newPair->getKeySize()) % length;
            } else {
                std::swap(newPair, table2[hash2]);
                hash2 = hash_function2(newPair->getKey(), newPair->getKeySize()) % length;
            }
        }
    }

    delete newPair;
    throw Container::Error("Rehashing failed, maximum attempts reached.");
}

void hash_table::removeByKey(void *key, size_t keySize) {
    size_t hash1 = hash_function1(key, keySize) % length;
    size_t hash2 = hash_function2(key, keySize) % length;

    if (table1[hash1] && memcmp(table1[hash1]->getKey(), key, keySize) == 0) {
        delete table1[hash1];
        table1[hash1] = nullptr;
        --pair_cnt;
    } else if (table2[hash2] && memcmp(table2[hash2]->getKey(), key, keySize) == 0) {
        delete table2[hash2];
        table2[hash2] = nullptr;
        --pair_cnt;
    } else {
        throw Container::Error("Key not found.");
    }

    if (pair_cnt < length / 4 && length > 64) {
        rehash();
    }
}

Container::Iterator *hash_table::findByKey(void *key, size_t keySize) {
    size_t hash1 = hash_function1(key, keySize) % length;
    size_t hash2 = hash_function2(key, keySize) % length;

    if (table1[hash1] && memcmp(table1[hash1]->getKey(), key, keySize) == 0) {
        return new ht_iterator(this, table1[hash1]);
    }
    if (table2[hash2] && memcmp(table2[hash2]->getKey(), key, keySize) == 0) {
        return new ht_iterator(this, table2[hash2]);
    }
    return nullptr;
}

void *hash_table::at(void *key, size_t keySize, size_t &valueSize) {
    size_t hash1 = hash_function1(key, keySize) % length;
    size_t hash2 = hash_function2(key, keySize) % length;

    if (table1[hash1] && memcmp(table1[hash1]->getKey(), key, keySize) == 0) {
        valueSize = table1[hash1]->getValSize();
        return table1[hash1]->getVal();
    }
    if (table2[hash2] && memcmp(table2[hash2]->getKey(), key, keySize) == 0) {
        valueSize = table2[hash2]->getValSize();
        return table2[hash2]->getVal();
    }
    valueSize = 0;
    throw Container::Error("Key not found.");
}

void hash_table::clear() {
    for (size_t i = 0; i < length; ++i) {
        if (table1[i]) {
            delete table1[i];
            table1[i] = nullptr;
        }
        if (table2[i]) {
            delete table2[i];
            table2[i] = nullptr;
        }
    }
    pair_cnt = 0;
}

bool hash_table::empty() {
    return pair_cnt == 0;
}

Container::Iterator *hash_table::find(void *elem, size_t size) {
    for (size_t i = 0; i < length; ++i) {
        if (table1[i] && memcmp(table1[i]->getVal(), elem, size) == 0) {
            return new ht_iterator(this, table1[i]);
        }
        if (table2[i] && memcmp(table2[i]->getVal(), elem, size) == 0) {
            return new ht_iterator(this, table2[i]);
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

Container::Iterator *hash_table::newIterator() {
    for (size_t i = 0; i < length; ++i) {
        if (table1[i] != nullptr) {
            return new ht_iterator(this, table1[i]);
        }
        if (table2[i] != nullptr) {
            return new ht_iterator(this, table2[i]);
        }
    }
    return nullptr;
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
        _current = _hash_table_iter->table1[_bucket_index];
        if (_current == nullptr) {
            _current = _hash_table_iter->table2[_bucket_index];
        }
        _bucket_index++;
    }
}
