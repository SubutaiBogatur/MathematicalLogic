//
// Created by Aleksandr Tukallo on 17.02.17.
//

#include "parser.h"
#include "predicate_parser_exception.h"

// Our grammar:
//
// <expr> ::= <disj>
//              |<disj>"->"<expr>
// <disj> ::= <conj>
//              |<disj>"|"<conj>
// <conj> ::= <unary>
//              |<conj>"&"<unary>
// <unary> ::= <predicate>
//              |"!"<unary>
//              | "("<expr>")"
//              | ("@"|"?")<var><unary>
// <var> ::= ("a"..."z"){"0"..."9"}*
// <predicate> ::= ("A"..."Z"){"0"..."9"}*["("<term>{","<term>}*")"]
//              |<term>"="<term>
// <term> ::= <sum>
//              |<term>"+"<sum>
// <sum> ::= <mul>
//              |<sum>"*"<mul>
// <mul> ::= ("a"..."z"){"0"..."9"}*"("<term>{","<term>}*")"
//              |<var>
//              |"("<term>")"
//              |"0"
//              |<mul>"'"

//inv: cur_ind points to first not processed symbol

predicate_ast parser::parse()
{
    return predicate_ast(get_expression());
}

parser::node_ptr parser::get_expression()
{
    skip_whitespaces();
    node_ptr res = get_disjunction((node_ptr) NULL);
    if (check_length(0) && to_parse[cur_ind] == '-')
    {
        cur_ind++;
        skip_whitespaces();
        ensure_length(0);
        if (to_parse[cur_ind] != '>')
            throw predicate_parser_exception();
        cur_ind++;
        skip_whitespaces();
        res = std::make_shared<predicate_ast::node>(res, get_expression(), IMPLICATION);
    }
    skip_whitespaces();
    return res;
}

parser::node_ptr parser::get_disjunction(node_ptr prev_disjunction)
{
    skip_whitespaces();
    node_ptr res = get_conjunction(NULL);
    if (prev_disjunction != NULL)
    {
        res = std::make_shared<predicate_ast::node>(prev_disjunction, res, DISJUNCTION);
    }
    if (check_length(0) && to_parse[cur_ind] == '|')
    {
        cur_ind++;
        res = get_disjunction(res);
    }
    skip_whitespaces();
    return res;
}

parser::node_ptr parser::get_conjunction(node_ptr prev_conjunction)
{
    skip_whitespaces();
    node_ptr res = get_unary();
    if (prev_conjunction != NULL)
    {
        res = std::make_shared<predicate_ast::node>(prev_conjunction, res, CONJUNCTION);
    }

    if (check_length(0) && to_parse[cur_ind] == '&')
    {
        cur_ind++;
        skip_whitespaces();
        res = get_conjunction(res);
    }
    skip_whitespaces();
    return res;
}

parser::node_ptr parser::get_unary()
{
    skip_whitespaces();
    ensure_length(0);
    switch (to_parse[cur_ind])
    {
        case '!':
            cur_ind++;
            return std::make_shared<predicate_ast::node>((node_ptr) NULL, get_unary(), NEGATION);
        case '@':
        case '?':
            token_types res_type = to_parse[cur_ind] == '@' ? FOR_ALL : EXISTS;
            cur_ind++;
            node_ptr res = std::make_shared<predicate_ast::node>((node_ptr) NULL, (node_ptr) NULL, res_type);
            skip_whitespaces();
            res->left = std::make_shared<predicate_ast::node>((node_ptr) NULL, (node_ptr) NULL, VARIABLE,
                                                              get_function_name());
            res->right = get_unary();
            skip_whitespaces();
            return res;
    }
    size_t pos_before_try = cur_ind;
    try
    {
        ensure_length(0);
        if (to_parse[cur_ind] == '(')
        {
            cur_ind++;
            skip_whitespaces();
            node_ptr res = get_expression();

            ensure_length(0);
            if (to_parse[cur_ind] != ')')
            {
                throw predicate_parser_exception("Parenthesis expected");
            }
            cur_ind++;
            skip_whitespaces();
            return res;
        }
    }
    catch (predicate_parser_exception e)
    {
        cur_ind = pos_before_try;
    }
    return get_predicate();
}

parser::node_ptr parser::get_sum()
{
    skip_whitespaces();
    node_ptr res = get_multiplication();
    if (check_length(0) && to_parse[cur_ind] == '+')
    {
        cur_ind++;
        skip_whitespaces();
        res = std::make_shared<predicate_ast::node>(res, get_sum(), SUM);
    }
    skip_whitespaces();
    return res;
}

parser::node_ptr parser::get_multiplication()
{
    skip_whitespaces();
    node_ptr res = get_multiplier();
    if (check_length(0) && to_parse[cur_ind] == '*')
    {
        cur_ind++;
        skip_whitespaces();
        res = std::make_shared<predicate_ast::node>(res, get_multiplication(), MULTIPLICATION);
    }
    skip_whitespaces();
    return res;
}

parser::node_ptr parser::get_multiplier()
{
    skip_whitespaces();
    node_ptr res;

    if (to_parse[cur_ind] >= 'a' && to_parse[cur_ind] <= 'z')
    {
        res = get_function();
    } else if (to_parse[cur_ind] == '(')
    {
        cur_ind++;
        skip_whitespaces();
        res = get_sum();
        if (to_parse[cur_ind] != ')')
        {
            throw predicate_parser_exception("Parenthesis expected");
        }
        cur_ind++;
        skip_whitespaces();
    } else if (to_parse[cur_ind] == '0')
    {
        res = std::make_shared<predicate_ast::node>((node_ptr) NULL, (node_ptr) NULL, ZERO);
        cur_ind++;
        skip_whitespaces();
    } else
        throw predicate_parser_exception("Unable to parse multiplier");

    while (to_parse[cur_ind] == '\'')
    {
        res = std::make_shared<predicate_ast::node>(res, (node_ptr) NULL, STROKE);
        cur_ind++;
        skip_whitespaces();
    }
    return res;
}

parser::node_ptr parser::get_arguments_body()
{
    skip_whitespaces();
    node_ptr res = std::make_shared<predicate_ast::node>(get_sum(), (node_ptr) NULL, ARGUMENTS);
    if (check_length(0) && to_parse[cur_ind] == ',')
    {
        cur_ind++;
        skip_whitespaces();
        res->right = get_arguments_body();
    }
    return res;
}

parser::node_ptr parser::get_arguments()
{
    skip_whitespaces();
    ensure_length(0);
    if (to_parse[cur_ind] != '(')
    {
        throw predicate_parser_exception("Expected parenthesis");
    }
    cur_ind++;
    skip_whitespaces();
    node_ptr res = std::make_shared<predicate_ast::node>((node_ptr) NULL, get_arguments_body(), ARGUMENTS);
    ensure_length(0);
    if (to_parse[cur_ind] != ')')
    {
        throw predicate_parser_exception("Expected parenthesis");
    }
    cur_ind++;
    skip_whitespaces();
    return res;
}

parser::node_ptr parser::get_function()
{
    node_ptr res = std::make_shared<predicate_ast::node>((node_ptr) NULL, (node_ptr) NULL, FUNCTION,
                                                         get_function_name());
    if (check_length(0) && to_parse[cur_ind] == '(')
    {
        res->right = get_arguments();
    }
    return res;
}

parser::node_ptr parser::get_predicate()
{
    skip_whitespaces();
    ensure_length(0);
    if (to_parse[cur_ind] >= 'A' && to_parse[cur_ind] <= 'Z')
    {
        node_ptr res = std::make_shared<predicate_ast::node>((node_ptr) NULL, (node_ptr) NULL, PREDICATE,
                                                             get_predicate_name());
        if (check_length(0) && to_parse[cur_ind] == '(')
        {
            res->right = get_arguments();
        }
        skip_whitespaces();
        return res;
    } else
    {
        node_ptr res = std::make_shared<predicate_ast::node>((node_ptr) NULL, (node_ptr) NULL, EQUALITY);
        res->left = get_sum();

        ensure_length(0);
        if (to_parse[cur_ind] != '=')
        {
            throw predicate_parser_exception("Expected \"=\" symbol");
        }
        cur_ind++;
        skip_whitespaces();
        res->right = get_sum();

        skip_whitespaces();
        return res;
    }
}

std::string parser::get_function_name()
{
    skip_whitespaces();
    std::string name;
    ensure_length(0);
    if (to_parse[cur_ind] < 'a' || to_parse[cur_ind] > 'z')
    {
        throw predicate_parser_exception("Function name expected");
    }
    name.push_back(to_parse[cur_ind++]);
    name += get_digits_from_name();
    return name;
}

std::string parser::get_predicate_name()
{
    skip_whitespaces();
    std::string name;
    ensure_length(0);
    if (to_parse[cur_ind] < 'A' || to_parse[cur_ind] > 'Z')
    {
        throw predicate_parser_exception("Predicate name expected");
    }
    name.push_back(to_parse[cur_ind++]);
    name += get_digits_from_name();
    return name;
}

std::string parser::get_digits_from_name()
{
    std::string name_tail;
    while (check_length(0) && to_parse[cur_ind] >= '0' && to_parse[cur_ind] <= '9')
    {
        name_tail.push_back(to_parse[cur_ind++]);
    }
    skip_whitespaces();
    return name_tail;
}

//function ensures that there are l more symbols
//  in the to_parse string. If true, nothing happens,
//  else exception is thrown
inline void parser::ensure_length(size_t l)
{
    if (cur_ind + l >= to_parse.size())
        throw predicate_parser_exception("Expected more symbols");
}

//function is easy to use in if statements
inline bool parser::check_length(size_t l)
{
    return cur_ind + l < to_parse.size();
}

inline void parser::skip_whitespaces()
{
    while (cur_ind < to_parse.size()
           && (to_parse[cur_ind] == ' '
               || to_parse[cur_ind] == 9
               || to_parse[cur_ind] == 13))
    {
        cur_ind++;
    }
}
