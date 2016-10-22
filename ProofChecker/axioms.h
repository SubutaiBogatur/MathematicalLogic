//
// Created by aleksandr on 19.10.16.
//

#ifndef PROOFCHECKER_AXIOMS_H
#define PROOFCHECKER_AXIOMS_H

#include <vector>
#include "ast.h"

struct axioms
{
public:
    static std::vector<ast> axiom_ast;
    axioms();
};


#endif //PROOFCHECKER_AXIOMS_H
