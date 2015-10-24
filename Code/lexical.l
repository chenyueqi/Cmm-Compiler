%{
	#include<stdio.h>
	#include"syntax.tab.h"
	int yycolumn = 1;
	extern unsigned int error_num;
	struct tree_node
	{
		int lineno;
		int type;
		char* unit_name;
		char *token_name;
		unsigned int children_num;
		struct tree_node** children;
	};
	#define YY_USER_ACTION \
	yylloc.first_line = yylloc.last_line = yylineno; \
	yylloc.first_column = yycolumn; \
	yylloc.last_column = yycolumn + yyleng -1; \
	yycolumn += yyleng;
	void creat_node_token(YYSTYPE* yylval , int yylineno , int type , char *token_name , int token_length , char* yytext , int yyleng);
%}
%option yylineno

letter 		[a-zA-Z]
digit 		[0-9]
digits		{digit}+
radix		({digits}\.{digit}?)|({digit}?\.{digits})


SEMI 		;
COMMA 		,
ASSIGNOP 	=
RELOP 		>|<|>=|<=|==|!=
PLUS 		\+
MINUS 		-
STAR 		\*
DIV 		\/
AND 		&&
OR 		\|\|
DOT 		\.
NOT 		!
TYPE 		int|float
LP 		\(
RP 		\)
LB 		\[
RB 		\]
LC 		\{
RC 		\}
STRUCT 		struct
RETURN 		return
IF 		if
ELSE 		else
WHILE 		while

INT		{digits}
FLOAT		({digits}\.{digits})|({radix}[E|e][+-]?{digits})
<<<<<<< HEAD
ID		({letter}|_)({letter}|{digit}|_)*
=======
ID		{letter}({letter}|{digit}|_)*
>>>>>>> origin/master

WHITESPACE	" "|"	"
%%
{SEMI}		{creat_node_token(&yylval , yylineno , 2 , "SEMI" , 5 , yytext , yyleng);	return SEMI;}
{COMMA}		{creat_node_token(&yylval , yylineno , 2 , "COMMA" , 6 , yytext , yyleng);	return COMMA;}
{ASSIGNOP}	{creat_node_token(&yylval , yylineno , 2 , "ASSIGNOP" , 9 , yytext , yyleng); 	return ASSIGNOP;}
{RELOP}		{creat_node_token(&yylval , yylineno , 2 , "RELOP" , 6 , yytext , yyleng);	return RELOP;}
{PLUS}		{creat_node_token(&yylval , yylineno , 2 , "PLUS" , 5 , yytext , yyleng);	return PLUS;}
{MINUS}		{creat_node_token(&yylval , yylineno , 2 , "MINUS" , 6 , yytext , yyleng);	return MINUS;}
{STAR}		{creat_node_token(&yylval , yylineno , 2 , "STAR" , 5 , yytext , yyleng);	return STAR;}
{DIV}		{creat_node_token(&yylval , yylineno , 2 , "DIV" , 4 , yytext , yyleng);	return DIV;}
{AND}		{creat_node_token(&yylval , yylineno , 2 , "AND" , 4 , yytext , yyleng);	return AND;}
{OR}		{creat_node_token(&yylval , yylineno , 2 , "OR" , 3 , yytext , yyleng);		return OR;}
{DOT}		{creat_node_token(&yylval , yylineno , 2 , "DOT" , 4 , yytext , yyleng);	return DOT;}
{NOT}		{creat_node_token(&yylval , yylineno , 2 , "NOT" , 4 , yytext , yyleng);	return NOT;}
{TYPE}		{creat_node_token(&yylval , yylineno , 1 , "TYPE" , 5 , yytext , yyleng);	return TYPE;}
{LP}		{creat_node_token(&yylval , yylineno , 2 , "LP" , 3 , yytext , yyleng);		return LP;}
{RP}		{creat_node_token(&yylval , yylineno , 2 , "RP" , 3 , yytext , yyleng);		return RP;}
{LB}		{creat_node_token(&yylval , yylineno , 2 , "LB" , 3 , yytext , yyleng);		return LB;}
{RB}		{creat_node_token(&yylval , yylineno , 2 , "RB" , 3 , yytext , yyleng);		return RB;}
{LC}		{creat_node_token(&yylval , yylineno , 2 , "LC" , 3 , yytext , yyleng);		return LC;}
{RC}		{creat_node_token(&yylval , yylineno , 2 , "RC" , 3 , yytext , yyleng);		return RC;}
{STRUCT}	{creat_node_token(&yylval , yylineno , 2 , "STRUCT" , 7 , yytext , yyleng);	return STRUCT;}
{RETURN}	{creat_node_token(&yylval , yylineno , 2 , "RETURN" , 7 ,yytext , yyleng);	return RETURN;}
{IF}		{creat_node_token(&yylval , yylineno , 2 , "IF" , 3 , yytext , yyleng);		return IF;}
{ELSE}		{creat_node_token(&yylval , yylineno , 2 , "ELSE" , 5 , yytext , yyleng);	return ELSE;}
{WHILE}		{creat_node_token(&yylval , yylineno , 2 , "WHILE" , 6 , yytext , yyleng);	return WHILE;}
{INT}		{creat_node_token(&yylval , yylineno , 1 , "INT" , 4 , yytext , yyleng);	return INT;}
{FLOAT}		{creat_node_token(&yylval , yylineno , 1 , "FLOAT" , 6 , yytext , yyleng);	return FLOAT;}
{ID}		{creat_node_token(&yylval , yylineno , 1 , "ID" , 3 , yytext , yyleng);		return ID;}
\n		{/*printf("\\n \n");yycolumn = 1;*/}
\r		{}
{WHITESPACE}	{/*printf("WHITESPACE: %s\t" , yytext);*/}

<<<<<<< HEAD
. 		{fprintf(stderr , "Error type A at Line %d: Mysterious characters %s\n" , yylineno , yytext); error_num ++;}
=======
. 		{printf("Error type A at Line %d: Mysterious characters %s\n" , yylineno , yytext); error_num ++;}
>>>>>>> origin/master
%%
void creat_node_token(YYSTYPE* yylval , int yylineno , int type , char *token_name , int token_length , char* yytext , int yyleng)
{
	yylval->pNode = (struct tree_node*)malloc(sizeof(struct tree_node));
	yylval->pNode->lineno = yylineno;
	yylval->pNode->type = type;
	yylval->pNode->token_name = (char*)malloc(token_length);
	strcpy(yylval->pNode->token_name , token_name);

	yylval->pNode->unit_name = (char*)malloc(yyleng + 1);
	strcpy(yylval->pNode->unit_name , yytext);
	yylval->pNode->children = NULL;
	yylval->pNode->children_num  = 0;
//	printf("%s: %s\t" , yylval->pNode->token_name , yylval->pNode->unit_name);
}
