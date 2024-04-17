#include "Table.h"
#include "Mem.h"
#include <string>
int main(){
    MemoryManager *mem = new Mem(1024);
    ht_list *list = new ht_list(*mem);
    ht_pair *pair = new ht_pair(new int(1), sizeof(int), new string("hello"), sizeof("hello"));
    list->push_front(pair, sizeof(ht_pair));
    list->clear();
    delete list;
    delete mem;
    return 0;
}