//
// Created by Aleksandr Tukallo on 22.10.16.
//

#ifndef PROOFCHECKER_PROOF_CHECKER_H
#define PROOFCHECKER_PROOF_CHECKER_H

#include "ast.h"
#include <unordered_set>
#include "axioms.h"
#include "hash.h"

struct classical_logic_proof_checker
{
public:
    classical_logic_proof_checker(std::string const& in, std::string const& out) : input_file_name(in), output_file_name(out)
    { };

    //generates output file
    void check_proof();
private:
    std::string const input_file_name;
    std::string const output_file_name;

    void analyze_title(std::unordered_set<ast>&, std::unordered_map<ast, std::vector<ast>>&);
};


#endif //PROOFCHECKER_PROOF_CHECKER_H
