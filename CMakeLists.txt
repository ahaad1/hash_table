cmake_minimum_required(VERSION 3.20)
project(hash_table)

set(CMAKE_CXX_STANDARD 23)

add_executable(hash_table
        main.cpp
        Table.h
        Table.cpp
        TableAbstract.h
        Container.h
        GroupContainer.h
        Mem.h
        MemoryManager.h
        Pair.cpp
        Pair.h
        HashTableTester.h
        Set.h
        set.cpp
        GroupList.cpp
        GroupList.h
        GroupList.h
        list.cpp
        list.h
)
