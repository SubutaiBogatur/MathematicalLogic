//
// Created by Aleksandr Tukallo on 19.10.16.
//

#ifndef PROOFCHECKER_AST_H
#define PROOFCHECKER_AST_H

#include "parser/parser.h"
#include <assert.h>
#include <vector>
#include <unordered_map>

struct ast
{
public:
    //fields are public, because they are accessed from classical_logic_proof_checker a lot
    std::shared_ptr<parser::node> root;
    //if proved negative - it's num of hypothesis
    int32_t line_num;

    ast(std::string const& expression);
    ast(std::shared_ptr<parser::node>);

    //post:
    // if not an axiom returns 0
    // else returns number of axiom (i + 1)
    uint16_t is_an_axiom() const;

    bool is_implication_first();
    bool is_the_same(ast const&) const;

private:
    void recursive_axiom_equality_check(std::shared_ptr<parser::node>, std::shared_ptr<parser::node>,
                                        std::unordered_map<std::string, std::vector<std::shared_ptr<parser::node>>>&) const;
    void recursive_equality_check(std::shared_ptr<parser::node>, std::shared_ptr<parser::node>) const;
    void greek_letters_equality_check(
            std::unordered_map<std::string, std::vector<std::shared_ptr<parser::node> >>&) const;

    friend bool operator==(const ast& lhs, const ast& rhs);
};

bool operator==(const ast& lhs, const ast& rhs);

#endif //PROOFCHECKER_AST_H
