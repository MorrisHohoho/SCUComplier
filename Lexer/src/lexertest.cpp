//
// Created by Administrator on 2023/5/22.
//
#include "lexertest.h"

void test_lexer(const std::string& ref, const std::string& output)
{
    std::ifstream ref_file(ref);
    std::ifstream out_file(output);
    std::string ref_line;
    std::string out_line;
    while(std::getline(ref_file, ref_line))
    {
        std::getline(out_file, out_line);
        assert(ref_line==out_line);
    }
    std::cout<<"Test "<< ref<<" passed.\n";
}