cmake_minimum_required(VERSION 3.10)
project(query_completion CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(query_completion main.cpp Trie.cpp TST.cpp Tests.cpp Query.cpp Helpers.cpp SpellCorrector.cpp)
