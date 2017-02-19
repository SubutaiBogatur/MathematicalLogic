//
// Created by Aleksandr Tukallo on 17.02.17.
//

#ifndef PREDICATECALCULUSPROOFPROCESSER_PREDICATE_AST_H
#define PREDICATECALCULUSPROOFPROCESSER_PREDICATE_AST_H

#include <memory>
#include <map>
#include <vector>
#include <set>
#include <unordered_map>

enum token_types
{
    NEGATION,
    CONJUNCTION,
    DISJUNCTION,
    IMPLICATION,
    ARGUMENTS,

    PREDICATE, //eg P(a, b, c) or P
    VARIABLE, //eg x
//    FUNCTION,  //eg f(a, b) //nb we will treat functions and vars the same. All the funcs are vars from now.

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

const std::map<token_types, uint8_t> precedence{
        {EQUALITY,       0},
        {VARIABLE,       0},
//        {FUNCTION,       0},

        {FOR_ALL,        1},
        {EXISTS,         1},

        {PREDICATE,      2},

        {ZERO,           3},
        {STROKE,         4},
        {MULTIPLICATION, 5},
        {SUM,            6},

        {NEGATION,       8},
        {CONJUNCTION,    9},
        {DISJUNCTION,    10},
        {IMPLICATION,    11},
        {ARGUMENTS,      12},
};

const std::map<token_types, std::string> string_representation{
        {EQUALITY,       "="},

        {FOR_ALL,        "@"},
        {EXISTS,         "?"},

        {ZERO,           "0"},
        {STROKE,         "'"},
        {MULTIPLICATION, "*"},
        {SUM,            "+"},

        {NEGATION,       "!"},
        {CONJUNCTION,    "&"},
        {DISJUNCTION,    "|"},
        {IMPLICATION,    "->"},
        {ARGUMENTS,      "args"},
};

struct predicate_ast
{
    friend struct parser;
    friend struct axioms;
public:
    predicate_ast() = delete;

    std::string to_string() const;
    bool is_var_free(std::string const& var) const;
    std::vector<std::string> get_all_free_vars() const;
    bool equals(predicate_ast const& other) const;

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

        bool has_default_string() const
        {
            return !(token_type == VARIABLE
                     /*|| token_type == FUNCTION*/
                     || token_type == PREDICATE);
        }

        //function doesn't check children!
        bool equals(node const& other) const
        {
            if (this->token_type != other.token_type)
                return false;

            if (this->has_default_string() == false && other.has_default_string() == false)
            {
                if (this->str != other.str)
                    return false;
            }

            return true;
        }

        bool is_leave() const
        {
            return left == NULL && right == NULL;
        }

        std::string to_string()
        {
            if(this->token_type == VARIABLE || this->token_type == PREDICATE)
                return this->str;
            else return string_representation.at(token_type);
        }
    };

    struct ast_exception
    {
    };

    //the only field
    std::shared_ptr<node> root;

    predicate_ast(std::shared_ptr<node> root)
            : root(root)
    {};

    bool is_var_free_rec(std::string const& var, std::shared_ptr<node> const& cur_node) const;
    void tree_walk(std::set<std::string>& list, std::shared_ptr<node> const& cur_node) const;
    void rec_to_string(std::shared_ptr<node> const& cur_node, std::string& res, uint8_t prev_prec, size_t pos) const;
    bool recursive_equals(std::shared_ptr<node> const& this_node, std::shared_ptr<node> const& other_node) const;
};


#endif //PREDICATECALCULUSPROOFPROCESSER_PREDICATE_AST_H
