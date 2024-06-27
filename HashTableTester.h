#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <random>
#include <cassert>
#include <thread>
#include <mutex>
#include "Table.h"
#include "Mem.h"
#include <sys/resource.h>
#include <memory> // для использования умных указателей

class HashTableTester {
public:
    HashTableTester(size_t memSize, size_t numOperations)
        : memory(memSize), table(memory), numOperations(numOperations) {
    }

    void runTests() {
        std::cout << "Running tests..." << std::endl;
        std::cout << "Testing custom hash table..." << std::endl;
        testCorrectness();
        std::cout << "Performance comparison between custom hash table and STL unordered_map:" << std::endl;
        comparePerformance();
        std::cout << "Testing partial removal from both tables..." << std::endl;
        testPartialRemoval();
    }

private:
    Mem memory;
    hash_table table;
    size_t numOperations;

    std::unordered_map<std::string, bool> key_saves;
    std::unordered_map<std::string, bool> value_saves;
    std::mutex tableMutex;

    void testCustomInsert() {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);

        for (size_t i = 0; i < numOperations; ++i) {
            std::string key = generateRandomString(distribution, generator);
            std::string value = generateRandomString(distribution, generator);

            auto keyCopy = std::make_unique<char[]>(key.size() + 1);
            std::copy(key.begin(), key.end(), keyCopy.get());
            keyCopy[key.size()] = '\0';

            auto valueCopy = std::make_unique<char[]>(value.size() + 1);
            std::copy(value.begin(), value.end(), valueCopy.get());
            valueCopy[value.size()] = '\0';

            table.insertByKey(reinterpret_cast<void *>(keyCopy.get()), key.size() + 1,
                              reinterpret_cast<void *>(valueCopy.get()), value.size() + 1);
        }
    }

    void testCustomFind() {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);

        for (size_t i = 0; i < numOperations; ++i) {
            std::string key = generateRandomString(distribution, generator);
            size_t valueSize;
            try {
                table.at(reinterpret_cast<void *>(const_cast<char *>(key.c_str())), key.size() + 1, valueSize);
            } catch (const Container::Error &e) {
                // Do nothing
            }
        }
    }

    void testCustomRemove() {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);
        for (size_t i = 0; i < numOperations; ++i) {
            std::string key = generateRandomString(distribution, generator);
            try {
                table.removeByKey(reinterpret_cast<void *>(const_cast<char *>(key.c_str())), key.size() + 1);
            } catch (const Container::Error &e) {
                // Do nothing
            }
        }
    }

    void testCustomClear() {
        table.clear();
    }

    void testSTLInsert(std::unordered_map<std::string, std::string> &stlTable) {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);

        for (size_t i = 0; i < numOperations; ++i) {
            std::string key = generateRandomString(distribution, generator);
            std::string value = generateRandomString(distribution, generator);
            stlTable[key] = value;
        }
    }

    void testSTLFind(std::unordered_map<std::string, std::string> &stlTable) {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);

        for (size_t i = 0; i < numOperations; ++i) {
            std::string key = generateRandomString(distribution, generator);
            auto it = stlTable.find(key);
        }
    }

    void testSTLRemove(std::unordered_map<std::string, std::string> &stlTable) {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);

        for (size_t i = 0; i < numOperations; ++i) {
            std::string key = generateRandomString(distribution, generator);
            stlTable.erase(key);
        }
    }

    void testSTLClear(std::unordered_map<std::string, std::string> &stlTable) {
        stlTable.clear();
    }

    std::string generateRandomString(std::uniform_int_distribution<int> &distribution,
                                     std::default_random_engine &generator) {
        std::string str;
        str.reserve(20);
        for (size_t i = 0; i < 20; ++i) {
            char c = static_cast<char>(distribution(generator));
            str += c;
        }
        return str;
    }

    void printMemoryUsage() {
        std::cout << "Custom hash table memory usage: " << memory.size() << " bytes" << std::endl;
    }

    void testCorrectness() {
        std::cout << "Running correctness tests..." << std::endl;

        table.clear();
        std::unordered_map<std::string, std::string> stlTable;

        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);

        for (size_t i = 0; i < numOperations; ++i) {
            std::string key = generateRandomString(distribution, generator);
            std::string value = generateRandomString(distribution, generator);

            key_saves[key] = false;
            value_saves[value] = false;

            table.insertByKey(reinterpret_cast<void *>(const_cast<char *>(key.c_str())), key.size() + 1,
                              reinterpret_cast<void *>(const_cast<char *>(value.c_str())), value.size() + 1);

            stlTable[key] = value;
        }

        for (const auto &key_itr: key_saves) {
            size_t valueSize;
            void *customValue = nullptr;
            try {
                customValue = table.at(reinterpret_cast<void *>(const_cast<char *>(key_itr.first.c_str())), key_itr.first.size() + 1, valueSize);
            } catch (const Container::Error &e) {
                // Do nothing
            }

            auto stlValue = stlTable.find(key_itr.first);

            assert(customValue != nullptr);
            assert(stlValue != stlTable.end());
            assert(strcmp(static_cast<char*>(customValue), stlValue->second.c_str()) == 0);
        }

        for (const auto &key_itr: key_saves) {
            table.removeByKey(reinterpret_cast<void *>(const_cast<char *>(key_itr.first.c_str())), key_itr.first.size() + 1);
            stlTable.erase(key_itr.first);

            size_t valueSize;
            try {
                void *customValue = table.at(reinterpret_cast<void *>(const_cast<char *>(key_itr.first.c_str())), key_itr.first.size() + 1, valueSize);
                assert(customValue == nullptr);
            } catch (const Container::Error &e) {
                // Do nothing
            }
            assert(stlTable.find(key_itr.first) == stlTable.end());
        }

        table.clear();
        stlTable.clear();
        const char *keys[] = {"key0", "key1", "key2", "key3", "key4"};
        const char *values[] = {"value0", "value1", "value2", "value3", "value4"};

        for (size_t i = 0; i < 5; ++i) {
            table.insertByKey(reinterpret_cast<void *>(const_cast<char *>(keys[i])), strlen(keys[i]) + 1,
                              reinterpret_cast<void *>(const_cast<char *>(values[i])), strlen(values[i]) + 1);
            stlTable[keys[i]] = values[i];
        }

        for (size_t i = 0; i < 5; ++i) {
            size_t valueSize;
            void *customValue = nullptr;
            try {
                customValue = table.at(reinterpret_cast<void *>(const_cast<char *>(keys[i])), strlen(keys[i]) + 1, valueSize);
            } catch (const Container::Error &e) {
                // Do nothing
            }
            auto stlValue = stlTable.find(keys[i]);

            assert(customValue != nullptr);
            assert(stlValue != stlTable.end());
            assert(strcmp(static_cast<char*>(customValue), stlValue->second.c_str()) == 0);
        }

        for (size_t i = 0; i < 5; ++i) {
            table.removeByKey(reinterpret_cast<void *>(const_cast<char *>(keys[i])), strlen(keys[i]) + 1);
            stlTable.erase(keys[i]);

            size_t valueSize;
            try {
                void *customValue = table.at(reinterpret_cast<void *>(const_cast<char *>(keys[i])), strlen(keys[i]) + 1, valueSize);
                assert(customValue == nullptr);
            } catch (const Container::Error &e) {
                // Do nothing
            }
            assert(stlTable.find(keys[i]) == stlTable.end());
        }

        std::cout << "Correctness tests passed." << std::endl;
    }

    void comparePerformance() {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);
        std::vector<std::string> keys(numOperations);
        std::vector<std::string> values(numOperations);

        for (size_t i = 0; i < numOperations; ++i) {
            keys[i] = generateRandomString(distribution, generator);
            values[i] = generateRandomString(distribution, generator);
        }

        std::unordered_map<std::string, std::string> stlTable;

        // Measure custom hash table insert time
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < numOperations; ++i) {
            table.insertByKey(reinterpret_cast<void *>(const_cast<char *>(keys[i].c_str())), keys[i].size() + 1,
                              reinterpret_cast<void *>(const_cast<char *>(values[i].c_str())), values[i].size() + 1);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> customInsertTime = end - start;

        // Measure STL unordered_map insert time
        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < numOperations; ++i) {
            stlTable[keys[i]] = values[i];
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> stlInsertTime = end - start;

        // Measure custom hash table find time
        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < numOperations; ++i) {
            size_t valueSize;
            try {
                table.at(reinterpret_cast<void *>(const_cast<char *>(keys[i].c_str())), keys[i].size() + 1, valueSize);
            } catch (const Container::Error &e) {
                // Do nothing
            }
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> customFindTime = end - start;

        // Measure STL unordered_map find time
        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < numOperations; ++i) {
            stlTable.find(keys[i]);
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> stlFindTime = end - start;

        // Measure custom hash table remove time
        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < numOperations; ++i) {
            try {
                table.removeByKey(reinterpret_cast<void *>(const_cast<char *>(keys[i].c_str())), keys[i].size() + 1);
            } catch (const Container::Error &e) {
                // Do nothing
            }
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> customRemoveTime = end - start;

        // Measure STL unordered_map remove time
        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < numOperations; ++i) {
            stlTable.erase(keys[i]);
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> stlRemoveTime = end - start;

        std::cout << "Insert time: Custom hash table = " << customInsertTime.count() << "s, STL unordered_map = " << stlInsertTime.count() << "s\n";
        std::cout << "Find time: Custom hash table = " << customFindTime.count() << "s, STL unordered_map = " << stlFindTime.count() << "s\n";
        std::cout << "Remove time: Custom hash table = " << customRemoveTime.count() << "s, STL unordered_map = " << stlRemoveTime.count() << "s\n";
    }

    void testPartialRemoval() {
        std::cout << "Testing partial removal..." << std::endl;

        // Insert elements into both tables
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 255);
        std::vector<std::string> keys(numOperations);
        std::vector<std::string> values(numOperations);

        for (size_t i = 0; i < numOperations; ++i) {
            keys[i] = generateRandomString(distribution, generator);
            values[i] = generateRandomString(distribution, generator);
            table.insertByKey(reinterpret_cast<void *>(const_cast<char *>(keys[i].c_str())), keys[i].size() + 1,
                              reinterpret_cast<void *>(const_cast<char *>(values[i].c_str())), values[i].size() + 1);
        }

        std::unordered_map<std::string, std::string> stlTable;
        for (size_t i = 0; i < numOperations; ++i) {
            stlTable[keys[i]] = values[i];
        }

        // Remove half of the elements from both tables
        for (size_t i = 0; i < numOperations / 2; ++i) {
            table.removeByKey(reinterpret_cast<void *>(const_cast<char *>(keys[i].c_str())), keys[i].size() + 1);
            stlTable.erase(keys[i]);
        }

        // Verify that the removed elements are no longer in the tables
        for (size_t i = 0; i < numOperations / 2; ++i) {
            size_t valueSize;
            try {
                void *customValue = table.at(reinterpret_cast<void *>(const_cast<char *>(keys[i].c_str())), keys[i].size() + 1, valueSize);
                assert(customValue == nullptr);
            } catch (const Container::Error &e) {
                // Expected behavior
            }
            assert(stlTable.find(keys[i]) == stlTable.end());
        }

        // Verify that the remaining elements are still in the tables
        for (size_t i = numOperations / 2; i < numOperations; ++i) {
            size_t valueSize;
            void *customValue = nullptr;
            try {
                customValue = table.at(reinterpret_cast<void *>(const_cast<char *>(keys[i].c_str())), keys[i].size() + 1, valueSize);
            } catch (const Container::Error &e) {
                // Do nothing
            }
            auto stlValue = stlTable.find(keys[i]);

            assert(customValue != nullptr);
            assert(stlValue != stlTable.end());
            assert(strcmp(static_cast<char*>(customValue), stlValue->second.c_str()) == 0);
        }

        std::cout << "Partial removal tests passed." << std::endl;
    }
};
