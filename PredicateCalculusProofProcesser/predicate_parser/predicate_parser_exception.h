//
// Created by Aleksandr Tukallo on 17.02.17.
//

#ifndef PREDICATECALCULUSPROOFPROCESSER_PREDICATE_PARSER_EXCEPTION_H
#define PREDICATECALCULUSPROOFPROCESSER_PREDICATE_PARSER_EXCEPTION_H

#include <stdexcept>

struct predicate_parser_exception : std::exception
{
    predicate_parser_exception() : std::exception()
    {};
};


#endif //PREDICATECALCULUSPROOFPROCESSER_PREDICATE_PARSER_EXCEPTION_H
