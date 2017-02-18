#include <iostream>
#include "predicate_parser/predicate_ast.h"
#include "predicate_parser/parser.h"
#include "predicate_parser/predicate_proof_processor.h"
#include "predicate_parser/axioms.h"
#include "predicate_parser/utils/expression_generator.h"

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

void generate_expressions(size_t num)
{
    std::freopen("tests/parser_tests/correct_expressions.in", "w", stdout);
    std::freopen("tests/parser_tests/log.txt", "w", stderr);
    std::ios::sync_with_stdio(false); //for faster output

    expression_generator eg;

    for (size_t i = 0; i < num; i++)
    {
        std::string str = eg.get_random_expression();
        std::cout << str << std::endl;
        std::cerr << str << "\t\t seed: " << eg.get_seed() << std::endl;
    }
}

void process_exprs(size_t num)
{
    std::freopen("tests/parser_tests/correct_expressions.in", "r", stdin);
    std::freopen("tests/parser_tests/output.out", "w", stdout);
    for(size_t i = 0; i < num; i++)
    {
        std::string e;
        std::getline(std::cin, e);
        parser p(e);
        predicate_ast ast = p.parse();
        std::cout << ast.to_string() << std::endl;
    }
}

void check_processor(size_t num)
{
    std::freopen("tests/parser_tests/output.out", "r", stdin);
    for(size_t i = 0; i < num; i++)
    {
        std::string e;
        std::getline(std::cin, e);
        parser p(e);
        predicate_ast ast = p.parse();
    }
}

int main()
{
    size_t num = 1000;
    generate_expressions(num);
    process_exprs(num);
    check_processor(num);

    return 0;
}