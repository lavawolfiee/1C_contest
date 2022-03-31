cmake_minimum_required(VERSION 3.17)
project(1C_contest)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Ofast)

add_executable(1C_contest main.cpp SuffixAutomaton.cpp NodesContainer.cpp)
add_executable(Tests tests.cpp SuffixAutomaton.cpp NodesContainer.cpp)