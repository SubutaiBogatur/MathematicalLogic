cmake_minimum_required(VERSION 3.5.1)
project(PredicateCalculusProofProcesser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O0 -Wall -pedantic")

set(SOURCE_FILES main.cpp predicate_parser/parser.cpp predicate_parser/predicate_ast.cpp predicate_parser/predicate_proof_processor.cpp predicate_parser/predicate_proof_processor.h predicate_parser/axioms.cpp predicate_parser/axioms.h)
add_executable(PredicateCalculusProofProcesser ${SOURCE_FILES})