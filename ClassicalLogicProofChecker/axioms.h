//
// Created by Aleksandr Tukallo on 19.10.16.
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

static const std::vector<std::string> axiom_names
        {"PHI -> (PSI -> PHI)",
         "(PHI -> PSI) -> (PHI -> PSI -> THETA) -> (PHI -> THETA)",
         "PHI -> (PSI -> PHI & PSI)",
         "PHI & PSI -> PHI",
         "PHI & PSI -> PSI",
         "PHI -> PHI | PSI",
         "PSI -> PHI | PSI",
         "(PHI -> THETA) -> ((PSI -> THETA) -> (PHI | PSI -> THETA))",
         "(PHI -> PSI) -> ((PHI -> !PSI) -> !PHI)",
         "!!PHI -> PHI",
        };

#endif //PROOFCHECKER_AXIOMS_H
