#include <iostream>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <random>
#include "Table.h"
#include "Mem.h"

class HashTableTester {
public:
    HashTableTester(size_t memSize, size_t numOperations)
            : memory(memSize), table(memory), numOperations(numOperations) {}

    void runTests() {
        std::cout << "Running tests..." << std::endl;

        std::cout << "Testing custom hash table..." << std::endl;
        runCustomHashTableTests();

        std::cout << "Testing STL unordered_map..." << std::endl;
        runSTLHashTableTests();
    }

private:
    Mem memory;
    hash_table table;
    size_t numOperations;

    void runCustomHashTableTests() {
        auto start = std::chrono::high_resolution_clock::now();
        testCustomInsert();
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Insert Test completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        testCustomFind();
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Find Test completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        testCustomRemove();
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Remove Test completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        testCustomClear();
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Clear Test completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        printMemoryUsage();
    }

    void runSTLHashTableTests() {
        std::unordered_map<std::string, std::string> stlTable;

        auto start = std::chrono::high_resolution_clock::now();
        testSTLInsert(stlTable);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Insert Test completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        testSTLFind(stlTable);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Find Test completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        testSTLRemove(stlTable);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Remove Test completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        testSTLClear(stlTable);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Clear Test completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
    }

    void testCustomInsert() {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);

        for (size_t i = 0; i < numOperations; ++i) {
            std::string key = generateRandomString(distribution, generator);
            std::string value = generateRandomString(distribution, generator);
            table.insertByKey((void*)key.c_str(), key.size() + 1, (void*)value.c_str(), value.size() + 1);
        }
    }

    void testCustomFind() {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);

        for (size_t i = 0; i < numOperations; ++i) {
            std::string key = generateRandomString(distribution, generator);
            size_t valueSize;
            table.at((void*)key.c_str(), key.size() + 1, valueSize);
        }
    }

    void testCustomRemove() {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);

        for (size_t i = 0; i < numOperations; ++i) {
            std::string key = generateRandomString(distribution, generator);
            table.removeByKey((void*)key.c_str(), key.size() + 1);
        }
    }

    void testCustomClear() {
        table.clear();
    }

    void testSTLInsert(std::unordered_map<std::string, std::string>& stlTable) {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);

        for (size_t i = 0; i < numOperations; ++i) {
            std::string key = generateRandomString(distribution, generator);
            std::string value = generateRandomString(distribution, generator);
            stlTable[key] = value;
        }
    }

    void testSTLFind(std::unordered_map<std::string, std::string>& stlTable) {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);

        for (size_t i = 0; i < numOperations; ++i) {
            std::string key = generateRandomString(distribution, generator);
            auto it = stlTable.find(key);
        }
    }

    void testSTLRemove(std::unordered_map<std::string, std::string>& stlTable) {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);

        for (size_t i = 0; i < numOperations; ++i) {
            std::string key = generateRandomString(distribution, generator);
            stlTable.erase(key);
        }
    }

    void testSTLClear(std::unordered_map<std::string, std::string>& stlTable) {
        stlTable.clear();
    }

    std::string generateRandomString(std::uniform_int_distribution<int>& distribution, std::default_random_engine& generator) {
        std::string str;
        for (size_t i = 0; i < 20; ++i) { // Use longer keys and values for more intensive tests
            char c = static_cast<char>(distribution(generator));
            str += c;
        }
        return str;
    }

    void printMemoryUsage() {
        // Placeholder for actual memory usage reporting, as getting exact memory usage is platform dependent
        std::cout << "Custom hash table memory usage: " << memory.size() << " bytes" << std::endl;
    }
};
