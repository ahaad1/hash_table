#pragma once

#include "ListAbstract.h"
#include "Pair.h"
#include <list>

class STLListWrapper : public AbstractList {
    friend class hash_table;
    friend class ht_iterator;
private:
    std::list<Pair*> list;

public:
    STLListWrapper(MemoryManager &mem) : AbstractList(mem) {}

    friend class hash_table;

    virtual ~STLListWrapper() {
        clear();
    }

    int push_front(void *elem, size_t elemSize) override {
        if (elemSize != sizeof(Pair)) {
            return 1;
        }
        list.push_front(new Pair(*(static_cast<Pair*>(elem))));
        return 0;
    }

    void pop_front() override {
        if (!list.empty()) {
            delete list.front();
            list.pop_front();
        }
    }

    void* front(size_t &size) override {
        if (list.empty()) {
            size = 0;
            return nullptr;
        }
        size = sizeof(Pair);
        return list.front();
    }

    int insert(Iterator *iter, void *elem, size_t elemSize) override {
        if (elemSize != sizeof(Pair)) {
            return 1;
        }
        list.insert(dynamic_cast<ListIterator*>(iter)->getIterator(), new Pair(*(static_cast<Pair*>(elem))));
        return 0;
    }

    int size() override {
        return static_cast<int>(list.size());
    }

    size_t max_bytes() override {
        return list.max_size() * sizeof(Pair*);
    }

    Iterator* find(void *elem, size_t size) override {
        if (size != sizeof(Pair)) {
            return nullptr;
        }
        Pair *pair = static_cast<Pair*>(elem);
        auto it = std::find_if(list.begin(), list.end(), [pair](Pair* p) {
            return (p->getKeySize() == pair->getKeySize() &&
                    memcmp(p->getKey(), pair->getKey(), p->getKeySize()) == 0 &&
                    p->getValSize() == pair->getValSize() &&
                    memcmp(p->getVal(), pair->getVal(), p->getValSize()) == 0);
        });

        if (it != list.end()) {
            return new ListIterator(it, list.end());
        }
        return nullptr;
    }

    Iterator* newIterator() override {
        if (list.empty()) {
            return nullptr;
        }
        return new ListIterator(list.begin(), list.end());
    }

    void remove(Iterator *iter) override {
        auto it = dynamic_cast<ListIterator*>(iter)->getIterator();
        if (it != list.end()) {
            delete *it;
            list.erase(it);
        }
    }

    void clear() override {
        for (auto pair : list) {
            delete pair;
        }
        list.clear();
    }

    bool empty() override {
        return list.empty();
    }

private:
    class ListIterator : public Iterator {
        friend class hash_table;
        friend class ht_iterator;

    private:
        std::list<Pair*>::iterator current;
        std::list<Pair*>::iterator end;

    public:
        ListIterator(std::list<Pair*>::iterator start, std::list<Pair*>::iterator end)
                : current(start), end(end) {}

        void* getElement(size_t &size) override {
            if (current == end) {
                size = 0;
                return nullptr;
            }
            size = sizeof(Pair);
            return *current;
        }

        bool hasNext() override {
            auto temp = current;
            return ++temp != end;
        }

        void goToNext() override {
            if (current != end) {
                ++current;
            }
        }

        bool equals(Iterator *right) override {
            auto rightIter = dynamic_cast<ListIterator*>(right);
            return current == rightIter->current;
        }

        std::list<Pair*>::iterator getIterator() {
            return current;
        }
    };
};
