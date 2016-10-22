//
// Created by aleksandr on 19.10.16.
//

#ifndef PROOFCHECKER_AST_H
#define PROOFCHECKER_AST_H

#include "parser.h"
#include <assert.h>
#include <vector>
#include <unordered_map>

struct ast
{
public:
    std::shared_ptr<parser::node> root;

    //TODO
    //uint32_t hash_of_the_tree; //??

    ast(std::string const& expression);

    //post:
    // if not an axiom returns 0
    // else returns number of axiom (i + 1)
    uint8_t is_tree_an_axiom() const;

    bool is_the_same(ast &);

private:
    void recursive_axiom_equality_check(parser::node *, parser::node *,
                                        std::unordered_map<std::string, std::vector<parser::node *>>&) const;
    void recursive_equality_check(parser::node *, parser:: node *) const;
    void greek_letters_equality_check(std::unordered_map<std::string, std::vector<parser::node *>>&) const;
};


#endif //PROOFCHECKER_AST_H
