#include <iostream>
#include "HashTableTester.h"

int main() {

    size_t numOperations = 1000000;
    size_t memSize = 1024 * 1024 * 1024;
    HashTableTester tester(memSize, numOperations);

    std::cout << "Running extensive tests..." << std::endl;
    tester.runTests();

    return 0;
}