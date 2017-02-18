//
// Created by Aleksandr Tukallo on 17.02.17.
//

#ifndef PREDICATECALCULUSPROOFPROCESSER_PREDICATE_AST_H
#define PREDICATECALCULUSPROOFPROCESSER_PREDICATE_AST_H

#include <memory>
#include <map>

enum token_types
{
    NEGATION,
    CONJUNCTION,
    DISJUNCTION,
    IMPLICATION,
    ARGUMENTS,

    PREDICATE, //eg P(a, b, c) or P
    VARIABLE, //eg x
    FUNCTION,  //eg f(a, b)

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
    //todo a lot of useful functions

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

        //todo mb some cool constructors
    };

    predicate_ast(std::shared_ptr<node> root)
            : root(root)
    {};

    std::shared_ptr<node> root;
};


#endif //PREDICATECALCULUSPROOFPROCESSER_PREDICATE_AST_H
