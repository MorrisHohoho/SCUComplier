%locations

%{
    #include <stdio.h>
    #include "lex.yy.c"
    #include "multinode.h"

    #define YYDEBUG 1
    yydebug=0;

    struct multinode* root;
    char syntax_error = 0;

%}

%union{
    struct multinode* NODE;
}

// terminals
%token <NODE> INT
%token <NODE> FLOAT
%token <NODE> ID
%token <NODE> SEMI
%token <NODE> COMMA
%token <NODE> ASSIGNOP
%token <NODE> RELOP
%token <NODE> PLUS
%token <NODE> MINUS
%token <NODE> STAR
%token <NODE> DIV
%token <NODE> AND
%token <NODE> OR
%token <NODE> DOT
%token <NODE> NOT
%token <NODE> TYPE
%token <NODE> LP
%token <NODE> RP
%token <NODE> LB
%token <NODE> RB
%token <NODE> LC
%token <NODE> RC
%token <NODE> STRUCT
%token <NODE> RETURN
%token <NODE> IF
%token <NODE> ELSE
%token <NODE> WHILE


// non-terminals
%type <NODE>Program ExtDefList ExtDef ExtDecList
%type <NODE>Specifier StructSpecifier OptTag Tag
%type <NODE>VarDec FunDec VarList ParamDec
%type <NODE>CompSt StmtList Stmt 
%type <NODE>DefList Def DecList Dec
%type <NODE>Exp Args


%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT 
%left DOT LB RB LP RP 
%%
Program : ExtDefList {$$ = createNode("Program",@1.first_line); addNode($$,$1);
root = $$;}
    ;

ExtDefList : ExtDef ExtDefList {$$ = createNode("ExtDefList",@1.first_line); addNode($$,$1); addNode($$,$2);}
    | { $$=NULL;}
    ;

ExtDef : Specifier ExtDecList SEMI {$$ = createNode("ExtDef",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | Specifier SEMI {$$ = createNode("ExtDef",@1.first_line); addNode($$,$1); addNode($$,$2);}
    | Specifier FunDec CompSt {$$ = createNode("ExtDef",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | error SEMI{custom_error("Global variable cannot have initializer");}
    ;

ExtDecList : VarDec {$$ = createNode("ExtDecList",@1.first_line); addNode($$,$1);}
    | VarDec COMMA ExtDecList {$$ = createNode("ExtDecList",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    ;

Specifier : TYPE {$$ = createNode("Specifier",@1.first_line); addNode($$,$1);}
    | StructSpecifier   {$$ = createNode("Specifier",@1.first_line); addNode($$,$1);}
    ;

StructSpecifier : STRUCT OptTag LC DefList RC   {$$ = createNode("StructSpecifier",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3); addNode($$,$4); addNode($$,$5);}
    | STRUCT Tag    {$$ = createNode("StructSpecifier",@1.first_line); addNode($$,$1); addNode($$,$2);}
    ;

OptTag : ID {$$ = createNode("OptTag",@1.first_line); addNode($$,$1);}
    | { $$=NULL;}
    ;

Tag : ID {$$ = createNode("Tag",@1.first_line); addNode($$,$1);}
    ;

VarDec : ID {$$ = createNode("VarDec",@1.first_line); addNode($$,$1);}
    | VarDec LB INT RB  {$$ = createNode("VarDec",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3); addNode($$,$4);}
    | VarDec LB error RB {custom_error("Illegal array index");}
    ;

FunDec : ID LP VarList RP {$$ = createNode("FunDec",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3); addNode($$,$4);}
    |ID LP RP {$$ = createNode("FunDec",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | ID LP error RP {custom_error("Expect more parameters after \',\'");}
    ;

VarList : ParamDec COMMA VarList {$$ = createNode("VarList",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | ParamDec {$$ = createNode("VarList",@1.first_line); addNode($$,$1);}
    ;

ParamDec : Specifier VarDec {$$ = createNode("ParamDec",@1.first_line); addNode($$,$1); addNode($$,$2);}
    ;

CompSt : LC DefList StmtList RC {$$ = createNode("CompSt",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3); addNode($$,$4);}
    ;

StmtList : Stmt StmtList {$$ = createNode("StmtList",@1.first_line); addNode($$,$1); addNode($$,$2);}
    | { $$=NULL;}
    ;

Stmt : Exp SEMI {$$ = createNode("Stmt",@1.first_line); addNode($$,$1); addNode($$,$2);}
    | CompSt {$$ = createNode("Stmt",@1.first_line); addNode($$,$1);}
    | RETURN Exp SEMI {$$ = createNode("Stmt",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | IF LP Exp RP Stmt {$$ = createNode("Stmt",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3); addNode($$,$4); addNode($$,$5);}
    | IF LP Exp RP Stmt ELSE Stmt {$$ = createNode("Stmt",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3); addNode($$,$4); addNode($$,$5); addNode($$,$6); addNode($$,$7);}
    | WHILE LP Exp RP Stmt {$$ = createNode("Stmt",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3); addNode($$,$4); addNode($$,$5);}
    | IF LP Exp RP error ELSE {custom_error("Expect statement between \'if\' and \'else\'");}
    | RETURN error SEMI {custom_error("Expect an Exp between return and ;");}
    | WHILE LP Exp RP error SEMI{custom_error("Expect statement after \'while\' condition");}
    | WHILE LP error RP {custom_error("Illegal Exp");}
    ;

DefList : Def DefList {$$ = createNode("DefList",@1.first_line); addNode($$,$1); addNode($$,$2);}
    | { $$=NULL;}
    ;

Def : Specifier DecList SEMI {$$ = createNode("Def",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | error SEMI{custom_error("Illegal type specifier");}
    | Specifier error SEMI {custom_error("Expect DecList");}
    ;

DecList : Dec {$$ = createNode("DecList",@1.first_line); addNode($$,$1);}
    | Dec COMMA DecList {$$ = createNode("DecList",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    ;

Dec : VarDec {$$ = createNode("Dec",@1.first_line); addNode($$,$1);}
    | VarDec ASSIGNOP Exp {$$ = createNode("Dec",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    ;

Exp : Exp ASSIGNOP Exp {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | Exp AND Exp {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | Exp OR Exp {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | Exp RELOP Exp {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | Exp PLUS Exp {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | Exp MINUS Exp {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | Exp STAR Exp {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | Exp DIV Exp {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | LP Exp RP {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | MINUS Exp {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2);}
    | NOT Exp {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2);}
    | ID LP Args RP {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3); addNode($$,$4);}
    | ID LP RP {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | Exp LB Exp RB {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3); addNode($$,$4);}
    | Exp DOT ID {$$ = createNode("Exp",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | ID {$$ = createNode("Exp",@1.first_line); addNode($$,$1);}
    | INT {$$ = createNode("Exp",@1.first_line); addNode($$,$1);}
    | FLOAT {$$ = createNode("Exp",@1.first_line); addNode($$,$1);}
    | Exp STAR error SEMI {custom_error(" \'*=\' is not supported");}
    | Exp LB error RB {custom_error("Array index can only be an integer");}
    ;

Args : Exp COMMA Args {$$ = createNode("Args",@1.first_line); addNode($$,$1); addNode($$,$2); addNode($$,$3);}
    | Exp {$$ = createNode("Args",@1.first_line); addNode($$,$1);}
    ; 
%%

int yyerror(char *s){
    /* if(!lexer_error)
    fprintf(stderr, "Error type B at line %d: %s.\n", yylineno, s);
    printf("Error type B at line %d: %s.\n", yylineno, s); */
}

void custom_error(char *s)
{
    syntax_error= 1;
    fprintf(stderr, "Error type B at line %d: %s.\n", yylineno, s);
}