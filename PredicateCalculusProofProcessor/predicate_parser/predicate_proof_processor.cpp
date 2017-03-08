//
// Created by Aleksandr Tukallo on 18.02.17.
//

#include <iostream>
#include "predicate_proof_processor.h"
#include "axioms.h"

predicate_proof_processor::predicate_proof_processor(std::string const& input, std::string output)
{
    output_filename = output;

    //todo change freopen to FILE variables
    std::freopen(input.c_str(), "r", stdin);

    std::string line; //title
    getline(std::cin, line);

    process_title(line);

    //read all old_lines
    while (!feof(stdin))
    {
        getline(std::cin, line);
        if (line != "")
        {
            old_lines.push_back(parser(line).parse());
        }
    }
}

//fills old_hypotheses and old_to_prove
void predicate_proof_processor::process_title(std::string title)
{
    size_t initial_title_size = title.length();
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
    std::set<std::string> free_vars_in_last_hypo;

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
std::vector<std::string> get_scheme_ax_lines(std::string A, std::string B)
{
    A = "(" + A + ")";
    B = "(" + B + ")";
    return {B,
            B + "->" + A + "->" + B,
            A + "->" + B};
}

//proves A->A, where A is hypo in deduction theorem
std::vector<std::string> get_hypo_lines(std::string A)
{
    A = "(" + A + ")";
    return {"(" + A + "->" + "(" + A + "->" + A + "))->(" + A + "->(" + A + "->" + A + ")->" + A + ")->" + A + "->" + A,
            A + "->" + A + "->" + A,
            "(" + A + "->(" + A + "->" + A + ")->" + A + ")->" + A + "->" + A,
            A + "->(" + A + "->" + A + ")->" + A,
            A + "->" + A};
}

//proves A->C, where we already know, that A->B and A->B->C
std::vector<std::string> get_mp_lines(std::string A, std::string B, std::string C)
{
    A = "(" + A + ")";
    B = "(" + B + ")";
    C = "(" + C + ")";
    return {"(" + A + "->" + B + ")->(" + A + "->" + B + "->" + C + ")->(" + A + "->" + C + ")",
            "(" + A + "->" + B + "->" + C + ")->(" + A + "->" + C + ")",
            A + "->" + C};
}

std::vector<std::string> get_second_rule_lines(std::string A, std::string C, std::string D)
{
    //todo think about it
}

//function rebuilds the proof and outputs it or error message to output file
void predicate_proof_processor::process()
{
    axioms::assemble_axioms_if_needed();

//    for (size_t i = 0; i < old_lines.size(); i++)
//    {
//        axioms::axiom_check_result res = axioms::is_an_axiom(old_lines[i]);
//        std::cout << "\n";
//    }

    //move hypotheses to new proof
    for (int w = 0; w < (int) old_hypotheses.size() - 1; w++)
    {
        new_hypotheses.push_back(old_hypotheses[w]);
    }

    //fills last hypo and lines if no last hypo, also fills to_prove
    get_last_hypo();

    //supply quick access to line number from ast
    for (int i = 0; i < old_hypotheses.size(); i++)
    {
        old_hypotheses_map.insert({old_hypotheses[i].to_string(), i + 1});
    }

    for (size_t w = 0; w < old_lines.size(); w++)
    {

        pos = w;
//        new_proove(); //this code does nothing?????

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
                goto cont;
            }
            if (c.finded_ax < -1)
            {
                poss_error = "терм " + predicate_ast(c.term).to_string() + " не свободен для подстановки в формулу ";
                poss_error += predicate_ast(c.formula).to_string() + " вместо переменной ";
                poss_error += c.var + ".";
            }

            ///---------------------------------------------
            it = old_hypotheses_map.find(cur_str);
            if (it != old_hypotheses_map.end())
            {
                if ((*it).second == old_hypotheses.size())
                {
                    concat_vectors(get_hypo_lines(last_hypo->to_string()));
                } else
                {
                    concat_vectors(get_scheme_ax_lines(last_hypo->to_string(), old_lines[w].to_string()));
                }
                goto cont;
            }
            ///---------------------------------------------
            std::map<std::string, std::pair<int, int> >::iterator it3;

            it3 = poss_m_p.find(cur_str);
            if (it3 != poss_m_p.end())
            {
//                is_MP((*it3).second.first, (*it3).second.second);
                get_mp_lines(last_hypo->to_string(),
                             old_lines[(*it3).second.first].to_string(),
                             old_lines[(*it3).second.second].to_string());
                goto cont;
            }
            ///---------------------------------------------

//            pred_rules_res res = check_if_it_new_pred_rule(proofs[w]);
//            if (res.res > 0) {
//                if ((!is_deduction) || (free_vars_in_h_a.find(res.var) == free_vars_in_h_a.end())) {
//
//                    if (res.res == 1) {
//                        is_2_rule(res.finded_no, proofs[w]);
//                    } else {
//                        is_3_rule(res.finded_no, proofs[w]);
//                    }
//                    goto cont;
//                } else {
//                    if (poss_error.empty()) {
//                        poss_error = "используется правило с квантором по переменной ";
//                        poss_error += res.var + ", входящей свободно в допущение ";
//                        poss_error += m_to_string(highlighted_assumption) + ".";
//                    }
//                }
//            } else {
//                if ((res.res < 0) && (poss_error.empty())) {
//                    poss_error = string("переменная ") + res.var;
//                    poss_error += string(" входит свободно в формулу ");
//                    poss_error += m_to_string(res.formula) + ".";
//                }
//            }
//            ///---------------------------------------------
//
//            if (proofs[w]->val == CONSEQUENCE) {
//                expr_sp c = proofs[w], v, ax;
//                string var;
//
//                if ((c->a[0]->val == CONJUNCTION) && (c->a[0]->a[1]->val == FOR_ALL)) {
//                    v = c->a[1];
//                    ax = to_expr("(A&@x(C->B))->C");
//                    var = c->a[0]->a[1]->a[0]->val;
//
//                    map<string, expr_sp> disp_1 = {{var, to_therm("0")}};
//                    map<string, expr_sp> disp_2 = {{var, to_therm(var + "'")}};
//
//                    map<string, expr_sp> disp = { {"A", substitute(v, disp_1)},
//                                                  {"B", substitute(v, disp_2)},
//                                                  {"C", v},
//                                                  {"x", to_therm(var)} };
//                    expr_sp res = substitute(ax, disp);
//
//                    if (m_to_string(res) == m_to_string(c)) {
//                        is_scheme_of_ax(20, c);
//                        goto cont;
//                    }
//                }
//            }
//
//            ///-----------------------
//
//            if (is_not_proved(poss_error)) {
//                return;
//            }
        }
        cont:
//
//        if (proofs[w]->val == CONSEQUENCE) {
//            all_consequences[m_to_string(proofs[w])] = w;
//
//            string left_child = m_to_string(proofs[w]->a[0]);
//
//            it = existing_proofs.find(left_child);
//            if (it != existing_proofs.end()) {
//                poss_m_p[ m_to_string(proofs[w]->a[1]) ] = {(*it).second, w};
//            } else {
//                poss_poss_m_p.insert({left_child,
//                                      {m_to_string(proofs[w]->a[1]), w}
//                                     } );
//            }
//        }
//
//        it2 = poss_poss_m_p.find(str);
//        while (it2 != poss_poss_m_p.end()) {
//            poss_m_p.insert({(*it2).second.first, {w, (*it2).second.second} });
//
//            poss_poss_m_p.erase(it2);
//            it2 = poss_poss_m_p.find(str);
//        }
//        existing_proofs.insert({str, w});
        ;
    }

    //tmp
    std::freopen("tests/output.txt", "w", stdout);
    for (size_t i = 0; i < new_lines.size(); i++)
    {
        std::cout << new_lines[i].to_string() << std::endl;
    }

    std::cout << "finished\n";
}