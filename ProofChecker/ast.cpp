//
// Created by aleksandr on 19.10.16.
//

#include "ast.h"

//public:

ast::ast(std::string const& expression)
{
    root = parser(expression).build_AST_from_postfix();
}