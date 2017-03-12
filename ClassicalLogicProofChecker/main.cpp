#include <iostream>
#include "classical_logic_proof_checker.h"

int main()
{
    classical_logic_proof_checker checker("../tests/input.txt", "../tests/output.txt");
    checker.check_proof();
    return 0;
}
