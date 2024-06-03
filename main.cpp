#include <iostream>
#include "Table.h"
#include "Mem.h"
#include "HashTableTester.h"

int main() {
    size_t memSize = 1024 * 1024 * 1024; // 1 GB
//    size_t numOperations = 1000000;
    size_t numOperations = 10;
    HashTableTester tester(memSize, numOperations);
    tester.runTests();

    return 0;
}
