#include <iostream>
#include "predicate_parser/predicate_ast.h"
#include "predicate_parser/parser.h"
#include "predicate_parser/predicate_proof_processor.h"
#include "predicate_parser/axioms.h"

//todo delete this code
//structs just for debugging in Clion
//they produce a lot of leaks, strictly tmp only
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
//    predicate_proof_processor ppp("./tests/input.in", "./tests/output.out");
    std::string str = "a=b";
    m_expr* ptr = ptr_from_shared(parser(str).parse().root);
    predicate_ast ast = parser(str).parse();
    std::vector<std::string> vec =  ast.get_all_free_vars();

    std::cout << "\nfinished" << std::endl;

    return 0;
}