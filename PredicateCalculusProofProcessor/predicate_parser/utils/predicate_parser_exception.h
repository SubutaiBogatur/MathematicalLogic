//
// Created by Aleksandr Tukallo on 17.02.17.
//

#ifndef PREDICATECALCULUSPROOFPROCESSER_PREDICATE_PARSER_EXCEPTION_H
#define PREDICATECALCULUSPROOFPROCESSER_PREDICATE_PARSER_EXCEPTION_H

#include <stdexcept>

struct predicate_parser_exception : std::exception
{
    std::string msg;
    predicate_parser_exception() : std::exception()
    {};
    predicate_parser_exception(std::string&& str) : msg(str)
    {}
    predicate_parser_exception(std::string& str) : msg(str)
    {}

    virtual const char* what() const noexcept
    {
        return msg.c_str();
    }
};


#endif //PREDICATECALCULUSPROOFPROCESSER_PREDICATE_PARSER_EXCEPTION_H
