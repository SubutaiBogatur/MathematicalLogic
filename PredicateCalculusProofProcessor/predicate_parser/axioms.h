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

    //returns 0 if not an axiom,
    //  else returns num, if logical axiom
    //  else returns -num, it mathematical axiom
    static int8_t is_an_axiom(predicate_ast ast);

private:
    //is used to compare expr with logical axioms
    static bool recursive_axiom_compare(std::shared_ptr<predicate_ast::node>& expr_nod,
                                        std::shared_ptr<predicate_ast::node>& ax_nod,
                                        std::map<char, std::string>& ax_leaves);
    static bool is_11_axiom(predicate_ast ast);
    static bool is_12_axiom(predicate_ast ast);
    static bool is_result_of_substitution(std::string& var,
                                          std::shared_ptr<predicate_ast::node>& expr1,
                                          std::shared_ptr<predicate_ast::node>& expr2);
    static bool is_result_of_sub_rec(std::string& var,
                                     std::shared_ptr<predicate_ast::node>& expr1,
                                     std::shared_ptr<predicate_ast::node>& expr2,
                                     std::set<std::string>& locked_vars,
                                     std::multiset<std::string>& can_be_locked,
                                     std::shared_ptr<predicate_ast::node>& new_finded_expr);
    static bool check_is_it_free_for_substitution(std::shared_ptr<predicate_ast::node>& expr,
                                                  std::set<std::string>& locked_vars);
};


#endif //PREDICATECALCULUSPROOFPROCESSER_AXIOMS_H
