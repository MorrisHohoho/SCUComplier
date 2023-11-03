//
// Created by 芝士海豹 on 2023/4/4.
//
#include"FileController.h"
#include"lexer.h"
#include"lexertest.h"


#include<iostream>
#include<string>

using namespace std;

int main(int argc, char *argv[])
{
    string input;
    string output;
    bool is_out2file = false;   //contol whether output to file
    FileController fc;        //control input file
    FileController out;      //control output file
    FileController lt;      //control ref file
    lexer mylex;            //lexer

    switch (argc) {
    case 1: {
        cout << "Please input file path.\n";
        return -1;
        break;
    }
        case 2: {
            input.assign(argv[1]);
            break;
        }
        case 3:
        {
            is_out2file= true;
            input.assign(argv[1]);
            output.assign(argv[2]);
            break;
        }
        default: {
            cout<<"Too many arguments.\n";
            return -1;
            break;
        }
    }
    if(is_out2file)
        fc.SetOutputPath(output);

    fc.ReadFile(input);
    while(true)
    {
        string current_file = fc.GetFile();
        if(current_file.empty())
            break;
        else
        {
            mylex.Lex(current_file,output);
        }
    }

    out.ReadFile(output);
    lt.ReadFile("../ref");
    while(true)
    {
        string current_file = out.GetFile();
        if(current_file.empty())
            break;
        else
        {
            string ref_file = lt.GetFile();
            test_lexer(current_file,ref_file);
        }
    }



    return 0;
}