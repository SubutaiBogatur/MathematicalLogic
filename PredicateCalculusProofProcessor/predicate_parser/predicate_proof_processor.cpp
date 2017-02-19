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
        hypotheses.push_back(hypo.parse());
        j += hypo.get_length_of_parsed();
        if (title[j] == ',') j++; //whitespaces already skipped
        all_hypos = title.substr(j, initial_size - j);
    }

    std::string to_prove_str = title.substr(i + 2, title.length() - all_hypos.length() - 2);
    this->to_prove = parser(to_prove_str).parse();
}

//function rebuilds the proof and outputs it or error message to output file
void predicate_proof_processor::process()
{
    axioms::assemble_axioms_if_needed();

    //todo tmp
    for (size_t i = 0; i < lines.size(); i++)
    {
        std::cout << static_cast<int>(axioms::is_an_axiom(lines[i])) << " " << std::endl;
    }
}