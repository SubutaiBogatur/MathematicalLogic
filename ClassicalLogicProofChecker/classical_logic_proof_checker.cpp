//
// Created by Aleksandr Tukallo on 22.10.16.
//

#include "classical_logic_proof_checker.h"
#include <iostream>

//public:

//inserts tree in mp only if needed
static void mp_insert_if_needed(ast& tree, std::unordered_map<ast, std::vector<ast>>& mp, int32_t& line_num)
{
    if (tree.is_implication_first())
    {
        //adding to mp if not added already
        auto it_mp = mp.find(tree.root->right);
        ast to_push(tree.root->left);
        to_push.line_num = line_num;
        if (it_mp != mp.end())
        {
            //if already inserted, increase vector
            (*it_mp).second.push_back(to_push);
        } else
        {
            //push new vector in map
            mp.insert({ast(tree.root->right), std::vector<ast>(1, to_push)});
        }
    }
}

static bool value_contains_proved_tree(std::vector<ast>& vect, std::unordered_set<ast>& proved,
                                       std::pair<int32_t, int32_t>& line_nums_mp)
{
    for (size_t i = 0; i < vect.size(); i++)
    {
        auto it = proved.find(vect[i]);
        if (it != proved.end())
        {
            line_nums_mp.first = (*it).line_num;
            line_nums_mp.second = vect[i].line_num;
            return true;
        }
    }
    return false;
}

void print_with_no_whitespaces(std::string& str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == ' ')
            continue;

        std::cout << str[i];
    }
}

void classical_logic_proof_checker::check_proof()
{
//    UTF8?
//    std::setlocale(LC_ALL, "en_US.UTF-8");

    {
        //building axioms to access them later as static field
        axioms axioms;
    }
    //making cin and cout faster
    std::ios_base::sync_with_stdio(false);

    FILE *in_stream = std::freopen(input_file_name.c_str(), "r", stdin);
    FILE *out_stream = std::freopen(output_file_name.c_str(), "w", stdout);
    if (in_stream == NULL || out_stream == NULL)
    {
        throw std::invalid_argument("Incorrect input or output file's name");
    }

    std::unordered_set<ast> proved;
    std::unordered_map<ast, std::vector<ast>> mp;

    analyze_title(proved, mp);

    std::string line_with_expression;

    int32_t global_counter = 0;
    while (std::getline(std::cin, line_with_expression))
    {
        global_counter++;
        std::cout << "(" << global_counter << ") ";
        print_with_no_whitespaces(line_with_expression);

        ast tree(line_with_expression);

        uint16_t ax = tree.is_an_axiom();
        if (ax != 0)
        {
            mp_insert_if_needed(tree, mp, global_counter);
            tree.line_num = global_counter;
            proved.insert(tree);
            std::cout << " (Сх. акс. " << ax << ")" << std::endl;
        } else
        {
            auto it_pr = proved.find(tree);
            if (it_pr != proved.end())
            {
                //string was already proved, we have to check if it was a hypothesis
                //if hypo:
                if ((*it_pr).line_num < 0)
                {
                    std::cout << " (Предп. " << std::abs((*it_pr).line_num) << ")" << std::endl;
                    continue;
                } else
                {
//                    We don't write annotations in case, if it was proved earlier
//                    std::cout << " (Доказано в стр. " << (*it_pr).line_num << ")" << std::endl;
                }
//                continue;
            }
            //if not a hypothesis
            auto it_mp = mp.find(tree); //let's see if it can be proved by mp already
            if (it_mp == mp.end())
            {
                //can not be proved
                std::cout << " (Не доказано)" << std::endl;
                continue;
            }
            //else
            std::pair<int32_t, int32_t> line_numbers_mp;
            if (value_contains_proved_tree((*it_mp).second, proved, line_numbers_mp))
            {
                mp_insert_if_needed(tree, mp, global_counter);
                tree.line_num = global_counter;
                proved.insert(tree);

                //let's check if some of mp participants are hypotheses
                if (line_numbers_mp.first < 0 && line_numbers_mp.second < 0)
                {
                    std::cout << " (M.P. предп. " << std::abs(line_numbers_mp.first) << ", предп. "
                              << std::abs(line_numbers_mp.second) << ")" << std::endl;
                } else if (line_numbers_mp.first < 0)
                {
                    std::cout << " (M.P. предп. " << std::abs(line_numbers_mp.first) << ", " << line_numbers_mp.second
                              << ")" << std::endl;
                } else if (line_numbers_mp.second < 0)
                {
                    std::cout << " (M.P. " << line_numbers_mp.first << ", предп. " << std::abs(line_numbers_mp.second)
                              << ")" << std::endl;
                } else
                {
                    std::cout << " (M.P. " << line_numbers_mp.first << ", " << line_numbers_mp.second << ")"
                              << std::endl;
                }
            } else
            {
                std::cout << " (Не доказано)" << std::endl;
            }
        }
    }
}

//private:

//function adds hypotheses to proved and also to mp
void classical_logic_proof_checker::analyze_title(std::unordered_set<ast>& proved, std::unordered_map<ast, std::vector<ast>>& mp)
{
    std::string title;
    std::getline(std::cin, title);
    std::cout << title << std::endl;

    std::string hypo;
    int32_t hypo_counter = -1;
    for (size_t i = 0; i < title.size(); i++)
    {
        if (((i + 1 < title.size() && title[i] == '|' && title[i + 1] == '-') || title[i] == ',') && i != 0)
        {
            //if end of current hypothesis expression
            ast hypo_tree(hypo);
            hypo_tree.line_num = hypo_counter;
            mp_insert_if_needed(hypo_tree, mp, hypo_counter);
            proved.insert(hypo_tree);
            hypo.clear();
            hypo_counter--;
        } else
        {
            hypo.push_back(title[i]);
        }

        if (i + 1 < title.size() && title[i] == '|' && title[i + 1] == '-')
        {
            return; //seems like we don't need the expression to prove at all
//            i++;
        }
    }
}