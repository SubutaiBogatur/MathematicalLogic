//
// Created by Aleksandr Tukallo on 17.02.17.
//

#include "predicate_ast.h"

std::map<token_types, uint8_t> precedence{
        {EQUALITY,    0},

        {FOR_ALL,     1},
        {EXISTS,      1},

        {ZERO,        3},
        {STROKE,      4},
        {MUL,         5},
        {SUM,         6},

        {NEGATION,    8},
        {CONJUNCTION, 9},
        {DISJUNCTION, 10},
        {IMPLICATION, 11},
        {ARGUMENTS,   12},
};

std::map<token_types, std::string> string_representation{
        {EQUALITY,    "="},

        {FOR_ALL,     "@"},
        {EXISTS,      "?"},

        {ZERO,        "0"},
        {STROKE,      "’"},
        {MUL,         "*"},
        {SUM,         "+"},

        {NEGATION,    "!"},
        {CONJUNCTION, "&"},
        {DISJUNCTION, "|"},
        {IMPLICATION, "->"},
        {ARGUMENTS,   "args"},
};

predicate_ast::node::node(node_ptr l, node_ptr r, token_types tt) : left(l), right(r), token_type(tt)
{
    this->str = string_representation.at(tt);
}