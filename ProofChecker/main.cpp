#include <iostream>
#include "parser.h"
#include "axioms.h"

#include "temporary_functions_for_debug.h"
#include "hash.h"

int main()
{
    //We have ast tree ready to work with and axiom trees. Next steps will be done next
    std::string str = "(AB & BC) | (CD & BCD) -> !(ABCD12 -> K8 -> K5)";
    ast tree(str);

    //for next commit
    //TODO: function to check if tree is an axiom

    axioms ax;
    for (size_t i = 0; i < ax.axiom_ast.size(); i++)
        std::cout << std::hash<ast>{}(ax.axiom_ast[i]) << std::endl;

    std::cout << "\nfinished" << std::endl;
    return 0;
}