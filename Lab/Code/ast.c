#include "common.h"
#include <assert.h>

int blank_num = 0;

AstNode *create_node(char *yylabel, int yytype, int yylineno)
{
    //printf("a\n");
    AstNode *ret = malloc(sizeof(AstNode));
    assert(ret);
    ret->lineno = yylineno;
    ret->syntax_label = (char *)malloc(strlen(yylabel) + 1);
    strcpy(ret->syntax_label, yylabel);
    
    ret->type = yytype;
    ret->first_son = ret->last_son = ret->right_brother = NULL;
    ret->child_num = 0;
    //printf("b\n");
    return ret;
}

void Link_tree(AstNode *parent, AstNode *son)
{
    if(son == NULL || parent == son)
        return;
    if(parent->last_son == NULL)
    {
        //printf("e\n");
        parent -> first_son = parent -> last_son = son;
        parent -> child_num++;
        //printf("f\n");
    }
    else
    {
        //printf("g\n");
        assert(parent->last_son);
        parent -> child_num++;
        parent -> last_son -> right_brother=son;
        //printf("h\n");
        parent -> last_son = son;
        //printf("h\n");
    }
    //printf("d\n");
}

void print_tree(AstNode *p)
{
    int i;
    AstNode *q = NULL;

    if(p->type == 1)
    {
        for(i=0;i<blank_num;i++)
            printf("  ");
        printf("%s (%d)\n", p->syntax_label, p->lineno);
        blank_num++;

        q = p->first_son;
        assert(q);
        while(q!=NULL)
        {
            print_tree(q);
            q = q->right_brother;
        }
    }
    else if(p->type == 2)
    {
        for(i = 0;i<blank_num;i++)
            printf("  ");
        printf("%s\n",p->syntax_label);
        
        blank_num++;
        q = p->first_son;
        while(q!=NULL)
        {
            print_tree(q);
            q = q->right_brother;
        }
    }
    else if(p-> type == 3)
    {
        for(i = 0;i<blank_num;i++)
            printf("  ");
        blank_num++;
        
        if(!strcmp(p->syntax_label,"INT"))
            printf("INT: %d\n",p->intval);
        else if(!strcmp(p->syntax_label,"FLOAT"))
            printf("FLOAT: %f\n", p->floval);
        else if(!strcmp(p->syntax_label,"ID"))
            printf("ID: %s\n",p->strval);
        else if(!strcmp(p->syntax_label,"TYPE"))
            printf("TYPE: %s\n",p->strval);
    }
    blank_num--;
}
