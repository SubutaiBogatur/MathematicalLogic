//
// Created by Aleksandr Tukallo on 18.02.17.
//

#ifndef PREDICATECALCULUSPROOFPROCESSER_AXIOMS_H
#define PREDICATECALCULUSPROOFPROCESSER_AXIOMS_H

#include <vector>
#include "predicate_ast.h"

struct axioms
{
    struct axiom_check_result
    {
        int finded_ax;
        std::shared_ptr<predicate_ast::node> term, formula;
        std::string var;

        axiom_check_result()
                : finded_ax(-1)
        {
        }

        axiom_check_result(int finded_ax)
                : finded_ax(finded_ax), term(0),
                  formula(0)
        {
        }

        axiom_check_result(int finded_ax, std::shared_ptr<predicate_ast::node> term,
                           std::shared_ptr<predicate_ast::node> formula, std::string var)
                : finded_ax(finded_ax), term(term),
                  formula(formula), var(var)
        {
        }
    };

    //todo rename, refactor
    struct pred_rules_res
    {
        int res, finded_no;
        std::string var;
        std::shared_ptr<predicate_ast::node> formula;

        pred_rules_res()
                : res(0), finded_no(-1)
        {
        }

        pred_rules_res(int res, int finded_no)
                : res(res), finded_no(finded_no)
        {
        }

        pred_rules_res(int res, int finded_no, std::string& var, std::shared_ptr<predicate_ast::node> formula)
                : res(res), finded_no(finded_no),
                  var(var), formula(formula)
        {
        }
    };


public:
    axioms() = delete;
    static std::vector<predicate_ast> axioms_log;
    static std::vector<predicate_ast> axioms_mat;

    static void assemble_axioms_if_needed();

    //returns 0 if not an axiom,
    //  else returns num, if logical axiom
    //  else returns -num, it mathematical axiom
    static axiom_check_result is_an_axiom(predicate_ast ast);
    static pred_rules_res
    check_if_it_new_pred_rule(std::shared_ptr<predicate_ast::node> c, std::map<std::string, int> all_consequences);

    static bool is_9_math_axiom(predicate_ast ast);

private:
    //is used to compare expr with logical axioms
    static bool recursive_axiom_compare(std::shared_ptr<predicate_ast::node>& expr_nod,
                                        std::shared_ptr<predicate_ast::node>& ax_nod,
                                        std::map<char, std::string>& ax_leaves);
    static axiom_check_result is_11_axiom(predicate_ast ast);
    static axiom_check_result is_12_axiom(predicate_ast ast);

    static axiom_check_result is_result_of_substitution(std::string& var,
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
    static bool check_is_var_free_in_expr(std::string& var, std::shared_ptr<predicate_ast::node> c);
};


#endif //PREDICATECALCULUSPROOFPROCESSER_AXIOMS_H
