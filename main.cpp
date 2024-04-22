#pragma once
#include "Table.h"
#include "List.h"
#include "Mem.h"
#include "Pair.h"

#include <string>
int main(){
    MemoryManager *mem = new Mem(1024);
    List *list = new List(*mem);
    Pair *pair = new Pair(new int(1), sizeof(int), new string("hello"), sizeof("hello"));
    list->push_front(pair, sizeof(Pair));
    list->clear();
    delete list;
    delete mem;
    return 0;
}