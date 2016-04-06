#include "common.h"

AstNode *root = NULL;
bool error_flag = false;

extern FILE* yyin;
extern int blank_num;
extern int yylineno;
extern int yyparse(void);
extern void yyrestart(FILE *input_file);

int main(int argc, char ** argv)
{
    if(argc == 0 || argc == 1)
        return 0;
    else
    {
        int i;
        for(i = 1;i < argc;i++)
        {
            FILE *fp = fopen(argv[i],"r");
            if(!fp)
            {
                perror(argv[i]);
                return 1;
            }
            yyrestart(fp);
            yyparse();

            if(!error_flag)
            {
                blank_num = 0;
                print_tree(root);
            }
            else
                error_flag = false;

            fclose(fp);
            yylineno = 1;
        }
    }
    return 0;
}
