//
// Created by Aleksandr Tukallo on 17.02.17.
//

#ifndef PREDICATECALCULUSPROOFPROCESSER_PARSER_H
#define PREDICATECALCULUSPROOFPROCESSER_PARSER_H

#include <string>
#include "predicate_ast.h"

struct parser
{
public:
    parser() = delete;
    parser(const std::string& to_parse) : to_parse(to_parse), cur_ind(0)
    {};

    predicate_ast parse();

    typedef std::shared_ptr<predicate_ast::node> node_ptr;
private:
    const std::string& to_parse;
    size_t cur_ind;

    node_ptr get_expression();
    node_ptr get_disjunction(node_ptr prev_disjunction);
    node_ptr get_conjunction(node_ptr prev_conjunction);
    node_ptr get_unary();

    node_ptr get_sum();
    node_ptr get_multiplication();
    node_ptr get_multiplier();

    node_ptr get_arguments_body();
    node_ptr get_arguments();

    node_ptr get_function();
    node_ptr get_predicate();

    void skip_whitespaces();
    void ensure_length(size_t l);
    bool check_length(size_t l);

    std::string get_function_name();
    std::string get_predicate_name();
    std::string get_digits_from_name();
};


#endif //PREDICATECALCULUSPROOFPROCESSER_PARSER_H
