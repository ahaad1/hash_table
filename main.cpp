#include "Table.h"
#include <string>
int main(){
    ht_list *list = new ht_list();
    list->push(new int(1), sizeof(int), new string("hello"), sizeof("hello"));
    list->push(new int(5), sizeof(int), new string("qwe"), sizeof("qwe"));
    list->push(new int(5), sizeof(int), new string("asdaasd"), sizeof("asdaasd"));
    list->pop(new int(5), sizeof(int));
    return 0;
}