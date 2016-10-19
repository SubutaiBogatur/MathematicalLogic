#include <iostream>
#include "parser.h"

int main()
{

    //We have ast tree ready to work with. Next steps will be done next
    std::string str = "(AB & BC) | (CD & BCD) -> !(ABCD12 -> K8 -> K5)";
    parser tmp(str);

    try
    {
        tmp.build_AST_from_postfix();
    }
    catch (parser_exception e)
    {
        std::cout << e.msg << std::endl;
    }

    std::cout << "finished" << std::endl;
    return 0;
}
