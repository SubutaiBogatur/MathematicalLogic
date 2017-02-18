//
// Created by Aleksandr Tukallo on 18.02.17.
//

#include "axioms.h"
#include "parser.h"

const std::vector<std::string> axioms_log_str = {
        "A->B->A",
        "(A->B)->(A->B->C)->(A->C)",
        "A->B->A&B",
        "A&B->A",
        "A&B->B",
        "A->A|B",
        "B->A|B",
        "(A->C)->(B->C)->(A|B->C)",
        "(A->B)->(A->!B)->!A",
        "!!A->A"
};

const std::vector<std::string> axioms_mat_str = {
        "a=b->a'=b'",
        "a=b->a=c->b=c",
        "a'=b'->a=b",
        "!a'=0",
        "a+b'=(a+b)'",
        "a+0=a",
        "a*0=0",
        "a*b'=a*b+a"
};

//defining static fields
std::vector<predicate_ast> axioms::axioms_log;
std::vector<predicate_ast> axioms::axioms_mat;

void axioms::assemble_axioms_if_needed()
{
    if (axioms::axioms_log.size() == 0)
    {
        for (size_t i = 0; i < axioms_log_str.size(); i++)
        {
            axioms_log.push_back(parser(axioms_log_str[i]).parse());
        }
        for (size_t i = 0; i < axioms_mat_str.size(); i++)
        {
            axioms_mat.push_back(parser(axioms_mat_str[i]).parse());
        }
    }
}