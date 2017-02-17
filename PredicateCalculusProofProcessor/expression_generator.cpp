//
// Created by Aleksandr Tukallo on 17.02.17.
//

#include "expression_generator.h"
#include <array>
#include <assert.h>
#include <iostream>

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

// Constants for random number generators:
const std::array<size_t, 2> expr = {5, 1};
const std::array<size_t, 2> disj = {5, 1};
const std::array<size_t, 2> conj = {5, 1};
const std::array<size_t, 4> unar = {5, 1, 1, 1};
//const std::array<size_t, 1> vari = {1};
const std::array<size_t, 2> pred = {1, 5};
const std::array<size_t, 2> term = {5, 1};
const std::array<size_t, 2> summ = {5, 1};
const std::array<size_t, 5> mult = {1, 5, 1, 1, 1};

const size_t max_func_name = 3;
const size_t max_pred_name = 3;

const size_t max_argu_numb = 3;

const size_t alphabet_size = 26;

template<size_t N>
size_t get_random_index(const std::array<size_t, N>& arr)
{
    int sum = 0;
    for (size_t i = 0; i < arr.size(); i++)
    {
        sum += arr[i];
    }

    int random_num = std::rand() % sum;
    for (size_t i = 0; i < arr.size(); i++)
    {
        random_num -= arr[i];
        if (random_num < 0)
        {
            return i;
        }
    }
    assert (false);
    return 0;
}

static std::string get_expr();
static std::string get_disj();
static std::string get_conj();
static std::string get_unar();
static std::string get_vari();
static std::string get_pred();
static std::string get_term();
static std::string get_summ();
static std::string get_mult();

std::string remove_odd_spaces(std::string const& old)
{
    std::string ret;
    size_t space_cnt = 0;
    for (size_t i = 0; i < old.size(); i++)
    {
        if (old[i] == ' ') space_cnt++;
        else space_cnt = 0;
        if (space_cnt <= 1) ret.push_back(old[i]);
    }
    return ret;
}

size_t expression_generator::get_seed()
{
    return seed;
}

std::string expression_generator::get_random_expression()
{
    std::srand(static_cast<uint>(std::time(0) + seed));
    seed++;
    return remove_odd_spaces(get_expr());
}

static std::string get_expr()
{
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    switch (get_random_index(expr))
    {
        case 0:
            return get_disj() + " ";
        case 1:
            return get_disj() + "-> " + get_expr() + " ";
        default:break;
    }
    assert (false);
}

static std::string get_disj()
{
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    switch (get_random_index(disj))
    {
        case 0:
            return get_conj() + " ";
        case 1:
            return get_disj() + "| " + get_conj() + " ";
        default:break;
    }
    assert (false);
}

static std::string get_conj()
{
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    switch (get_random_index(conj))
    {
        case 0:
            return get_unar() + " ";
        case 1:
            return get_conj() + "& " + get_unar() + " ";
        default:break;
    }
    assert (false);
}

static std::string get_unar()
{
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    switch (get_random_index(unar))
    {
        case 0:
            return get_pred() + " ";
        case 1:
            return "!" + get_unar() + " ";
        case 2:
            return "(" + get_expr() + ")";
        case 3:
            return (std::rand() % 2 ? "@" : "?") + get_vari() + get_unar() + " ";
        default:break;
    }
    assert (false);
}

static std::string get_vari_name()
{
    std::string ret;
    size_t len = std::rand() % (max_func_name - 1) + 1;
    ret.push_back(static_cast<char>(std::rand() % alphabet_size + 'a'));
    for (size_t i = 1; i < len; i++)
    {
        ret.push_back(static_cast<char>(std::rand() % 10 + '0'));
    }
    return ret + " ";
}

static std::string get_vari()
{
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return get_vari_name();
}

static std::string get_pred_name()
{
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::string ret;
    size_t len = std::rand() % (max_pred_name - 1) + 1;
    ret.push_back(static_cast<char>(std::rand() % alphabet_size + 'A'));
    for (size_t i = 1; i < len; i++)
    {
        ret.push_back(static_cast<char>(std::rand() % 10 + '0'));
    }
    return ret + " ";
}

//if flag is false, empty arguments can be returned
static std::string get_arguments(bool flag)
{
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::string ret;
    size_t num = std::rand() % (max_argu_numb - flag) + flag;

    if (num > 0)
    {
        ret += "(";
        ret += get_term();
        for (size_t i = 1; i < num; i++)
        {
            ret += "," + get_term();
        }
        ret += ")";
    }
    return ret + " ";
}

static std::string get_pred()
{
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    switch (get_random_index(pred))
    {
        case 0:
            return get_pred_name() + get_arguments(false) + " ";
        case 1:
            return get_term() + "= " + get_term() + " ";
        default:break;
    }
    assert (false);
}

static std::string get_term()
{
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    switch (get_random_index(term))
    {
        case 0:
            return get_summ() + " ";
        case 1:
            return get_term() + "+ " + get_summ() + " ";
        default:break;
    }
    assert (false);
}

static std::string get_summ()
{
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    switch (get_random_index(summ))
    {
        case 0:
            return get_mult() + " ";
        case 1:
            return get_summ() + "* " + get_mult() + " ";
        default:break;
    }
    assert (false);
}

static std::string get_mult()
{
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    switch (get_random_index(mult))
    {
        case 0:
            return get_vari_name() + get_arguments(true) + " ";
        case 1:
            return get_vari() + " ";
        case 2:
            return "(" + get_term() + ") ";
        case 3:
            return "0 ";
        case 4:
            return get_mult() + "'";
        default:break;
    }
    assert (false);
}