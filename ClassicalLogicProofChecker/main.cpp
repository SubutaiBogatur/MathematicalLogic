#include <iostream>
#include "classical_logic_proof_checker.h"
#include <ctime>

int main()
{

//    std::clock_t start;
//    double duration;
//    start = std::clock();

    classical_logic_proof_checker checker("../tests/samples/good6.in", "../tests/output.txt");
    checker.check_proof();

    //strings below are needed to see duration of program working in terminal
//    std::freopen("/dev/tty", "w", stdout);
//    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
//    std::cout << "\nFinished in " << duration << std::endl;

    return 0;
}