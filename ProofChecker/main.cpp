#include <iostream>
#include "parser.h"
#include "axioms.h"

#include "temporary_functions_for_debug.h"
#include "hash.h"

int main()
{
    axioms ax;
    //We have ast tree ready to work with and axiom trees. Next steps will be done next
    std::string str1 = "(AB & BC) | (CD & BCD) -> !(ABCD12 -> K8 -> K5)";
    std::string str2 = "(AB & BC) | (CD & BCD) -> !(ABCD12 -> K8 -> K5)";
    std::string str3 = "((!A | !B)->A)->((!A|!B)->!A)->!(!A|!B)";
    std::string str4 = "(!A->A)->((!B->A)->((!A|!B)->A))";
    ast tree1(str1);
    ast tree2(str2);
    ast tree3(str3);
    ast tree4(str4);

    std::cout << static_cast<int>(tree4.is_tree_an_axiom()) << std::endl;

//    for (size_t i = 0; i < axioms::axiom_ast.size(); i++)
//        std::cout << std::hash<ast>{}(axioms::axiom_ast[i]) << std::endl;

    std::cout << "\nfinished" << std::endl;
    return 0;
}