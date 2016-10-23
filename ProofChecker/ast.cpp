//
// Created by Aleksandr Tukallo on 19.10.16.
//

#include "ast.h"
#include "axioms.h"

struct ast_exception
{
};

//public:

ast::ast(std::string const& expression)
{
    root = parser(expression).build_AST_from_postfix();
}

ast::ast(std::shared_ptr<parser::node> new_root)
{
    root = new_root;
}

uint16_t ast::is_an_axiom() const
{
    assert(axioms::axiom_ast.size() == 10); //ensure, that axioms initialized
    for (size_t i = 0; i < axioms::axiom_ast.size(); i++)
    {
        try
        {
            std::unordered_map<std::string, std::vector<std::shared_ptr<parser::node>>> greek_letters;
            recursive_axiom_equality_check(this->root, axioms::axiom_ast[i].root, greek_letters);
            greek_letters_equality_check(greek_letters);
            return static_cast<uint16_t>(i + 1);
        } catch (ast_exception&)
        { }
    }
    return 0;
}

bool ast::is_implication_first()
{
    return this->root->type == impl;
}

bool ast::is_the_same(ast const& other) const
{
    try
    {
        recursive_equality_check(this->root, other.root);
        return true;
    } catch (ast_exception& e)
    {
        return false;
    }
}

bool operator==(const ast& lhs, const ast& rhs)
{
    return ast(lhs).is_the_same(rhs);
}

//private:

void ast::recursive_axiom_equality_check(std::shared_ptr<parser::node> nod_expr, std::shared_ptr<parser::node> nod_ax,
                                         std::unordered_map<std::string, std::vector<std::shared_ptr<parser::node> >>& greek_letters) const
{
    if (nod_ax->left != NULL && nod_ax->right != NULL)
    {
        //bin op
        if (nod_expr->left == NULL || nod_expr->right == NULL)
            throw ast_exception();
        if (nod_expr->type != nod_ax->type)
            throw ast_exception();

        recursive_axiom_equality_check(nod_expr->left, nod_ax->left, greek_letters);
        recursive_axiom_equality_check(nod_expr->right, nod_ax->right, greek_letters);
        return;
    } else if (nod_ax->left != NULL)
    {
        //neg
        if (nod_expr->left == NULL)
            throw ast_exception();
        if (nod_expr->type != nod_ax->type)
            throw ast_exception();
        recursive_axiom_equality_check(nod_expr->left, nod_ax->left, greek_letters);
        return;
    }
    else
    {
        //greek var
        auto finded = greek_letters.find(nod_ax->var_name);
        if (finded == greek_letters.end())//greek letter wasn't inserted
        {
            //associating greek letter in ax node with ptr to expr node to check if all greek letters are the same later
            greek_letters.insert({nod_ax->var_name, std::vector<std::shared_ptr<parser::node> >(1, nod_expr)});
        }
        else //letter was already inserted
        {
            (*finded).second.push_back(nod_expr);
        }
    }
}

void ast::greek_letters_equality_check(
        std::unordered_map<std::string, std::vector<std::shared_ptr<parser::node> >>& greek_letters) const
{
    for (auto it = greek_letters.begin(); it != greek_letters.end(); it++)
    {
        //Greek letters can only be met once, twice, or three times in any axiom.
        if ((*it).second.size() == 2)
        {
            recursive_equality_check((*it).second[0], (*it).second[1]);
        } else if ((*it).second.size() == 3)
        {
            recursive_equality_check((*it).second[0], (*it).second[1]);
            recursive_equality_check((*it).second[0], (*it).second[2]);
        }
    }
}

void ast::recursive_equality_check(std::shared_ptr<parser::node> nod_expr, std::shared_ptr<parser::node> nod_ax) const
{
    if (nod_expr->left != NULL && nod_expr->right != NULL)
    {
        //bin op
        if (nod_ax->left == NULL || nod_ax->right == NULL)
            throw ast_exception();
        if (nod_expr->type != nod_ax->type)
            throw ast_exception();

        recursive_equality_check(nod_expr->left, nod_ax->left);
        recursive_equality_check(nod_expr->right, nod_ax->right);
        return;
    } else if (nod_expr->left != NULL)
    {
        //neg
        if (nod_ax->left == NULL)
            throw ast_exception();
        if (nod_expr->type != nod_ax->type)
            throw ast_exception();
        recursive_equality_check(nod_expr->left, nod_ax->left);
        return;
    }
    else
    {
        //var
        if (nod_expr->var_name != nod_ax->var_name)
            throw ast_exception();
        return;
    }
}