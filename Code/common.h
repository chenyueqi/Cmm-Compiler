#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#ifndef __COMMON__
#define __COMMON__

//#define DEBUG

struct tree_node
{
	int lineno;
	int type;/*1:INT FLOAT TYPE ID 2:lexical 3:bison*/
	char* unit_name;
	char* token_name;
	unsigned int children_num;
	struct tree_node** children;
};

void Seman_analysis(struct tree_node* root);

void translate(struct tree_node* root);

void display_tree(struct tree_node* , int);//display syntax tree

void pre_occupy_func();

void outputInterCode();

void initial_InterCodes();

#endif
