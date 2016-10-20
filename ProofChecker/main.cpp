#include <iostream>
#include "parser.h"
#include "axioms.h"

#include "temporary_functions_for_debug.h"

int main()
{
    //We have ast tree ready to work with and axiom trees. Next steps will be done next
    std::string str = "(AB & BC) | (CD & BCD) -> !(ABCD12 -> K8 -> K5)";
    ast tree(str);

    axioms ax;
    for (size_t i = 0; i < ax.axiom_ast.size(); i++)
        ast_visualisator(ax.axiom_ast[i]);

    std::cout << "\nfinished" << std::endl;
    return 0;
}