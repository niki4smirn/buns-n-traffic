cmake_minimum_required(VERSION 3.21)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(Test
        abstract_graph.cpp
        graph.cpp
        clique.cpp
        chain.cpp
        tests/graph_tests.cpp
        tests/clique_tests.cpp
        tests/chain_tests.cpp
        )

target_link_libraries(Test gtest_main)