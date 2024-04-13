cmake_minimum_required(VERSION 3.28)
project(hash_table)

set(CMAKE_CXX_STANDARD 11)

add_executable(hash_table
        main.cpp
        Table.h
        Table.cpp
        TableAbstract.h
        Container.cpp
        Container.h
        GroupContainer.h
        GroupList.h
        Mem.h
        MemoryManager.h
        main.cpp
        Table.cpp
)
