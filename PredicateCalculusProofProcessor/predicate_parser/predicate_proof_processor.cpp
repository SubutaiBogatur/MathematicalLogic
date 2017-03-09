//
// Created by Aleksandr Tukallo on 18.02.17.
//

#include <iostream>
#include <fstream>
#include "predicate_proof_processor.h"
#include "axioms.h"

predicate_proof_processor::predicate_proof_processor(std::string const& input, std::string output)
{
    output_filename = output;

    std::ifstream ifs(input.c_str());

    std::string line; //title
    getline(ifs, line);

    process_title(line);

    //read all old_lines
    while (ifs.good())
    {
        getline(ifs, line);
        if (line != "")
        {
            old_lines.push_back(parser(line).parse());
        }
    }
}

//fills old_hypotheses and old_to_prove
void predicate_proof_processor::process_title(std::string title)
{
    size_t i = 0;

    std::string all_hypos;

    while (!(title[i] == '|' && title[i + 1] == '-'))
    {
        all_hypos.push_back(title[i]);
        i++;
    }

    size_t j = 0;
    size_t initial_size = all_hypos.size();
    while (j < initial_size)
    {
        parser hypo(all_hypos);
        old_hypotheses.push_back(hypo.parse());
        j += hypo.get_length_of_parsed();
        if (title[j] == ',') j++; //whitespaces already skipped
        all_hypos = title.substr(j, initial_size - j);
    }

    std::string to_prove_str = title.substr(i + 2, title.length() - all_hypos.length() - 2);
    this->old_to_prove = parser(to_prove_str).parse();
}

void predicate_proof_processor::get_last_hypo()
{
    //if initially there were more than 0 hypos
    //  last hypo is the one to use in deduction theorem
    if (old_hypotheses.size() > 0)
    {
        last_hypo = old_hypotheses[old_hypotheses.size() - 1];
        free_vars_in_last_hypo = last_hypo->get_all_free_vars();

        //hypo->need_to_prove
        new_to_prove = predicate_ast(
                std::make_shared<predicate_ast::node>(last_hypo->root, old_to_prove->root, token_types::IMPLICATION));
    } else
    {
        //last hypo remains uninitialized
        new_lines = old_lines;
        new_to_prove = old_to_prove;
    }
}

//concats new lines vector with vectors of strings
void predicate_proof_processor::concat_vectors(std::vector<std::string> const& strs)
{
    for (size_t i = 0; i < strs.size(); i++)
    {
        new_lines.push_back(parser(strs[i]).parse());
    }
}

//proves A->B, where A is selected hypo and B is axiom or hypo
//A is selected hypothesis, B is expr which is axiom
std::vector<std::string> predicate_proof_processor::get_scheme_ax_lines(std::string A, std::string B)
{
    std::ifstream ifs("predicate_parser/proof_parts/axiom_or_hypo.txt");
    std::vector<std::string> ret;

    std::string cur_line;
    while (ifs.good())
    {
        std::getline(ifs, cur_line);
        std::string substituted;
        for (size_t i = 0; i < cur_line.size(); i++)
        {
            if (cur_line[i] == 'A')
            {
                substituted += "(" + A + ")";
            } else if (cur_line[i] == 'B')
            {
                substituted += "(" + B + ")";
            } else
            {
                substituted += cur_line[i];
            }
        }
        ret.push_back(substituted);
    }
    return ret;
}

//proves A->A, where A is hypo in deduction theorem
std::vector<std::string> predicate_proof_processor::get_hypo_lines(std::string A)
{
    std::ifstream ifs("predicate_parser/proof_parts/last_hypo.txt");
    std::vector<std::string> ret;

    std::string cur_line;
    while (ifs.good())
    {
        std::getline(ifs, cur_line);
        std::string substituted;
        for (size_t i = 0; i < cur_line.size(); i++)
        {
            if (cur_line[i] == 'A')
            {
                substituted += "(" + A + ")";
            } else
            {
                substituted += cur_line[i];
            }
        }
        ret.push_back(substituted);
    }
    return ret;
}

//proves A->C, where we already know, that A->B and A->B->C
std::vector<std::string> predicate_proof_processor::get_mp_lines(std::string A, std::string B, std::string C)
{
    std::ifstream ifs("predicate_parser/proof_parts/mp.txt");
    std::vector<std::string> ret;

    std::string cur_line;
    while (ifs.good())
    {
        std::getline(ifs, cur_line);
        std::string substituted;
        for (size_t i = 0; i < cur_line.size(); i++)
        {
            if (cur_line[i] == 'A')
            {
                substituted += "(" + A + ")";
            } else if (cur_line[i] == 'B')
            {
                substituted += "(" + B + ")";
            } else if (cur_line[i] == 'C')
            {
                substituted += "(" + C + ")";
            } else
            {
                substituted += cur_line[i];
            }
        }
        ret.push_back(substituted);
    }
    return ret;
}

std::vector<std::string>
predicate_proof_processor::get_predicate_rule_lines(bool first_rule_needed, std::string A, std::string B, std::string C,
                                                    std::string x)
{
    std::ifstream ifs;
    if (first_rule_needed)
    {
        ifs = std::ifstream("predicate_parser/proof_parts/first_predicate_rule.txt");
    } else
    {
        ifs = std::ifstream("predicate_parser/proof_parts/second_predicate_rule.txt");
    }


    std::vector<std::string> ret;

    std::string cur_line;
    std::getline(ifs, cur_line); //read header

    while (ifs.good())
    {
        std::getline(ifs, cur_line);
        std::string substituted;
        for (size_t i = 0; i < cur_line.size(); i++)
        {
            if (cur_line[i] == 'A')
            {
                substituted += "(" + A + ")";
            } else if (cur_line[i] == 'B')
            {
                substituted += "(" + B + ")";
            } else if (cur_line[i] == 'C')
            {
                substituted += "(" + C + ")";
            } else if (cur_line[i] == 'x')
            {
                substituted += x;
            } else
            {
                substituted += cur_line[i];
            }
        }
        ret.push_back(substituted);
    }
    return ret;
}

//function rebuilds the proof and outputs it or error message to output file
void predicate_proof_processor::process()
{
    axioms::assemble_axioms_if_needed();

    //move hypotheses to new proof
    for (int w = 0; w < (int) old_hypotheses.size() - 1; w++)
    {
        new_hypotheses.push_back(old_hypotheses[w]);
    }

    //fills last hypo and lines if no last hypo, also fills to_prove
    get_last_hypo();

    //supply quick access to line number from ast
    for (size_t i = 0; i < old_hypotheses.size(); i++)
    {
        old_hypotheses_map.insert({old_hypotheses[i].to_string(), i + 1});
    }

    for (size_t w = 0; w < old_lines.size(); w++)
    {
        pos = w;

        //just convert line to string
        std::string cur_str = old_lines[w].to_string();

        std::map<std::string, int>::iterator it;
        std::multimap<std::string, std::pair<std::string, int>>::iterator it2;

        {
            std::string poss_error;
            axioms::axiom_check_result c = axioms::is_an_axiom(old_lines[w]);

            if (c.finded_ax >= 0)
            {
                if (last_hypo)
                {
                    //add new lines to proof
                    concat_vectors(get_scheme_ax_lines((*last_hypo).to_string(), old_lines[w].to_string()));
                }
            } else if (c.finded_ax < -1)
            {
                poss_error = "терм " + predicate_ast(c.term).to_string() + " не свободен для подстановки в формулу ";
                poss_error += predicate_ast(c.formula).to_string() + " вместо переменной ";
                poss_error += c.var + ".";
            } else
            {

                ///---------------------------------------------
                it = old_hypotheses_map.find(cur_str);
                if (it != old_hypotheses_map.end())
                {
                    if ((*it).second == static_cast<int>(old_hypotheses.size()))
                    {
                        concat_vectors(get_hypo_lines(last_hypo->to_string()));
                    } else
                    {
                        concat_vectors(get_scheme_ax_lines(last_hypo->to_string(), old_lines[w].to_string()));
                    }
                } else
                {
                    ///---------------------------------------------
                    std::map<std::string, std::pair<int, int> >::iterator it3;

                    it3 = poss_m_p.find(cur_str);
                    if (it3 != poss_m_p.end())
                    {
                        concat_vectors(get_mp_lines(last_hypo->to_string(),
                                                    old_lines[(*it3).second.first].to_string(),
                                                    predicate_ast(
                                                            old_lines[(*it3).second.second].root->right).to_string()));
                    } else
                    {
                        ///---------------------------------------------

                        axioms::pred_rules_res res = axioms::check_if_it_new_pred_rule(old_lines[w].root,
                                                                                       all_consequences);
                        if (res.res > 0)
                        {
                            if (this->free_vars_in_last_hypo.find(res.var) == free_vars_in_last_hypo.end())
                            {
                                if (res.res == 1)
                                {
                                    std::shared_ptr<predicate_ast::node> B = old_lines[w].root->left;
                                    std::shared_ptr<predicate_ast::node> C = old_lines[w].root->right->right;
                                    std::string x = old_lines[w].root->right->left->str;
                                    concat_vectors(
                                            get_predicate_rule_lines(true, last_hypo->to_string(),
                                                                     predicate_ast(B).to_string(),
                                                                     predicate_ast(C).to_string(), x));
                                } else
                                {
                                    std::shared_ptr<predicate_ast::node> B = old_lines[w].root->left->right;
                                    std::shared_ptr<predicate_ast::node> C = old_lines[w].root->right;
                                    std::string x = old_lines[w].root->left->left->str;
                                    concat_vectors(
                                            get_predicate_rule_lines(false, last_hypo->to_string(),
                                                                     predicate_ast(B).to_string(),
                                                                     predicate_ast(C).to_string(), x));
                                }
                            } else
                            {
                                if (poss_error.empty())
                                {
                                    poss_error = "используется правило с квантором по переменной ";
                                    poss_error += res.var + ", входящей свободно в допущение ";
                                    poss_error += last_hypo->to_string() + ".";
                                }
                            }
                        } else
                        {
                            if ((res.res < 0) && (poss_error.empty()))
                            {
                                poss_error = std::string("переменная ") + res.var;
                                poss_error += std::string(" входит свободно в формулу ");
                                poss_error += predicate_ast(res.formula).to_string() + ".";
                            }

//            ///---------------------------------------------

                            if (axioms::is_9_math_axiom(old_lines[w]))
                            {
                                std::cerr << "is induction axiom\n";
                                concat_vectors(get_scheme_ax_lines((*last_hypo).to_string(), old_lines[w].to_string()));
                            } else
                            {
                                //else not proved statement, sophism must be punished
                                print_output(true, poss_error);
                                return;
                            }
                        }
                    }
                }
            }
        }

        if (old_lines[w].root->token_type == IMPLICATION)
        {
            all_consequences[old_lines[w].to_string()] = w;

            std::string left_child = predicate_ast(old_lines[w].root->left).to_string();

            it = existing_proofs.find(left_child);
            if (it != existing_proofs.end())
            {
                poss_m_p[predicate_ast(old_lines[w].root->right).to_string()] = {(*it).second, w};
            } else
            {
                poss_poss_m_p.insert({left_child, {predicate_ast(old_lines[w].root->right).to_string(), w}});
            }
        }

        it2 = poss_poss_m_p.find(cur_str);
        while (it2 != poss_poss_m_p.end())
        {
            poss_m_p.insert({(*it2).second.first, {w, (*it2).second.second}});

            poss_poss_m_p.erase(it2);
            it2 = poss_poss_m_p.find(cur_str);
        }
        existing_proofs.insert({cur_str, w});;
    }

    print_output(false, "");
    return;
}

void predicate_proof_processor::print_output(bool error_happened, std::string error)
{
    std::freopen("tests/output.txt", "w", stdout);
    std::ios::sync_with_stdio(false);

    if (error_happened)
    {
        std::cout << "Вывод некорректен начиная с формулы номер ";
        std::cout << pos + 1;

        if (!error.empty())
        {
            std::cout << ": " << error;
        }
        return;
    } else
    {
        //print header
        bool b = 0;
        for (size_t i = 0; i < new_hypotheses.size(); i++)
        {
            if (b)
            {
                std::cout << ",";
            }
            b = 1;
            std::cout << new_hypotheses[i].to_string();
        }
        std::cout << "|-" << new_to_prove->to_string() << "\n";
        //print lines
        for (size_t i = 0; i < new_lines.size(); i++)
        {
            std::cout << new_lines[i].to_string() << std::endl;
        }
    }
}