//
// Created by Aleksandr Tukallo on 17.02.17.
//

#ifndef PREDICATECALCULUSPROOFPROCESSER_PREDICATE_AST_H
#define PREDICATECALCULUSPROOFPROCESSER_PREDICATE_AST_H

#include <memory>
#include <map>
#include <vector>
#include <set>

enum token_types
{
    NEGATION,
    CONJUNCTION,
    DISJUNCTION,
    IMPLICATION,
    ARGUMENTS,

    PREDICATE, //eg P(a, b, c) or P
    VARIABLE, //eg x
//    FUNCTION,  //eg f(a, b) //nb We will treat functions and vars the same. All the funcs are vars from now.

    ZERO,
    STROKE, //eg 0'
    MULTIPLICATION,
    SUM,

    FOR_ALL, //eg @
    EXISTS, //eg ?

    EQUALITY
};

/* Predicate can have multiple or zero arguments,
  it's when ARGUMENTS token_type is needed, eg:
  P(a,b) is converted in:
      P
       \
      ARGS
        \
       ARGS
      /   \
     a    ARGS
          /
         b
 */

struct predicate_ast
{
    friend struct parser;
public:
    predicate_ast() = delete;

    std::string to_string();
    bool is_var_free(std::string const& var);
    std::vector<std::string> get_all_free_vars();

    //todo private
public:
    struct node
    {
        typedef std::shared_ptr<node> node_ptr;

        node_ptr left;
        node_ptr right;

        token_types token_type;

        std::string str;

        node(node_ptr l, node_ptr r,
             token_types tt, std::string str) : left(l), right(r), token_type(tt), str(str)
        {};

        //constructor automatically chooses right strings
        node(node_ptr l, node_ptr r, token_types tt);

        bool has_default_string()
        {
            return !(token_type == VARIABLE
                     /*|| token_type == FUNCTION*/
                     || token_type == PREDICATE);
        }
    };

    //the only field
    std::shared_ptr<node> root;

    predicate_ast(std::shared_ptr<node> root)
            : root(root)
    {};

    bool is_var_free_rec(std::string const& var, std::shared_ptr<node> const& cur_node);
    void tree_walk(std::set<std::string>& list, std::shared_ptr<node> const& cur_node);
    void rec_to_string(std::shared_ptr<node> const& cur_node, std::string& res, uint8_t prev_prec, size_t pos);
};


#endif //PREDICATECALCULUSPROOFPROCESSER_PREDICATE_AST_H
