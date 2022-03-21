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

find_package(benchmark REQUIRED)

set(build_files
        src/Graphs/AbstractGraph/abstract_graph.cpp
        src/Graphs/Graph/graph.cpp
        src/Graphs/Clique/clique.cpp
        src/Graphs/Chain/chain.cpp

        src/TrafficManager/traffic_manager.cpp
        )

add_executable(Test
        ${build_files}
        tests/graph_tests.cpp
        tests/clique_tests.cpp
        tests/chain_tests.cpp

        tests/traffic_manager_tests.cpp
        )

add_executable(Benchmark
        ${build_files}
        benchmarks/benchmark.cpp
        )

target_link_libraries(Test gtest_main)

target_link_libraries(Benchmark benchmark::benchmark)
