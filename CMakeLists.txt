project(query_completion)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wno-narrowing")

set(SOURCE_FILES main.cpp)
add_executable(query_completion ${SOURCE_FILES} Trie.h Trie.cpp TST.h TST.cpp Tests.h Tests.cpp Query.cpp Query.h Helpers.h Helpers.cpp SpellCorrector.cpp SpellCorrector.h)