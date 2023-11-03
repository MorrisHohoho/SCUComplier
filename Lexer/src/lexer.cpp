//
// Created by 芝士海豹 on 2023/4/4.
//

#include "lexer.h"

using namespace std;

//internal function
/**
 *  Get output file name from input file name.
 * @param current_file
 * @return
 */
string getOutputName(const string &current_file) {
    string output_name;
    int slash_pos = current_file.find_last_of('\\');
    int dot_pos = current_file.find_last_of('.');
    if (slash_pos == string::npos && dot_pos == string::npos) {
        output_name = current_file + ".out";
    } else {
        if (slash_pos == string::npos) {
            output_name = current_file.substr(0, dot_pos) + ".out";
        } else if (dot_pos == string::npos) {
            output_name = current_file.substr(slash_pos + 1, current_file.size()) + ".out";
        } else {
            output_name = current_file.substr(slash_pos + 1, dot_pos - slash_pos - 1) + ".out";
        }
    }
    return output_name;
}

//internal function
/**
 *  Check whether a char is a digit.
 * @param c
 * @return
 */
bool isSymbol(const char c) {
    return (c == '+' || c == '-' || c == '*' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' ||
            c == ',' || c == ';');
}

//internal function
/**
 *  Check whether a string is a reserved word.
 * @param str
 * @return
 */
bool isReservedWord(const string &str) {
    return (str == "if" || str == "else" || str == "int" || str == "return" || str == "void" || str == "while");
}


/**
 * Constructor
 * @param fin
 * @param current_file
 */
void lexer::FillBuffer(ifstream &fin, const string &current_file) {
    fin.open(current_file, ios::in);
    if (!fin.is_open()) {
        cerr << "Can't open input file. \n";
    }
    char c;
    while ((c = fin.get()) != EOF) {
        input_buffer.push_back(c);
    }
    fin.close();
}

void lexer::GetOutput() {
    int i = 0;
    int j = i;
    string tmp_str ="";
    line_no = 1;
    current_state = State::INIT;
    current_token = TokenType::INIT;

    while (i < input_buffer.size()) {
        char c = input_buffer[i];
        switch (current_state) {
            case State::INIT: {
                if (isalpha(c)) {
                    current_state = State::INID;
                    current_token = TokenType::ID;
                    j=i;
                } else if (isdigit(c)) {
                    current_state = State::INNUM;
                    current_token = TokenType::NUM;
                    j=i;
                } else if (c == '=') {
                    current_state = State::INASSIGN;
                    current_token = TokenType::SPECIAL_SYMBOL;
                    j=i;
                } else if (c == '<') {
                    current_state = State::INLS;
                    current_token = TokenType::SPECIAL_SYMBOL;
                    j=i;
                } else if (c == '>') {
                    current_state = State::INGT;
                    current_token = TokenType::SPECIAL_SYMBOL;
                    j=i;
                } else if (c == '!') {
                    current_state = State::INEXCLAIM;
                    current_token = TokenType::SPECIAL_SYMBOL;
                    j=i;
                } else if (c == '/') {
                    current_state = State::INSLASH;
                    current_token = TokenType::SPECIAL_SYMBOL;
                    j=i;
                } else if (c == ' ') {
                    current_state = State::INIT;
                } else if (c == '\t') {
                    current_state = State::INIT;
                } else if (c == '\n') {
                    current_state = State::INIT;
                    line_no++;
                } else if (isSymbol(c)) {
                    current_state = State::INSYMBOL;
                    j=i;
                } else {
                    current_state = State::ERROR;
                }
                break;
            }
            case State::INID: {
                if (isalpha(c)) {
                    current_state = State::INID;
                    current_token = TokenType::ID;
                } else {
                    current_state = State::DONE;
                    current_token = TokenType::ID;
                }
                break;
            }
            case State::INNUM: {
                if (isdigit(c)) {
                    current_state = State::INNUM;
                    current_token = TokenType::NUM;
                } else {
                    current_state = State::DONE;
                    current_token = TokenType::NUM;
                }
                break;
            }
            case State::INASSIGN: {
                if (c == '=') {
                    current_state = State::INEQ;
                    current_token = TokenType::SPECIAL_SYMBOL;
                } else {
                    current_state = State::DONE;
                    current_token = TokenType::SPECIAL_SYMBOL;
                }
                break;
            }
            case State::INEQ:{
                current_state = State::DONE;
                current_token = TokenType::SPECIAL_SYMBOL;
                break;
            }
            case State::INLS: {
                if (c == '=') {
                    current_state = State::INLSOREQ;
                    current_token = TokenType::SPECIAL_SYMBOL;
                } else {
                    current_state = State::INNEARLYDONE;
                    current_token = TokenType::SPECIAL_SYMBOL;
                }
                break;
            }
            case State::INLSOREQ:{
                current_state = State::DONE;
                current_token = TokenType::SPECIAL_SYMBOL;
                break;
            }
            case State::INGT: {
                if (c == '=') {
                    current_state = State::INGTOREQ;
                    current_token = TokenType::SPECIAL_SYMBOL;
                } else {
                    current_state = State::INNEARLYDONE;
                    current_token = TokenType::SPECIAL_SYMBOL;
                }
                break;
            }
            case State::INGTOREQ:{
                current_state = State::DONE;
                current_token = TokenType::SPECIAL_SYMBOL;
                break;
            }
            case State::INEXCLAIM: {
                if (c == '=') {
                    current_state = State::INNEQ;
                    current_token = TokenType::SPECIAL_SYMBOL;
                } else {
                    current_state = State::ERROR;
                    current_token = TokenType::ERROR;
                }
                break;
            }
            case State::INNEQ:{
                current_state = State::DONE;
                current_token = TokenType::SPECIAL_SYMBOL;
                break;
            }
            case State::INSYMBOL: {
                current_state = State::DONE;
                current_token = TokenType::SPECIAL_SYMBOL;
                break;
            }
            case State::INSLASH: {
                if (c == '*') {
                    current_state = State::INCOMMENT;
                } else {
                    current_state = State::DONE;
                    current_token = TokenType::SPECIAL_SYMBOL;
                }
                break;
            }
            case State::INCOMMENT: {
                if (c == '*') {
                    current_state = State::INCOMMENTEND;
                } else {
                    if (c == '\n')
                        line_no++;
                    current_state = State::INCOMMENT;
                }
                break;
            }
            case State::INCOMMENTEND: {
                if (c == '/') {
                    current_state = State::INNEARLYDONE;
                    current_token =TokenType::INIT;
                } else {
                    if (c == '\n')
                        line_no++;
                    current_state = State::INCOMMENT;
                }
                break;
            }
            case State::INNEARLYDONE:{
                current_state = State::DONE;
                break;
            }
        }
        if(current_state==State::DONE)
        {
            tmp_str.clear();
            if(current_token == TokenType::ID)
            {
                for(int z=j;z<i;z++)
                {
                    tmp_str.append(1,input_buffer[z]);
                }
                if(isReservedWord(tmp_str))
                    tmp_str= to_string(line_no)+": RESERVED_WORD "+tmp_str+"\n";
                else
                    tmp_str= to_string(line_no)+": ID "+tmp_str+"\n";
                output_buffer.push_back(tmp_str);
            }
            else if(current_token==TokenType::NUM)
            {
                for(int z=j;z<i;z++)
                {
                    tmp_str.append(1,input_buffer[z]);
                }
                tmp_str= to_string(line_no)+": NUM "+tmp_str+"\n";
                output_buffer.push_back(tmp_str);
            }
            else if(current_token==TokenType::SPECIAL_SYMBOL)
            {
                for(int z=j;z<i;z++)
                {
                    tmp_str.append(1,input_buffer[z]);
                }
                tmp_str= to_string(line_no)+": SPECIAL_SYMBOL "+tmp_str+"\n";
                output_buffer.push_back(tmp_str);
            }
            else if(current_token==TokenType::ERROR)
            {
                tmp_str = "ERROR AT "+ to_string(line_no);
                output_buffer.push_back(tmp_str);
                break;
            }
            current_state=State::INIT;
            current_token=TokenType::INIT;
            i--;
        }
        if(current_state==State::ERROR)
        {
            tmp_str = "ERROR at line "+ to_string(line_no);
            output_buffer.push_back(tmp_str);
            break;
        }
        i++;
    }
}

void lexer::Lex(const std::string &source, const std::string &output) {
    FillBuffer(fin, source);

    GetOutput();

    //Ouput
    if (output != "") {
        string output_name = output + "/" + getOutputName(source);
        Output(output_name);
    } else {
        for (auto str: output_buffer)
            cout << str;
    }
    input_buffer.clear();
}

void lexer::Output(const std::string &output_name) {
    fout.open(output_name, ios::out);
        for (auto c: output_buffer) {
            fout << c;
        }
        fout.close();
    output_buffer.clear();
}
