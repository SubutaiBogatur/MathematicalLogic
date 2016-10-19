//
// Created by aleksandr on 14.10.16.
//

#include "postfix_string_builder.h"

void postfix_string_builder::build_postfix_notation_string()
{
    expr_postfx();
}

void postfix_string_builder::expr_postfx()
{
    skip_whitespace();

    factor_postfx();

    while (!is_end_of_string())
    {
        binary_postfx();
        factor_postfx();
    }

    while (operand_stack.size() > 0)
    {
        if (operand_stack.top() == ')' || operand_stack.top() == '(')
            throw parser_exception("no closing parenthesis in infix form expected");

        concat_output_with_argument(operand_stack.top());
        operand_stack.pop();
    }
}

void postfix_string_builder::expr_inside_parenthesis_postfx()
{
    skip_whitespace();

    factor_postfx();

    while (get_next_token(false) != close_parenthesis)
    {
        binary_postfx();
        factor_postfx();
    }

}

void postfix_string_builder::factor_postfx()
{
    skip_whitespace();

    token_types cur_token = get_next_token(true);
    switch (cur_token)
    {
        case open_parenthesis:
            operand_stack.push(open_parenthesis);
            expr_inside_parenthesis_postfx();

            if (get_next_token(true) != close_parenthesis)
                throw parser_exception("expected closing parenthesis");
            else
            {
                while (operand_stack.top() != open_parenthesis)
                {
                    concat_output_with_argument(operand_stack.top());
                    operand_stack.pop();
                }
                operand_stack.pop();
            }

            break;
        case var:
            output_string.append(" " + get_var_name());
            break;
        case neg:
            //no check for stack, 'cause neg has the highest precedence
            operand_stack.push(neg);
            factor_postfx();
            break;
        default:
            throw parser_exception("unexpected factor token");
    }
    return;
}

void postfix_string_builder::binary_postfx()
{
    skip_whitespace();

    token_types cur_token = get_next_token(true);

    while (is_preced_stack_head_bigger(cur_token))
    {
        token_types head = operand_stack.top();
        operand_stack.pop();

        concat_output_with_argument(head);
    }
    operand_stack.push(cur_token);
    return;
}

//post: moves cuurent_index to the next symbol after the end of the token
token_types postfix_string_builder::get_next_token(bool move_index)
{
    skip_whitespace();

    if (is_end_of_string())
        throw parser_exception("expected token");

    if (expression[current_index] == '(')
    {
        if (move_index)current_index++;
        return open_parenthesis;
    }
    else if (expression[current_index] == ')')
    {
        if (move_index)current_index++;
        return close_parenthesis;
    }
    else if (expression[current_index] == '&')
    {
        if (move_index)current_index++;
        return conj;
    }
    else if (expression[current_index] == '|')
    {
        if (move_index)current_index++;
        return disj;
    }
    else if (expression[current_index] == '!')
    {
        if (move_index)current_index++;
        return neg;
    }
    else if (current_index + 1 < expression.size() && expression[current_index] == '-' &&
             expression[current_index + 1] == '>')
    {
        if (move_index)current_index += 2;
        return impl;
    }
    else if (expression[current_index] >= 'A' && expression[current_index] <= 'Z')
    {
        return var;
    }

    throw parser_exception("unexpected token");
}

void postfix_string_builder::concat_output_with_argument(token_types app)
{
    switch (app)
    {
        case conj:
            output_string.append(" &");
            break;
        case disj:
            output_string.append(" |");
            break;
        case impl:
            output_string.append(" ->");
            break;
        case neg:
            output_string.append(" !");
            break;
        default:
            throw parser_exception("unexpected binary token");
    }
    return;
}

bool postfix_string_builder::is_preced_stack_head_bigger(token_types new_token)
{
    if (operand_stack.size() == 0)
        return false;

    //if right associative
    if (new_token == impl)
    {
        //if operand on head of stack has bigger precedence, than new operand
        if ((*operand_precedence.find(operand_stack.top())).second > (*operand_precedence.find(new_token)).second)
        {
            return true;
        }
        else
            return false;
    } else
    {
        //same, but less or equal for left-associative operations
        if ((*operand_precedence.find(operand_stack.top())).second >= (*operand_precedence.find(new_token)).second)
        {
            return true;
        }
        else return false;
    }
}

bool postfix_string_builder::is_end_of_string()
{
    skip_whitespace();
    return expression.size() == current_index;
}

void postfix_string_builder::skip_whitespace()
{
    while (expression[current_index] == ' ')
        current_index++;
    return;
}

//pre: function is called, when expression[current_index] = 'A'...'Z'
std::string postfix_string_builder::get_var_name()
{
    std::string name;
    name.push_back(expression[current_index]);
    current_index++;

    while ((expression[current_index] >= '0' && expression[current_index] <= '9')
           || (expression[current_index] >= 'A' && expression[current_index] <= 'Z'))
    {
        name.push_back(expression[current_index]);
        current_index++;
    }
    return name;
}