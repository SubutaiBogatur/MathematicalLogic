//
// Created by Aleksandr Tukallo on 18.02.17.
//

#ifndef PREDICATECALCULUSPROOFPROCESSER_PREDICATE_PROOF_PROCESSOR_H
#define PREDICATECALCULUSPROOFPROCESSER_PREDICATE_PROOF_PROCESSOR_H

#include <string>
#include <vector>
#include "predicate_ast.h"
#include "parser.h"
#include <experimental/optional>
#include "axioms.h"

struct predicate_proof_processor
{
public:
    predicate_proof_processor() = delete;

    //constructor reads all the needed data to arrays
    //pre:
    //  input file has correct syntax
    predicate_proof_processor(std::string const& input, std::string output);

    //main function, that does all the work
    void process();

    //todo private after debugging
public:
    std::string output_filename;
    std::vector<predicate_ast> hypotheses;
    std::vector<predicate_ast> lines;
    std::experimental::optional<predicate_ast> to_prove; //optional because of lack of empty constructor

    void process_title(std::string);
};


#endif //PREDICATECALCULUSPROOFPROCESSER_PREDICATE_PROOF_PROCESSOR_H
