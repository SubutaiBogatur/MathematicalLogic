//
// Created by aleksandr on 19.10.16.
//

#ifndef PROOFCHECKER_AST_H
#define PROOFCHECKER_AST_H

#include "parser.h"

struct ast
{
public:
    std::shared_ptr<parser::node> root;

    //TODO
    //uint32_t hash_of_the_tree; //??

    ast(std::string const& expression);
//    ast(std::shared_ptr<parser::node> rt):root(rt ){}

private:
    //TODO
    //build_hash()?
};


#endif //PROOFCHECKER_AST_H
