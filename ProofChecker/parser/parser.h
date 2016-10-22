//
// Created by aleksandr on 03.10.16.
//

#ifndef PARSER_H
#define PARSER_H

#include "postfix_string_builder.h"
#include <memory>

struct parser
{
    friend struct ast;
public:
    std::string const& expression;

    struct node
    {
        //~node(); destructor isn't needed 'cause we use shared_ptrs

        node(std::string name) : type(var), left(NULL), right(NULL), var_name(name)
        { }

        node(token_types typ, std::shared_ptr<node> left_son, std::shared_ptr<node> right_son) : type(typ),
                                                                                                 left(left_son),
                                                                                                 right(right_son)
        { }

        token_types type;
        std::string var_name;

        std::shared_ptr<node> left;
        std::shared_ptr<node> right;
    };

private:
    parser(std::string const& expression) : expression(expression)
    { }
    std::shared_ptr<node> build_AST_from_postfix(); //builds AST from postfix string

    bool is_end_of_string(std::string&, size_t&);
    void skip_whitespace(std::string&, size_t&);
    token_types get_next_token(std::string&, size_t&);
    std::string get_var_name(std::string&, size_t&);
};


#endif //PROOFCHECKER_PARSER_H
