cmake_minimum_required(VERSION 3.26)
project(stsql_project C)

set(CMAKE_C_STANDARD 11)

add_executable(stsql_project main.c
        src/storage/storage.c
        src/storage/storage.h
        src/parser/parser.c
        src/parser/parser.h
        src/executor/executor.c
        src/executor/executor.h)
