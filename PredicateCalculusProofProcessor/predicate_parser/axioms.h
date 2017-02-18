//
// Created by Aleksandr Tukallo on 18.02.17.
//

#ifndef PREDICATECALCULUSPROOFPROCESSER_AXIOMS_H
#define PREDICATECALCULUSPROOFPROCESSER_AXIOMS_H

#include <vector>
#include "predicate_ast.h"

struct axioms
{
public:
    axioms() = delete;
    static std::vector<predicate_ast> axioms_log;
    static std::vector<predicate_ast> axioms_mat;

    static void assemble_axioms_if_needed();
private:

};


#endif //PREDICATECALCULUSPROOFPROCESSER_AXIOMS_H
