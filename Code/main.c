#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"syntax.tab.h"
#include<stdarg.h>
#include"common.h"

extern FILE* yyin;
extern int yyparse(void);
extern void yyrestart (FILE *input_file  );
extern unsigned int error_num;

struct tree_node* root;

//void display_tree(struct tree_node* , int);// display syntax tree
struct tree_node* creat_node(int arity , char *token_name , struct YYLTYPE* current_pos, ...);
void distroy_tree(struct tree_node*);
void display_float(char* p);// change float number string  to float type in c
extern int yylineno;

int main(int argc , char** argv)
{
	FILE* source = fopen(argv[1] , "r");
	if(!source)
	{
		perror(argv[1]);
		return 1;
	}
	yyrestart(source);
	yyparse();
	if(error_num == 0)
	{
		pre_occupy_func();
		Seman_analysis(root);
		initial_InterCodes();
		translate(root);
		distroy_tree(root);
	}
	else
		error_num = 0;
	yylineno = 1;
	fclose(source);

	outputInterCode();

	return 0;
/*	int i = 0;
	for(i = 1 ; i < argc ; i ++)
	{
		FILE* f = fopen(argv[i] , "r");
		if(!f)
		{
			perror(argv[i]);
			return 1;
		}
	//	printf("========================\n");
	//	printf("*** %s ***\n" , argv[i]);
		yyrestart(f);
		yyparse();
		if(error_num == 0)
		{
			Seman_analysis(root);
	//		display_tree(root , 0);
			distroy_tree(root);

		}
		else
			error_num = 0;
		yylineno = 1;
		fclose(f);
	}
//	printf("========================\n");
	return 0;*/
}

struct tree_node* creat_node(int arity , char* token_name , struct YYLTYPE* current_pos, ...)
{
	if(arity == 0)
		return NULL;
	struct tree_node* result = (struct tree_node*)malloc(sizeof(struct tree_node));
	result->lineno = current_pos->first_line; 
	result->type = 3;
	result->token_name = (char*)malloc(strlen(token_name) + 1);
	strcpy(result->token_name , token_name);
	result->unit_name = (char*)malloc(strlen(token_name) + 1);
	strcpy(result->unit_name , token_name);

	result->children_num = arity;
	
	
	result->children = (struct tree_node**)malloc(sizeof(struct tree_node*) * result->children_num);// children pointer array

	va_list vap;
	va_start(vap , current_pos);
	int i = 0;
	for(; i < arity ; i++)
		result->children[i] = va_arg(vap , struct tree_node*);
	va_end(vap);
	return result;
}
void display_tree(struct tree_node* p , int count)
{
	if(p->type == 1)
	{
		int i = 0;
		for(; i < count ; i++)
			printf(" ");
		if(!strcmp(p->token_name , "FLOAT"))
			display_float(p->unit_name);
		else
			printf("%s: %s\n" , p->token_name , p->unit_name);
	}
	else if(p->type == 2)
	{
		int i = 0;
		for(; i < count ; i++)
			printf(" ");
		printf("%s\n" , p->token_name);
		for(i = 0 ; i < p->children_num ; i++)
		{
			if(p->children[i] == NULL)
				continue;
			display_tree(p->children[i] , count + 2);
		}
	}
	else if(p->type == 3)
	{
		int i = 0;
		for(; i < count ; i++)
			printf(" ");
		printf("%s (%d)\n" , p->token_name , p->lineno);
		for(i = 0 ; i < p->children_num ; i++)
		{
			if(p->children[i] == NULL)
				continue;
			display_tree(p->children[i] , count + 2);
		}
	}
}

void distroy_tree(struct tree_node* p )
{
	int i = 0;
	for( ; i < p->children_num ; i++)
	{
		if(p->children[i] != NULL)
			distroy_tree(p->children[i]);
	}
	free(p) ;
	p = NULL;
}

void display_float(char* p)
{
	unsigned int pivot = 0;
	float result = 0;
//	float integer = 0;
//	float decimal = 0;
	while(p[pivot] != '\0')
	{
		if( (p[pivot] == 'e') || (p[pivot] == 'E'))
			break;
		pivot++;
	}
	if( (p[pivot] == 'e') || (p[pivot] == 'E'))
	{
		int i = 0;
		int flag = 0;
		int count = 0;
		while(i != pivot)
		{
			if(p[i] == '.')
			{
				i ++ ;
				flag = 1;
				continue;
			}
			if(flag)
				count++;
			result = result * 10 + p[i] - '0';
			i++;
		}
		for(i = 0 ; i < count ; i++)
			result = result / 10;
		if(p[pivot + 1] == '-')
		{
			char *q = &p[pivot + 2];
			unsigned int  order = atoi(q);
			int j = 0;
			for( ; j < order; j++)
			 	result = result / 10; 

		}
		else if(p[pivot + 1] == '+')
		{
			char *q = &p[pivot + 2];
			unsigned int  order = atoi(q);
			int j = 0;
			for( ; j < order; j++)
			 	result = result * 10; 

		}
		else
		{
			char *q = &p[pivot + 1];
			unsigned int  order = atoi(q);
			int j = 0;
			for( ; j < order; j++)
			 	result = result * 10; 

		}
		printf("FLOAT: %f\n" , result);
	}
	else
	{
		int i = 0;
		int flag = 0;
		int count = 0;
		while(p[i] != '\0')
		{
			if(p[i] == '.')
			{
				i++;
				flag = 1;
				continue;
			}
			if(flag)
				count ++;
			result = result * 10 + p[i] - '0';
			i++;
		}

		for(i = 0 ; i < count ; i++)
			result = result / 10;
		printf("FLOAT: %f\n" , result);
	}
}
