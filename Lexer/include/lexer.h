//
// Created by 芝士海豹 on 2023/4/4.
//

#ifndef CMINUS_COMPLIER_LEXER_H
#define CMINUS_COMPLIER_LEXER_H

#include<vector>
#include<string>
#include<fstream>
#include<iostream>




enum class TokenType{
    INIT,
    ID,
    NUM,
    SPECIAL_SYMBOL,
    ERROR,
};

//States of FSM
enum class State{
    INIT,
    DONE,
    ERROR,
    INID,
    INNUM,
    INASSIGN,
    INEQ,
    INNEQ,
    INGT,
    INLS,
    INGTOREQ,
    INLSOREQ,
    INSLASH,
    INCOMMENT,
    INCOMMENTEND,
    INEXCLAIM,
    INSYMBOL,
    INNEARLYDONE,
};


class lexer {
private:
    std::vector<char> input_buffer;
    std::vector<std::string> output_buffer;
    std::ifstream  fin;
    std::ofstream  fout;
    TokenType current_token = TokenType::INIT;
    State current_state = State::INIT;
    int line_no;
public:
    lexer()=default;
    ~lexer()=default;

    void Lex(const std::string& source, const std::string& output);
    void FillBuffer(std::ifstream& fin, const std::string& current_file);
    void Output( const std::string& output_name);
    void GetOutput();
};


#endif //CMINUS_COMPLIER_LEXER_H
