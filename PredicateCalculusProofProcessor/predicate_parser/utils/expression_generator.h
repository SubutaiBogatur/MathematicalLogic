//
// Created by Aleksandr Tukallo on 17.02.17.
//

#ifndef PREDICATECALCULUSPROOFPROCESSER_EXPRESSION_GENERATOR_H
#define PREDICATECALCULUSPROOFPROCESSER_EXPRESSION_GENERATOR_H

#include <string>
#include <ctime>

struct expression_generator
{
    size_t get_seed();
    std::string get_random_expression();
private:
    size_t seed = static_cast<size_t>(std::time(0));
};


#endif //PREDICATECALCULUSPROOFPROCESSER_EXPRESSION_GENERATOR_H
