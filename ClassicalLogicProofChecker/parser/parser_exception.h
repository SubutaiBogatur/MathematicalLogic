//
// Created by Aleksandr Tukallo on 11.02.17.
//

#include <string>

#ifndef PROOFCHECKER_PARSER_EXCEPTION_H_H

struct parser_exception
{
    std::string msg;

    parser_exception(std::string message) : msg(message)
    { }
};


#define PROOFCHECKER_PARSER_EXCEPTION_H_H

#endif //PROOFCHECKER_PARSER_EXCEPTION_H_H
