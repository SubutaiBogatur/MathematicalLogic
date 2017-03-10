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
    std::vector<predicate_ast> old_hypotheses;
    std::vector<predicate_ast> old_lines;
    std::experimental::optional<predicate_ast> old_to_prove; //optional because of lack of empty constructor

    std::vector<predicate_ast> new_hypotheses;
    std::vector<predicate_ast> new_lines;
    std::experimental::optional<predicate_ast> new_to_prove;

    //hypothesis to use in deduction theorem
    std::experimental::optional<predicate_ast> last_hypo;

    //{expr.to_string(), line_number}
    std::map<std::string, int> old_hypotheses_map;

    std::map<std::string, std::pair<int, int> > poss_m_p;

    std::set<std::string> free_vars_in_last_hypo;


    std::map<std::string, size_t> all_consequences;

    std::map<std::string, int> existing_proofs;
    std::multimap<std::string, std::pair<std::string, int> > poss_poss_m_p;

    size_t pos;

    std::vector<std::string> get_scheme_ax_lines(std::string A, std::string B);
    std::vector<std::string> get_hypo_lines(std::string A);
    std::vector<std::string> get_mp_lines(std::string A, std::string B, std::string C);
    std::vector<std::string>
    get_predicate_rule_lines(bool first_rule_needed, std::string A, std::string B, std::string C, std::string var);

    void concat_vectors(std::vector<std::string> const&);
    void process_title(std::string);
    void get_last_hypo();
    void print_output(bool error_happened, std::string error);
};


#endif //PREDICATECALCULUSPROOFPROCESSER_PREDICATE_PROOF_PROCESSOR_H
