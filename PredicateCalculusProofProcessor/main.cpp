#include <iostream>
#include "predicate_parser/predicate_ast.h"
#include "predicate_parser/parser.h"
#include "expression_generator.h"

//structs just for debugging in Clion
struct m_expr
{
    m_expr *left;
    m_expr *right;

    std::string str;
    token_types tt;
};

m_expr *ptr_from_shared(std::shared_ptr<predicate_ast::node> sh_ptr)
{
    m_expr *ptr = new m_expr();
    ptr->left = sh_ptr->left == NULL ? 0 : ptr_from_shared(sh_ptr->left);
    ptr->right = sh_ptr->right == NULL ? 0 : ptr_from_shared(sh_ptr->right);
    ptr->tt = sh_ptr->token_type;
    ptr->str = sh_ptr->str;
    return ptr;
}
//end of code for debugging only

int main()
{
//    predicate_ast ast = parser("@xP(x)").parse();
//    m_expr *tmp = ptr_from_shared(ast.root);

    //testing parser
    std::freopen("./tests/parser_tests/correct_expressions.in", "w", stdout);
    expression_generator eg;
    for (size_t i = 0; i < 10000; i++)
    {
        std::string str = eg.get_random_expression();
        std::cout << str << " seed: " << eg.get_seed();
        parser(str).parse();
        std::cout << " parsed succesfully\n";
    }

//    std::cout << expression_generator::get_random_expression() << std::endl;
//    std::cout << "finished" << std::endl;

    return 0;
}