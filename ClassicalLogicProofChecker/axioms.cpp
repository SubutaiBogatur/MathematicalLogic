//
// Created by Aleksandr Tukallo on 19.10.16.
//

#include "axioms.h"

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