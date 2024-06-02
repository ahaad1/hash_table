#include <iostream>
#include "Table.h"
#include "Mem.h"

void printResult(const char* testName, bool result) {
    std::cout << testName << ": " << (result ? "PASSED" : "FAILED") << std::endl;
}

int main() {
    Mem mem(1024);
    hash_table table(mem);

    const char* key1 = "key1";
    const char* value1 = "value1";
    size_t valueSize1 = strlen(value1) + 1;

    const char* key2 = "key2";
    const char* value2 = "value2";
    size_t valueSize2 = strlen(value2) + 1;

    // Тест на вставку элемента
    bool insertTest1 = (table.insertByKey((void*)key1, strlen(key1) + 1, (void*)value1, valueSize1) == 0);
    printResult("Insert Test 1", insertTest1);

    // Тест на получение элемента по ключу
    size_t valueSizeRetrieved;
    void* valueRetrieved = table.at((void*)key1, strlen(key1) + 1, valueSizeRetrieved);
    bool atTest1 = (valueRetrieved != nullptr && strcmp((char*)valueRetrieved, value1) == 0);
    printResult("At Test 1", atTest1);

    // Тест на повторную вставку того же ключа
    bool insertTest2 = (table.insertByKey((void*)key1, strlen(key1) + 1, (void*)value1, valueSize1) == 1);
    printResult("Insert Test 2", insertTest2);

    // Тест на вставку второго элемента
    bool insertTest3 = (table.insertByKey((void*)key2, strlen(key2) + 1, (void*)value2, valueSize2) == 0);
    printResult("Insert Test 3", insertTest3);

    // Тест на удаление элемента по ключу
    table.removeByKey((void*)key1, strlen(key1) + 1);
    valueRetrieved = table.at((void*)key1, strlen(key1) + 1, valueSizeRetrieved);
    bool removeTest1 = (valueRetrieved == nullptr);
    printResult("Remove Test 1", removeTest1);

    // Тест на поиск элемента по ключу
    Container::Iterator* foundIter = table.findByKey((void*)key2, strlen(key2) + 1);
    bool findByKeyTest1 = (foundIter != nullptr);
    printResult("Find By Key Test 1", findByKeyTest1);

    // Тест на получение размера таблицы
    bool sizeTest1 = (table.size() == 1);
    printResult("Size Test 1", sizeTest1);

    // Тест на очистку таблицы
    table.clear();
    bool clearTest1 = (table.size() == 0 && table.empty());
    printResult("Clear Test 1", clearTest1);

    // Тест на вставку и удаление через итератор
    table.insertByKey((void*)key1, strlen(key1) + 1, (void*)value1, valueSize1);
    Container::Iterator* iter = table.findByKey((void*)key1, strlen(key1) + 1);
    bool insertAndFindTest1 = (iter != nullptr);
    printResult("Insert and Find Test 1", insertAndFindTest1);

    if (iter != nullptr) {
        table.remove(iter);
        valueRetrieved = table.at((void*)key1, strlen(key1) + 1, valueSizeRetrieved);
        bool removeIteratorTest1 = (valueRetrieved == nullptr);
        printResult("Remove Iterator Test 1", removeIteratorTest1);
        delete iter;
    }

    return 0;
}
