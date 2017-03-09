//
// Created by Aleksandr Tukallo on 09.03.17.
//

#include <fstream>
#include "prover.h"

std::string replace_all(std::string subject, const std::string& search, const std::string& replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos)
    {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

void prove_less(int a, int b, std::string output)
{
    std::ofstream ofs(output);

    int c = b - a;
    std::string A = "0";
    std::string B = "0";
    std::string C = "0";
    for (int i = 0; i < b; i++)
    {
        if (i < c)
        {
            C += "'";
        }
        if (i < a)
        {
            A += "'";
        }
        B += "'";
    }

    std::string s = "";
    ofs << "|-?p(" + A + "+p)=0" + B << std::endl;

    std::ifstream ifs_base("prover/parts_of_proof/Base2.txt");
    while (ifs_base.good())
    {
        std::string str;
        std::getline(ifs_base, str);
        ofs << str << std::endl;
    }

    ofs << "@a(a+0=a)->" + A + "+0=" + A << std::endl;
    ofs << A + "+0=" + A << std::endl;

    std::string prooff = "";
    std::ifstream ifs_proof("prover/parts_of_proof/Proof1.txt");

    while (ifs_proof.good())
    {
        std::string str;
        std::getline(ifs_proof, str);
        prooff += replace_all(str, "A", A) + "\n";
    }

    std::string sh = "";

    for (int i = 0; i < c; i++)
    {
        ofs << replace_all(prooff, "#", sh);
        sh += "'";
    }
    ofs << "(" + A + "+" + C + ")=" + B + "->?p(" + A + "+p)=" + B + "\n" + "?p(" + A + "+p)=" + B;
}