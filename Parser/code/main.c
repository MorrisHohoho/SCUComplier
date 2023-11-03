#include"syntax.tab.h"
#include "multinode.h"

#include<stdio.h>
#include<string.h>

extern int yylineno;
extern int yyparse();
extern void yyrestart(FILE*);
extern struct multinode* root;
extern char syntax_error;
int main(int argc, char** argv)
{
    //Read only the stdin
    if(argc<=1)
    {
        yyparse();
        return 0;
    }
    else
    {
        for(int i=1;i<argc;i++)
        {
            FILE *f =fopen(argv[i],"r");
            if(!f)
            {
                perror(argv[i]);
                return 1;
            }
            yyrestart(f);
            yylineno=1;
            yyparse();
            if(!syntax_error)
            Print_Tree(root,root->layer);
            fclose(f);
        }
    }
}

