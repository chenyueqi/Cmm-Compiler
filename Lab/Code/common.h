#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define true 1
#define false 0

typedef struct astnode AstNode;

struct astnode
{
    int lineno;
    int type;
    char* syntax_label;
    union{
        int intval;
        double floval;
        char *strval;
    };

    unsigned int child_num; 
    AstNode *first_son;
    AstNode *last_son;
    AstNode *right_brother;
};


AstNode *create_node(char *label, int type, int lineno);
void print_tree(AstNode *p);
void Link_tree(AstNode *parent, AstNode *son);

#endif
