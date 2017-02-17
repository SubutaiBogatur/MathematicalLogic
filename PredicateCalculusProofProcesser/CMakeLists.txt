cmake_minimum_required(VERSION 3.6)
project(PredicateCalculusProofProcesser)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp predicate_parser/parser.cpp predicate_parser/predicate_ast.cpp)
add_executable(PredicateCalculusProofProcesser ${SOURCE_FILES})