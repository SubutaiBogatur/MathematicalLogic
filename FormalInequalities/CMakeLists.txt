cmake_minimum_required(VERSION 3.6)
project(FormalInequalities)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp prover/prover.h prover/prover.cpp)
add_executable(FormalInequalities ${SOURCE_FILES})