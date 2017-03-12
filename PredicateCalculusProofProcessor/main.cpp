#include "predicate_parser/predicate_proof_processor.h"

int main()
{
    predicate_proof_processor p("../tests/input.in", "../tests/output.out");
    p.process();
    return 0;
}
