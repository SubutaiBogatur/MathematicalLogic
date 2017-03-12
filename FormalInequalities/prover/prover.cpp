//
// Created by Aleksandr Tukallo on 09.03.17.
//

#include <fstream>

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
    ofs << "|-?p(" + A + "+p)=" + B << std::endl;

    std::ifstream ifs_base("../prover/parts_of_proof/less_base.txt");
    while (ifs_base.good())
    {
        std::string str;
        std::getline(ifs_base, str);
        ofs << str << std::endl;
    }

    ofs << "@a(a+0=a)->" + A + "+0=" + A << std::endl;
    ofs << A + "+0=" + A << std::endl;

    std::string prooff = "";
    std::ifstream ifs_proof("../prover/parts_of_proof/less_proof.txt");

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

void prove_greater(int a, int b, std::string output)
{
    std::ofstream ofs(output);

    {
        std::string a_str = "0";
        std::string b_str = "0";

        for (size_t i = 0; i < a; i++)
        {
            a_str += "'";
            if (i < b)
            {
                b_str += "'";
            }
        }
        ofs << "|-!?p(" + a_str + "+p)=" + b_str << std::endl;
    }

    std::ifstream ifs_simple("../prover/parts_of_proof/greater_base.txt");
    while (ifs_simple.good())
    {
        std::string str;
        std::getline(ifs_simple, str);
        ofs << str << std::endl;
    }

    int c = a - b;
    std::string B = "";
    std::string C = "";
    for (int i = 0; i < c - 1; i++)
    {
        if (i < b)
        {
            B += "'";
        }
        C += "'";
    }
    if (c - 1 < b)
    {
        for (int i = c - 1; i < b; i++)
        {
            B += "'";
        }
    }

    std::ifstream ifs_second("../prover/parts_of_proof/greater_proof.txt");

    while (ifs_second.good())
    {
        std::string str;
        std::getline(ifs_second, str);
        str = replace_all(str, "C", C);
        str = replace_all(str, "B", B);
        str = replace_all(str, " ", "");
        ofs << str << std::endl;
    }
}