//
// Created by aleksandr on 19.10.16.
//

#include "axioms.h"

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

//initializing static field
std::vector<ast> axioms::axiom_ast;

axioms::axioms()
{
    for (size_t i = 0; i < axiom_names.size(); i++)
    {
        ast tmp(axiom_names[i]);
        axiom_ast.push_back(tmp);
    }
}