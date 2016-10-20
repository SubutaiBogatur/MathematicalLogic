//
// Created by aleksandr on 20.10.16.
//

#ifndef PROOFCHECKER_TEMPORARY_FUNCTIONS_FOR_DEBUG_H
#define PROOFCHECKER_TEMPORARY_FUNCTIONS_FOR_DEBUG_H
#include "ast.h"
#include <iostream>

//is included in main only to test new features

void recursive_print(parser::node *nod);
void ast_visualisator(ast& tree)
{
    recursive_print(tree.root.get());
    std::cout << std::endl;
}

void print_node(parser::node *nod)
{
    if (nod->type == var)
    {
        std::cout << nod->var_name + " ";
        return;
    }
    switch (nod->type)
    {
        case conj:
            std::cout << "& ";
            break;
        case disj:
            std::cout << "| ";
            break;
        case impl:
            std::cout << "-> ";
            break;
        case neg:
            std::cout << "! ";
            break;
        default:
            std::cout << "not expected such type, wtf ";
            break;
    }
}

void recursive_print(parser::node *nod)
{
    if (nod->left != NULL)
        recursive_print(nod->left);

    print_node(nod);

    if (nod->right != NULL)
        recursive_print(nod->right);
}

#endif //PROOFCHECKER_TEMPORARY_FUNCTIONS_FOR_DEBUG_H
