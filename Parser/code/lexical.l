%option yylineno

%{
    #include "syntax.tab.h"
    #include "multinode.h"

    int yycolumn = 1;


            
    #define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn+yyleng - 1;\
    yycolumn += yyleng;
    extern char syntax_error;
%}

letter [_a-zA-Z]
digit [0-9]
whitespace [ \t\r]

%%
{whitespace}+ {/*skip whitespace*/}


([1-9]{digit}+)|{digit} {
    char* tmp =join_str("INT: ",yytext); 
    yylval.NODE = createNode(tmp,-1); free(tmp); return INT;}

"0"[0-7]+ {
    int num=oct2dec(yytext);
    char text[128]={'\0'};
    sprintf(text,"%d",num);
    char* tmp =join_str("INT: ",text); 
    yylval.NODE = createNode(tmp,-1); free(tmp); return INT;}    

"0"[0-9]+ {syntax_error=1;
        fprintf(stderr, "Error type A at Line %d: Illegal Octal \'%s\'.\n",yylineno,yytext);
 return INT;}  

("0x"|"0X")({digit}|[abcdefABCDEF])+ {
    int num = hex2dec(yytext);
    char text[128]={'\0'};
    sprintf(text,"%d",num);
    char* tmp =join_str("INT: ",text); 
    yylval.NODE = createNode(tmp,-1); free(tmp); return INT;}

("0x"|"0X")({digit}|{letter})+ {syntax_error=1;  
        fprintf(stderr, "Error type A at Line %d: Illegal Hexdecimal \'%s\'.\n",yylineno,yytext);
 return INT;}


(({digit}+"."{digit}+)|({digit}+".")|("."{digit}+))([eE][-+]?{digit}+)? {
        char*tmp=NULL;
        char text[128]={'\0'};
        float num = pow2dec(yytext);
        sprintf(text,"%.6f",num);
        tmp =join_str("FLOAT: ",text); 
    yylval.NODE = createNode(tmp,-1); free(tmp); return FLOAT;}

(({digit}+"."{digit}+)|({digit}+".")|("."{digit}+))([eE][-+]?) {
        syntax_error=1;
        fprintf(stderr, "Error type A at Line %d: Illegal number \'%s\'.\n",yylineno,yytext);
        return FLOAT;}

 "." {yylval.NODE =createNode("DOT",-1);return DOT;}

[0-9.]+ {
        syntax_error=1;
        fprintf(stderr, "Error type A at Line %d: Illegal number \'%s\'.\n",yylineno,yytext);
        return FLOAT;
}

 "struct" {yylval.NODE =createNode("STRUCT",-1); return STRUCT;}

 "return" {yylval.NODE =createNode("RETURN",-1);return RETURN;}

 "if" {yylval.NODE =createNode("IF",-1);return IF;}

 "else" {yylval.NODE =createNode("ELSE",-1);return ELSE;}

 "while" {yylval.NODE =createNode("WHILE",-1);return WHILE;}
 
 "int"|"float" { char* tmp =join_str("TYPE: ",yytext); 
    yylval.NODE = createNode(tmp,-1); free(tmp); return TYPE;}

{letter}+[_0-9a-zA-Z]* {char* tmp =join_str("ID: ",yytext); 
    yylval.NODE = createNode(tmp,-1); free(tmp); return ID;}

{digit}+[_0-9a-zA-Z]* {
    syntax_error=1;
    fprintf(stderr, "Error type A at Line %d: Illegal identifier \'%s\'\n",yylineno,yytext);
    return ID;}

";" {yylval.NODE =createNode("SEMI",-1);return SEMI;}

"," {yylval.NODE =createNode("COMMA",-1);return COMMA;}

"=" {yylval.NODE =createNode("ASSIGNOP",-1);return ASSIGNOP;}

 ">"|"<"|">="|"<="|"=="|"!=" {yylval.NODE =createNode("RELOP",-1);return RELOP;}

 "+" {yylval.NODE =createNode("PLUS",-1);return PLUS;}

 "-" {yylval.NODE =createNode("MINUS",-1);return MINUS;}

 "*" {yylval.NODE =createNode("STAR",-1);return STAR;}

 "/" {yylval.NODE =createNode("DIV",-1);return DIV;}

 "&&" {yylval.NODE =createNode("AND",-1);return AND;}

 "||" {yylval.NODE =createNode("OR",-1);return OR;}


 "!" {yylval.NODE =createNode("NOT",-1);return NOT;}

 "(" {yylval.NODE =createNode("LP",-1);return LP;}

 ")" {yylval.NODE =createNode("RP",-1);return RP;}

 "[" {yylval.NODE =createNode("LB",-1);return LB;}

 "]" {yylval.NODE =createNode("RB",-1);return RB;}

 "{" {yylval.NODE =createNode("LC",-1);return LC;}

 "}" {yylval.NODE =createNode("RC",-1);return RC;}

"/*"  {/*skip comments*/
char c = input();
while(1)
{
    if(c=='*')
    {
        c = input();
        if(c=='/')
        {
            break;
        }
    }
    c = input();
}
}

"\n"  {yycolumn=1;}

"//" {/*skip comments*/
char c = input();
while(c!='\n') c=input();}


. {fprintf(stderr, "Error type A at Line %d: Mysetrious character \'%s\'\n",yylineno,yytext);
    syntax_error=1;
    return -1;}

%%