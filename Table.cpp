#include "Table.h"

hash_table::hash_table(MemoryManager &mem) : AbstractTable(mem), length(16), pair_cnt(0) {
    table = static_cast<STLListWrapper**>(mem.allocMem(length * sizeof(STLListWrapper*)));
    for (size_t i = 0; i < length; ++i) {
        table[i] = nullptr;
    }
}

hash_table::~hash_table() {
    clear();
    _memory.freeMem(table);
}

size_t hash_table::hash_function(void *key, size_t keySize) {
    if(key == nullptr){
        return -1;
    }
    auto hash = keySize;
    const auto *p = static_cast<const unsigned char *>(key);
    while (*p != '\0') {
        hash ^= ((hash << keySize) + hash) + (*p);
        p++;
    }

    return static_cast<int64_t>(hash) % length;
}

int hash_table::insertByKey(void *key, size_t keySize, void *elem, size_t elemSize) {
    size_t hash = hash_function(key, keySize);
    if (table[hash] == nullptr) {
        table[hash] = new STLListWrapper(_memory);
    }
    Pair p(key, keySize, elem, elemSize);
    Iterator* is_find = table[hash]->find(&p, sizeof(Pair));
    if (is_find != nullptr) {
        delete is_find;
        is_find = nullptr;
        return 1; // elem with this key already exists
    }
    table[hash]->push_front(&p, sizeof(Pair));
    ++pair_cnt;
    if (pair_cnt > length) {
        expand_table();
    }
    return 0;
}

void hash_table::removeByKey(void *key, size_t keySize) {
    size_t hash = hash_function(key, keySize);
    if (table[hash] != nullptr) {
        Iterator* iter = table[hash]->newIterator();
        while (iter->hasNext()) {
            size_t size;
            Pair* p = static_cast<Pair*>(iter->getElement(size));
            if (p != nullptr && p->getKeySize() == keySize && memcmp(p->getKey(), key, keySize) == 0) {
                table[hash]->remove(iter);
                --pair_cnt;
                delete iter;
                break;
            }
            iter->goToNext();
        }
        delete iter;
    }
    if (pair_cnt < length / 4) {
        compress_table();
    }
}

Container::Iterator* hash_table::findByKey(void *key, size_t keySize) {
    size_t hash = hash_function(key, keySize);
    if (table[hash] != nullptr) {
        Iterator* iter = table[hash]->newIterator();
        while (iter->hasNext()) {
            size_t size;
            Pair* p = static_cast<Pair*>(iter->getElement(size));
            if (p != nullptr && p->getKeySize() == keySize && memcmp(p->getKey(), key, keySize) == 0) {
                return iter;
            }
            iter->goToNext();
        }
        delete iter;
    }
    return nullptr;
}

void* hash_table::at(void *key, size_t keySize, size_t &valueSize) {
    size_t hash = hash_function(key, keySize);
    if (table[hash] != nullptr) {
        Iterator* iter = table[hash]->newIterator();
        while (iter->hasNext()) {
            size_t size;
            Pair* p = static_cast<Pair*>(iter->getElement(size));
            if (p != nullptr && p->getKeySize() == keySize && memcmp(p->getKey(), key, keySize) == 0) {
                valueSize = p->getValSize();
                delete iter;
                return p->getVal();
            }
            iter->goToNext();
        }
        delete iter;
    }
    valueSize = 0;
    return nullptr;
}

void hash_table::compress_table() {
    size_t new_length = length / 2;
    STLListWrapper** new_table = static_cast<STLListWrapper**>(_memory.allocMem(new_length * sizeof(STLListWrapper*)));
    for (size_t i = 0; i < new_length; ++i) {
        new_table[i] = nullptr;
    }

    for (size_t i = 0; i < length; ++i) {
        if (table[i] != nullptr) {
            Iterator* iter = table[i]->newIterator();
            while (iter->hasNext()) {
                size_t size;
                Pair* p = static_cast<Pair*>(iter->getElement(size));
                size_t new_hash = hash_function(p->getKey(), p->getKeySize()) % new_length;
                if (new_table[new_hash] == nullptr) {
                    new_table[new_hash] = new STLListWrapper(_memory);
                }
                new_table[new_hash]->push_front(p, sizeof(Pair));
                iter->goToNext();
            }
            delete iter;
            delete table[i];
        }
    }

    _memory.freeMem(table);
    table = new_table;
    length = new_length;
}

void hash_table::expand_table() {
    size_t new_length = length * 2;
    STLListWrapper** new_table = static_cast<STLListWrapper**>(_memory.allocMem(new_length * sizeof(STLListWrapper*)));
    for (size_t i = 0; i < new_length; ++i) {
        new_table[i] = nullptr;
    }

    for (size_t i = 0; i < length; ++i) {
        if (table[i] != nullptr) {
            Iterator* iter = table[i]->newIterator();
            while (iter->hasNext()) {
                size_t size;
                Pair* p = static_cast<Pair*>(iter->getElement(size));
                size_t new_hash = hash_function(p->getKey(), p->getKeySize()) % new_length;
                if (new_table[new_hash] == nullptr) {
                    new_table[new_hash] = new STLListWrapper(_memory);
                }
                new_table[new_hash]->push_front(p, sizeof(Pair));
                iter->goToNext();
            }
            delete iter;
            delete table[i];
        }
    }

    _memory.freeMem(table);
    table = new_table;
    length = new_length;
}

int hash_table::size() {
    return pair_cnt;
}

size_t hash_table::max_bytes() {
    return length * sizeof(STLListWrapper*);
}

Container::Iterator* hash_table::newIterator() {
    return new ht_iterator(this);
}

void hash_table::remove(Container::Iterator* iter) {
    auto* ht_iter = dynamic_cast<ht_iterator*>(iter);
    if (ht_iter != nullptr) {
        size_t size;
        void* element = ht_iter->getElement(size);
        if (element != nullptr) {
//            ht_iter->_current_table_cell_iter->remove(iter);
            delete ht_iter->_current_table_cell_iter->getElement(size);
            --pair_cnt;
            if (pair_cnt < length / 4) {
                compress_table();
            }
        }
    }
}

Container::Iterator* hash_table::find(void *elem, size_t size) {
    for (size_t i = 0; i < length; ++i) {
        if (table[i] != nullptr) {
            Iterator* iter = table[i]->newIterator();
            while (iter->hasNext()) {
                size_t elem_size;
                Pair* p = static_cast<Pair*>(iter->getElement(elem_size));
                if (p != nullptr && memcmp(p->getVal(), elem, size) == 0) {
                    return iter;
                }
                iter->goToNext();
            }
            delete iter;
        }
    }
    return nullptr;
}

void hash_table::clear() {
    for (size_t i = 0; i < length; ++i) {
        if (table[i] != nullptr) {
            table[i]->clear();
            delete table[i];
            table[i] = nullptr;
        }
    }
    pair_cnt = 0;
}

bool hash_table::empty() {
    return pair_cnt == 0;
}

/* ========== ht_iterator ========== */
hash_table::ht_iterator::ht_iterator(hash_table* ht) : _hash_table_iter(ht), _table_iter_index(0) {
    while (_table_iter_index < _hash_table_iter->length && (_hash_table_iter->table[_table_iter_index] == nullptr || _hash_table_iter->table[_table_iter_index]->empty())) {
        ++_table_iter_index;
    }
    if (_table_iter_index < _hash_table_iter->length) {
        _current_table_cell_iter = static_cast<STLListWrapper::ListIterator*>(_hash_table_iter->table[_table_iter_index]->newIterator());
    } else {
        _current_table_cell_iter = nullptr;
    }
}

void* hash_table::ht_iterator::getElement(size_t &size) {
    if (_current_table_cell_iter == nullptr) {
        size = 0;
        return nullptr;
    }
    return _current_table_cell_iter->getElement(size);
}

bool hash_table::ht_iterator::hasNext() {
    if (_current_table_cell_iter != nullptr && _current_table_cell_iter->hasNext()) {
        return true;
    }
    size_t temp_index = _table_iter_index + 1;
    while (temp_index < _hash_table_iter->length) {
        if (_hash_table_iter->table[temp_index] != nullptr && !_hash_table_iter->table[temp_index]->empty()) {
            return true;
        }
        ++temp_index;
    }
    return false;
}

size_t hash_table::ht_iterator::change_nxt_table_cell() {
    for (size_t i = _table_iter_index + 1; i < _hash_table_iter->length; ++i) {
        if (_hash_table_iter->table[i] != nullptr && !_hash_table_iter->table[i]->empty()) {
            return i;
        }
    }
    return _hash_table_iter->length;
}

void hash_table::ht_iterator::goToNext() {
    if (_current_table_cell_iter != nullptr && _current_table_cell_iter->hasNext()) {
        _current_table_cell_iter->goToNext();
    } else {
        delete _current_table_cell_iter;
        _current_table_cell_iter = nullptr;
        _table_iter_index = change_nxt_table_cell();
        if (_table_iter_index < _hash_table_iter->length) {
            _current_table_cell_iter = static_cast<STLListWrapper::ListIterator*>(_hash_table_iter->table[_table_iter_index]->newIterator());
        }
    }
}

bool hash_table::ht_iterator::equals(Container::Iterator* right) {
    auto* right_iter = static_cast<ht_iterator*>(right);
    return _table_iter_index == right_iter->_table_iter_index && _current_table_cell_iter == right_iter->_current_table_cell_iter;
}
