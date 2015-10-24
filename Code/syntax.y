%{
	#include<stdio.h>
	#include"lex.yy.c"
	unsigned int  error_num = 0;
	extern struct tree_node* creat_node(int arity , char *token_name , struct YYLTYPE* current_pos , ...);
	extern struct tree_node* root;
%}
%locations
%error-verbose

%union{
	struct tree_node* pNode;
}

%token <pNode> INT
%token <pNode> FLOAT
%token <pNode> ID
%token <pNode> SEMI COMMA ASSIGNOP RELOP
%token <pNode> PLUS MINUS STAR DIV
%token <pNode> AND OR DOT NOT
%token <pNode> TYPE LP RP LB
%token <pNode> RB LC RC STRUCT
%token <pNode> RETURN IF ELSE WHILE


%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT UMINUS
%left LP RP LB RB LC RC DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%type <pNode> Program ExtDefList ExtDef Specifier
%type <pNode> ExtDecList FunDec CompSt VarDec
%type <pNode> StructSpecifier OptTag DefList Tag
%type <pNode> Def Stmt Exp DecList
%type <pNode> Dec VarList ParamDec StmtList
%type <pNode> Args


%%

Program : ExtDefList {$$ = creat_node(1 , "Program" , &@$ ,  $1); root = $$;}
;
ExtDefList : ExtDef ExtDefList {$$ = creat_node(2 , "ExtDefList" , &@$ ,  $1 , $2);}
| %empty {$$ = creat_node(0 , "ExtDefList" , &@$);}
;
ExtDef : Specifier ExtDecList SEMI {$$ = creat_node(3 , "ExtDef" , &@$ ,  $1 , $2 , $3);}
| Specifier SEMI {$$ = creat_node(2 , "ExtDef" , &@$ ,  $1 , $2);}
| Specifier FunDec CompSt {$$ = creat_node(3 , "ExtDef" , &@$ ,  $1 , $2 , $3);}
| error SEMI{}
;
ExtDecList : VarDec {$$ = creat_node(1 , "ExtDecList" , &@$ ,  $1);}
| VarDec COMMA ExtDecList {$$ = creat_node(3 , "ExtDecList" , &@$ ,  $1 , $2 , $3);}
| error COMMA {}
;


Specifier : TYPE {$$ = creat_node(1 , "Specifier" , &@$ ,  $1);}
| StructSpecifier {$$ = creat_node(1 , "Specifier" , &@$ ,  $1);}
;
StructSpecifier : STRUCT OptTag LC DefList RC {$$ = creat_node(5 , "StructSpecifier" , &@$ ,  $1 , $2 , $3 , $4 , $5);}
| STRUCT Tag {$$ = creat_node(2 , "StructSpecifier" , &@$ ,  $1 , $2);}
;
OptTag : ID {$$ = creat_node(1 , "OptTag" , &@$ ,  $1);}
| %empty {$$ = creat_node(0 , "OptTag" , &@$);}
;
Tag : ID {$$ = creat_node(1 , "Tag" , &@$ ,  $1);}
;


VarDec : ID {$$ = creat_node(1 , "VarDec" , &@$ ,  $1);}
| VarDec LB INT RB {$$ = creat_node(4 , "VarDec" , &@$ ,  $1 , $2 , $3 , $4);}
| error SEMI {}
;
FunDec : ID LP VarList RP {$$ = creat_node(4 , "FunDec" , &@$ ,  $1 , $2 , $3 , $4);}
| ID LP RP {$$ = creat_node(3 , "FunDec" , &@$ ,  $1 , $2 , $3);}
;
VarList : ParamDec COMMA VarList {$$ = creat_node(3 , "VarList" , &@$ ,  $1 , $2 , $3);}
| ParamDec {$$ = creat_node(1 , "VarList" , &@$ ,  $1);}
;
ParamDec : Specifier VarDec {$$ = creat_node(2 , "ParamDec" , &@$ ,  $1 , $2);}
;


CompSt : LC DefList StmtList RC {$$ = creat_node(4 , "CompSt" , &@$ ,  $1 , $2 , $3 , $4);}
| error RC {}
;
StmtList : Stmt StmtList {$$ = creat_node(2 , "StmtList" , &@$ ,  $1 , $2);}
| %empty {$$ = creat_node(0 , "StmtList" , &@$);}
;
Stmt : Exp SEMI {$$ = creat_node(2 , "Stmt" , &@$ ,  $1 , $2);}
| CompSt {$$ = creat_node(1 , "stmt" , &@$ , $1);}
| RETURN Exp SEMI {$$ = creat_node(3 , "stmt" , &@$ , $1 , $2 , $3);}
| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$ = creat_node(5 , "stmt" , &@$ , $1 , $2 , $3 , $4 , $5);}
| IF LP Exp RP Stmt ELSE Stmt {$$ = creat_node(7 , "stmt" , &@$ , $1 , $2 , $3 , $4 , $5 , $6 , $7);}
| WHILE LP Exp RP Stmt {$$ = creat_node(5 , "stmt" , &@$ , $1 , $2 , $3 , $4 , $5);}
| error SEMI {}
;


DefList : Def DefList {$$ = creat_node(2 , "DefList" , &@$ ,  $1 , $2);}
| %empty {$$ = creat_node(0 , "DefList" , &@$);}
;
Def : Specifier DecList SEMI {$$ = creat_node(3 , "Def" , &@$ ,  $1 , $2 , $3);}
;
DecList : Dec {$$ = creat_node(1 , "DecList" , &@$ , $1);}
| Dec COMMA DecList {$$ = creat_node(3 , "DecList" , &@$ ,  $1 , $2 , $3);}
| error COMMA {}
;
Dec : VarDec {$$ = creat_node(1 , "Dec" , &@$ ,  $1);}
| VarDec ASSIGNOP Exp {$$ = creat_node(3 , "Dec" , &@$ ,  $1 , $2 , $3);}
;


Exp : Exp ASSIGNOP Exp {$$ = creat_node(3 , "Exp" , &@$ ,  $1 , $2 , $3);}
| Exp AND Exp {$$ = creat_node(3 , "Exp" , &@$ ,  $1 , $2 , $3);}
| Exp OR Exp {$$ = creat_node(3 , "Exp" , &@$ ,  $1 , $2 , $3);}
| Exp RELOP Exp {$$ = creat_node(3 , "Exp" , &@$ ,  $1 , $2 , $3);}
| Exp PLUS Exp {$$ = creat_node(3 , "Exp" , &@$ ,  $1 , $2 , $3);}
| Exp MINUS Exp {$$ = creat_node(3 , "Exp" , &@$ ,  $1 , $2 , $3);}
| Exp STAR Exp {$$ = creat_node(3 , "Exp" , &@$ ,  $1 , $2 , $3);}
| Exp DIV Exp {$$ = creat_node(3 , "Exp" , &@$ ,  $1 , $2 , $3);}
| LP Exp RP {$$ = creat_node(3 , "Exp" , &@$ ,  $1 , $2 , $3);}	
| MINUS Exp %prec UMINUS {$$ = creat_node(2 , "Exp" , &@$ ,  $1 , $2);}		
| ID LP Args RP {$$ = creat_node(4 , "Exp" , &@$ ,  $1 , $2 , $3 , $4);}
| ID LP RP {$$ = creat_node(3 , "Exp" , &@$ ,  $1 , $2 , $3);}
| Exp LB Exp RB {$$ = creat_node(4 , "Exp" , &@$ ,  $1 , $2 , $3 , $4);}
| Exp DOT ID {$$ = creat_node(3 , "Exp" , &@$ ,  $1 , $2 , $3);}
| ID {$$ = creat_node(1 , "Exp" , &@$ ,  $1);}
| INT {$$ = creat_node(1 , "Exp" , &@$ ,  $1);}
| FLOAT {$$ = creat_node(1 , "Exp" , &@$ ,  $1);}
;

Args : Exp COMMA Args {$$ = creat_node(3 , "Exp" , &@$ ,  $1 , $2 , $3);}	
| Exp {$$ = creat_node(1 , "Exp" , &@$ ,  $1);}
;

%%
yyerror(char *msg)
{
	error_num++;
	fprintf(stderr , "Error type B at Line %d : near '%s' %s\n" , yylineno ,  yytext , msg);
}

