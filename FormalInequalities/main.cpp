#include <iostream>
#include "prover/prover.h"

int main()
{
    std::freopen("tests/input.txt", "r", stdin);
    int a, b;
    std::cin >> a >> b;
    int c = b - a;

    if (c >= 0)
    {
        prove_less(a, b, "tests/output.txt");
    } else
    {
        prove_greater(a, b, "tests/output.txt");
    }
    return 0;
}