//
// Created by Aleksandr Tukallo on 17.02.17.
//

#include <assert.h>
#include <iostream>
#include "predicate_ast.h"

predicate_ast::node::node(node_ptr l, node_ptr r, token_types tt) : left(l), right(r), token_type(tt)
{
    this->str = string_representation.at(tt);
}

bool predicate_ast::is_var_free(std::string const& var) const
{
    return is_var_free_rec(var, this->root);
}

bool predicate_ast::is_var_free_rec(std::string const& var, std::shared_ptr<node> const& cur_node) const
{
    bool free_in_left = false;
    bool free_in_right = false;

    //if this is quantifier and it blocks needed var
    if ((cur_node->token_type == FOR_ALL || cur_node->token_type == EXISTS) && cur_node->left->str == var)
    {
        return false;
    }

    //variables never have children, but function can!!
    if (cur_node->token_type == VARIABLE /*|| cur_node->token_type == FUNCTION*/)
    {
//        assert (cur_node->left == NULL && cur_node->right == NULL); //functions can have children
        return cur_node->str == var;
    }

    if (cur_node->left != NULL)
    {
        free_in_left = is_var_free_rec(var, cur_node->left);
    }
    if (cur_node->right != NULL)
    {
        free_in_right = is_var_free_rec(var, cur_node->right);
    }

    return free_in_left || free_in_right;
}

//todo function can be optimized
//function walks the tree and returns vector with the names of all free vars in ast
std::set<std::string> predicate_ast::get_all_free_vars() const
{
    std::set<std::string> set;
    tree_walk(set, this->root);

//    std::vector<std::string> ret;
    std::set<std::string> ret;
    auto it = set.begin();
    for (; it != set.end(); it++)
    {
        if (this->is_var_free(*it))
        {
//            ret.push_back(*it);
            ret.insert(*it);
        }
    }

    return ret;
}

//functions adds all the variable to the set
void predicate_ast::tree_walk(std::set<std::string>& set, std::shared_ptr<node> const& cur_node) const
{
    if (cur_node->token_type == VARIABLE)
    {
        set.insert(cur_node->str);
    }

    if (cur_node->left != NULL)
    {
        tree_walk(set, cur_node->left);
    }

    if (cur_node->right != NULL)
    {
        tree_walk(set, cur_node->right);
    }
}

std::string predicate_ast::to_string() const
{
    std::string ret;
    rec_to_string(root, ret, 255, 1);
    return ret;
}

void predicate_ast::rec_to_string(std::shared_ptr<node> const& cur_node, std::string& res,
                                  uint8_t prev_prec, size_t pos) const
{
    bool brackets = 0;
    int associativity = 0;

    if ((cur_node->token_type == DISJUNCTION) || (cur_node->token_type == CONJUNCTION))
    {
        associativity = 1;
    }

//    std::cout << precedence.find(cur_node->token_type)->second << std::endl;
    if ((precedence.find(cur_node->token_type)->second > prev_prec) ||
        ((precedence.find(cur_node->token_type)->second == prev_prec) && (pos == associativity)))
    {
        brackets = 1;
    }
    if ((precedence.find(cur_node->token_type)->second == prev_prec) && (pos == 0) && (cur_node->token_type == STROKE))
    {
        brackets = 0;
    }

    if (brackets)
    {
        res.push_back('(');
    }

    bool cur_node_is_quantifier = cur_node->token_type == FOR_ALL || cur_node->token_type == EXISTS;
    if (cur_node_is_quantifier)
    {
        res += string_representation.at(cur_node->token_type);
    }

    //left son
    if (cur_node->left != NULL)
    {
        rec_to_string(cur_node->left, res, precedence.at(cur_node->token_type), 0);
    }

    //node by itself
    if (!cur_node_is_quantifier)
    {
        if (cur_node->token_type != ARGUMENTS)
        {
            //for most of token_types
            res.append(cur_node->has_default_string() ? string_representation.at(cur_node->token_type) : cur_node->str);
        } else if ((cur_node->left != NULL) && (cur_node->right != NULL))
        {
            res.append(",");
        }
    }

    //right son
    if (cur_node->right != NULL)
    {
        rec_to_string(cur_node->right, res, precedence.at(cur_node->token_type), 1);
    }

    if (brackets)
    {
        res.push_back(')');
    }
}

bool predicate_ast::equals(predicate_ast const& other) const
{
    return recursive_equals(this->root, other.root);
}

bool predicate_ast::recursive_equals(std::shared_ptr<node> const& this_node,
                                     std::shared_ptr<node> const& other_node) const
{
    if (!this_node->equals(*other_node))
        return false;

    if (this_node->left != NULL && other_node->left != NULL
        && this_node->right != NULL && other_node->right != NULL)
        return recursive_equals(this_node->left, other_node->left)
               && recursive_equals(this_node->right, other_node->right);

    if (this_node->left != NULL && other_node->left != NULL)
        return recursive_equals(this_node->left, other_node->left);

    if (this_node->right != NULL && other_node->right != NULL)
        return recursive_equals(this_node->right, other_node->right);

    if (this_node->is_leave() && other_node->is_leave())
        return true;

    //else children are different
    return false;
}

