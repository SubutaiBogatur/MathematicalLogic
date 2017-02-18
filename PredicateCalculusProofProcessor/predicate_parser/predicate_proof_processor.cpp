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

    //read all lines
    while (!feof(stdin))
    {
        getline(std::cin, line);
        if (line != "")
        {
            lines.push_back(parser(line).parse());
        }
    }
}

//fills hypotheses and to_prove
void predicate_proof_processor::process_title(std::string const& title)
{
    std::string cur_hypothesis;
    size_t i = 0;
    while (!(title[i] == '|' && title[i + 1] == '-'))
    {
        if (title[i] == ',')
        {
            parser hypo(cur_hypothesis);
            hypotheses.push_back(hypo.parse());
            cur_hypothesis = "";
        } else
        {
            cur_hypothesis.push_back(title[i]);
        }
        i++;
    }
    if (cur_hypothesis != "")
    {
        hypotheses.push_back(parser(cur_hypothesis).parse());
    }
    i += 2;

    std::string to_prove_str;
    for (; i < title.size(); i++)
    {
        to_prove_str.push_back(title[i]);
    }
    this->to_prove = parser(to_prove_str).parse();
}

//function rebuilds the proof and outputs it or error message to output file
void predicate_proof_processor::process()
{
    axioms::assemble_axioms_if_needed();
}