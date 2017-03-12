//
// Created by Aleksandr Tukallo on 18.02.17.
//

#include <assert.h>
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

bool axioms::check_is_var_free_in_expr(std::string const& var, std::shared_ptr<predicate_ast::node> const& c)
{
    if (!c)
    {
        return 0;
    }
    if ((c->token_type == VARIABLE || c->token_type == PREDICATE) && c->str == var)
    {
        return 1;
    }
    if ((c->token_type == FOR_ALL) || (c->token_type == EXISTS))
    {
        //if variable is blocked
        if (c->left->str == var)
        {
            return 0;
        }
    }
    if (axioms::check_is_var_free_in_expr(var, c->left))
    {
        return 1;
    }
    if (axioms::check_is_var_free_in_expr(var, c->right))
    {
        return 1;
    }
    return 0;
}

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

//tested, seems ok
bool axioms::recursive_axiom_compare(std::shared_ptr<predicate_ast::node> const& expr_nod,
                                     std::shared_ptr<predicate_ast::node> const& ax_nod,
                                     std::map<char, std::string>& ax_leaves)
{
    if (expr_nod == NULL)
    {
        return true;
    }

    if ((ax_nod->token_type == VARIABLE ||
         ax_nod->token_type == PREDICATE) //in current parser leaves in ax are always predicates
        && (ax_nod->str.length() == 1))
    {
        char v = ax_nod->str[0];
        if (ax_leaves.find(v) == ax_leaves.end())
        {
            ax_leaves.insert({v, predicate_ast(expr_nod).to_string()});
        } else
        {
            //comparing same leaves in axiom
            if (ax_leaves[v] != predicate_ast(expr_nod).to_string())
            {
                return false;
            }
        }
        return true;
    }

    if (!ax_nod->equals(*expr_nod))
    {
        return false;
    }

    if (((ax_nod->left == 0) && (expr_nod->left != 0)) ||
        ((ax_nod->left != 0) && (expr_nod->left == 0)) ||
        ((ax_nod->right == 0) && (expr_nod->right != 0)) ||
        ((ax_nod->right != 0) && (expr_nod->right == 0)))
    {
        return true;
    }

    if (ax_nod->left)
    {
        if (!recursive_axiom_compare(expr_nod->left, ax_nod->left, ax_leaves))
        {
            return 0;
        }
    }

    if (ax_nod->right)
    {
        if (!recursive_axiom_compare(expr_nod->right, ax_nod->right, ax_leaves))
        {
            return 0;
        }
    }

    return true;
}

axioms::axiom_check_result axioms::is_result_of_substitution(std::string const& var,
                                                             std::shared_ptr<predicate_ast::node>& expr1,
                                                             std::shared_ptr<predicate_ast::node>& expr2)
{
    std::multiset<std::string> can_be_locked;
    std::set<std::string> locked_vars;
    std::shared_ptr<predicate_ast::node> new_finded_expr = NULL;

    bool res = is_result_of_sub_rec(var, expr1, expr2, locked_vars, can_be_locked, new_finded_expr);
    if (res == 0)
    {
        return axiom_check_result(0);
    }
    if (check_is_it_free_for_substitution(new_finded_expr, locked_vars))
    {
        return axiom_check_result(1);
    }
    return axiom_check_result(-1, new_finded_expr, expr1, var);
}

bool axioms::check_is_it_free_for_substitution(std::shared_ptr<predicate_ast::node> const& expr,
                                               std::set<std::string> const& locked_vars)
{
    if (expr == NULL)
    {
        return 1;
    }
    if (expr->token_type == VARIABLE)
    {
        return (locked_vars.find(expr->to_string()) == locked_vars.end());
    }

    if (!check_is_it_free_for_substitution(expr->left, locked_vars))
    {
        return 0;
    }
    if (!check_is_it_free_for_substitution(expr->right, locked_vars))
    {
        return 0;
    }
    return 1;
}

bool axioms::is_result_of_sub_rec(std::string const& var, std::shared_ptr<predicate_ast::node> const& expr1,
                                  std::shared_ptr<predicate_ast::node> const& expr2,
                                  std::set<std::string>& locked_vars,
                                  std::multiset<std::string>& can_be_locked,
                                  std::shared_ptr<predicate_ast::node>& new_finded_expr)
{
    if (expr1->token_type == VARIABLE && expr1->to_string() == var)
    {
        if (new_finded_expr == 0)
        {
            new_finded_expr = expr2;
        } else
        {
            if (predicate_ast(new_finded_expr).to_string() != predicate_ast(expr2).to_string())
            {
                return 0;
            }
        }
        for (auto v : can_be_locked)
        {
            locked_vars.insert(v);
        }
        can_be_locked.clear();
        return 1;
    }

    if (!expr1->equals(*expr2))
    {
        return 0;
    }

    if ((expr1->token_type == FOR_ALL) || (expr1->token_type == EXISTS))
    {
        if (!expr1->left->equals(*(expr2->left)))
        {
            return 0;
        }
        std::string quant_var = expr1->left->to_string();

        if (quant_var == var)
        {
            return (predicate_ast(expr1->right).to_string() == predicate_ast(expr2->right).to_string());
        }
        if (locked_vars.find(quant_var) == locked_vars.end())
        {
            can_be_locked.insert(quant_var);
        }

        bool res = is_result_of_sub_rec(var, expr1->right, expr2->right, locked_vars, can_be_locked, new_finded_expr);
        can_be_locked.erase(quant_var);
        return res;
    }
    if (((expr1->left == 0) ^ (expr2->left == 0)) ||
        ((expr1->right == 0) ^ (expr2->right == 0)))
    {
        return 0;
    }

    if (expr1->left)
    {
        if (!is_result_of_sub_rec(var, expr1->left, expr2->left, locked_vars, can_be_locked, new_finded_expr))
        {
            return 0;
        }
    }
    if (expr1->right)
    {
        if (!is_result_of_sub_rec(var, expr1->right, expr2->right, locked_vars, can_be_locked, new_finded_expr))
        {
            return 0;
        }
    }
    return 1;
}


axioms::axiom_check_result axioms::is_11_axiom(predicate_ast& ast)
{
    std::shared_ptr<predicate_ast::node>& c = ast.root;
    if ((!c) || (c->token_type != IMPLICATION) || (c->left->token_type != FOR_ALL))
    {
        return 0;
    }

    std::string var = c->left->left->to_string();
    std::shared_ptr<predicate_ast::node> expr1 = c->left->right;
    std::shared_ptr<predicate_ast::node> expr2 = c->right;

    axiom_check_result res = is_result_of_substitution(var, expr1, expr2);
    res.finded_ax *= 10;
    return res;
}

axioms::axiom_check_result axioms::is_12_axiom(predicate_ast& ast)
{
    std::shared_ptr<predicate_ast::node> c = ast.root;
    if ((!c) || (c->token_type != IMPLICATION) || (c->right->token_type != EXISTS))
    {
        return 0;
    }
    std::string var = c->right->left->to_string();
    std::shared_ptr<predicate_ast::node> c1 = c->right->right;
    std::shared_ptr<predicate_ast::node> c2 = c->left;

    axiom_check_result res = is_result_of_substitution(var, c1, c2);
    res.finded_ax *= 11;
    return res;
}

axioms::axiom_check_result axioms::is_an_axiom(predicate_ast& ast)
{
    assert (axioms_mat.size() == 8);
    //firstly check if mathematical axiom:
    std::string str = ast.to_string();
    for (size_t i = 0; i < axioms::axioms_mat.size(); i++)
    {
        if (str == axioms::axioms_mat[i].to_string())
        {
            return static_cast<int8_t>(static_cast<int8_t >(i + 12)); //explicit casts to suppress warnings
        }
    }

    assert (axioms_log.size() == 10);
    //now check if logical axiom:
    for (size_t i = 0; i < axioms::axioms_log.size(); i++)
    {
        std::map<char, std::string> map; //every new cycle it dies
        if (recursive_axiom_compare(ast.root, axioms_log[i].root, map))
        {
            return static_cast<int8_t>(static_cast<int8_t >(i));
        }
    }

    axiom_check_result ret_res, lok_res;
    lok_res = is_11_axiom(ast);
    if (lok_res.finded_ax > 0)
    {
        return lok_res;
    } else
    {
        if (lok_res.finded_ax < 0)
        {
            ret_res = lok_res;
        }
    }

    lok_res = is_12_axiom(ast);
    if (lok_res.finded_ax > 0)
    {
        return lok_res;
    } else
    {
        if (lok_res.finded_ax < 0)
        {
            ret_res = lok_res;
        }
    }
    return ret_res;
}

axioms::pred_rules_res axioms::check_if_it_new_pred_rule(
        std::shared_ptr<predicate_ast::node> const& c, std::map<std::string, size_t> const& all_consequences)
{
    if (c->token_type != IMPLICATION)
    {
        return {0, -1};
    }

    pred_rules_res ret_res;

    if (c->right->token_type == FOR_ALL)
    {
        std::shared_ptr<predicate_ast::node> v = std::make_shared<predicate_ast::node>(c->left, c->right->right,
                                                                                       IMPLICATION);

        auto it = all_consequences.find(predicate_ast(v).to_string());
        if (it != all_consequences.end())
        {
            int no = (*it).second;

            pred_rules_res lok_res(1, no, c->right->left->str, c->left);
            if (check_is_var_free_in_expr(lok_res.var, lok_res.formula))
            {
                lok_res.res *= -1;
            }

            if ((lok_res.res > 0) || (ret_res.res == 0))
            {
                ret_res = lok_res;
            }
        }
    }

    if (c->left->token_type == EXISTS)
    {
        std::shared_ptr<predicate_ast::node> v = std::make_shared<predicate_ast::node>(c->left->right, c->right,
                                                                                       IMPLICATION);

        auto it = all_consequences.find(predicate_ast(v).to_string());
        if (it != all_consequences.end())
        {
            int no = (*it).second;

            pred_rules_res lok_res(2, no, c->left->left->str, c->right);
            if (check_is_var_free_in_expr(lok_res.var, lok_res.formula))
            {
                lok_res.res *= -1;
            }

            if ((lok_res.res > 0) || (ret_res.res == 0))
            {
                ret_res = lok_res;
            }
        }
    }
    return ret_res;
}

//substitutes variable
std::shared_ptr<predicate_ast::node> axioms::substitute(
        std::shared_ptr<predicate_ast::node> c, std::string const& old_val, std::string const& new_val)
{

    if (c == 0)
    {
        return NULL;
    }

    std::shared_ptr<predicate_ast::node> ret = std::make_shared<predicate_ast::node>(c->left, c->right, c->token_type,
                                                                                     c->str);

    if ((c->token_type == FOR_ALL || c->token_type == EXISTS) && c->left->str == old_val)
    {
        return ret; //all lower is blocked
    }

    if (c->token_type == VARIABLE && c->str == old_val)
    {
        ret->str = new_val;
        return ret;
    }

    if (c->left != NULL)
    {
        ret->left = substitute(c->left, old_val, new_val);
    }
    if (c->right != NULL)
    {
        ret->right = substitute(c->right, old_val, new_val);
    }
    return ret;
}

bool axioms::is_9_math_axiom(predicate_ast& ast)
{
    if (ast.root->token_type == IMPLICATION)
    {
        if (ast.root->left->token_type == CONJUNCTION
            && ast.root->left->right->token_type == FOR_ALL)
        {
            std::string to_comp = ast.to_string();

            std::string var = ast.root->left->right->left->str;
            std::shared_ptr<predicate_ast::node> subst_0 = ast.root->right;
            std::shared_ptr<predicate_ast::node> subst_shtr = ast.root->right;

            std::string str_subst_0 = predicate_ast(substitute(subst_0, var, "0")).to_string();
            std::string str_subst_shtr = predicate_ast(substitute(subst_shtr, var, var + "'")).to_string();
            std::string str_no_subst = predicate_ast(ast.root->right).to_string();

            std::string ax = "((" + str_subst_0 + ")" + "&" + "@" + var + "((" + str_no_subst + ")->" + str_subst_shtr +
                             "))->" + str_no_subst;
            ax = predicate_ast(parser(ax).parse()).to_string(); //just to ensure, that everything is okk

            return ax == to_comp;
        }
    }
    return false;
}