//
// Created by aleksandr on 03.10.16.
//

#ifndef PARSER_H

#include "postfix_string_builder.h"

struct parser
{
public:

    struct node;

    node *root;
    std::string& expression;

    parser(std::string& expression) : expression(expression), root(NULL)
    { }

    ~parser();

    struct node
    {
        node(std::string name) : type(var), left(NULL), right(NULL), var_name(name)
        { }

        node(token_types typ, node *left_son, node *right_son) : type(typ), left(left_son), right(right_son)
        { }

        token_types type;
        std::string var_name;

        node *left;
        node *right;
    };

    void build_AST_from_postfix(); //builds AST from postfix string

private:

    void recursive_node_deletion(node *);
    bool is_end_of_string(std::string&, size_t&);
    void skip_whitespace(std::string&, size_t&);
    token_types get_next_token(std::string&, size_t&);
    std::string get_var_name(std::string&, size_t&);
};

#define PARSER_H

#endif //PROOFCHECKER_PARSER_H
