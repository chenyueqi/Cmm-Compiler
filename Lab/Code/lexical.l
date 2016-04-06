%{
    #include "common.h"
    #include "syntax.tab.h"
    extern bool error_flag;

    int yycolumn = 1;

    #define YY_USER_ACTION { \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng; \
    }

    void create_token(char *str, int type, int lineno);
%}

%option yylineno

SEMI        ;
COMMA       ,
ASSIGNOP    =
RELOP       >|<|>=|<=|==|!=
PLUS        \+
MINUS       -
STAR        \*
DIV         \/
AND         &&
OR          \|\|
DOT         \.
NOT         !
TYPE        int|float
LP          \(
RP          \)
LB          \[
RB          \]
LC          \{
RC          \}
STRUCT      struct 
RETURN      return 
IF          if
ELSE        else
WHILE       while

ID          [_a-zA-Z][_a-zA-Z0-9]*

digit       [0-9]
digits      {digit}+
radix       ({digits}\.{digit}?)|({digit}?\.{digits})

INT         {digits}
FLOAT       ({digits}\.{digits})|({radix}[E|e][+-]?{digits}) 

BLANK       [\n\t\r ]+
%%
\n          {yycolumn = 1;}

{SEMI}          {
    create_token("SEMI",2,yylineno);
    return SEMI;}
{COMMA}         {
    create_token("COMMA",2,yylineno);
    return COMMA;}
{ASSIGNOP}      {
    create_token("ASSIGNOP",2,yylineno);
    return ASSIGNOP;}
{RELOP}         {
    create_token("RELOP",2,yylineno);
    return PLUS;}
{PLUS}         {
    create_token("PLUS",2,yylineno);
    return PLUS;}
{MINUS}         {
    create_token("MINUS",2,yylineno);
    return MINUS;}
{STAR}          {
    create_token("STAR",2,yylineno);
    return STAR;}
{DIV}           {
    create_token("DIV",2,yylineno);
    return DIV;}
{AND}           {
    create_token("AND",2,yylineno);
    return AND;}
{OR}            {
    create_token("OR",2,yylineno);
    return OR;}
{DOT}           {
    create_token("DOT",2,yylineno);
    return DOT;}
{NOT}           {
    create_token("NOT",2,yylineno);
    return NOT;}
{LP}            {
    create_token("LP",2,yylineno);
    return LP;}
{RP}            {
    create_token("RP",2,yylineno);
    return RP;}
{LB}            {
    create_token("LB",2,yylineno);
    return LB;}
{RB}            {
    create_token("RB",2,yylineno);
    return RB;}
{LC}            {
    create_token("LC",2,yylineno);
    return LC;}
{RC}            {
    create_token("RC",2,yylineno);
    return RC;}
{STRUCT}        {
    create_token("STRUCT",2,yylineno);
    return STRUCT;}
{RETURN}        {
    create_token("RETURN",2,yylineno);
    return RETURN;}
{IF}            {
    create_token("IF",2,yylineno);
    return IF;}
{ELSE}          {
    create_token("ELSE",2,yylineno);
    return ELSE;}
{WHILE}         {
    create_token("WHILE",2,yylineno);
    return WHILE;}

{TYPE}          {
    AstNode *p = create_node("TYPE",3,yylineno);
    if(!strcmp(yytext,"int"))
    {
        p->strval = malloc(4);
        strcpy(p->strval,yytext);
    }
    else if(!strcmp(yytext,"float"))
    {
        p->strval = malloc(6);
        strcpy(p->strval,yytext);
    }
    yylval.ast = p;
    return TYPE;}
{INT}           {
    AstNode *p = create_node("INT",3,yylineno);
    p->intval = atoi(yytext);
    yylval.ast = p;
    return INT;
    printf("TYPE\n");}
{ID}            {
    AstNode *p = create_node("ID",3,yylineno);
    p->strval = malloc(yyleng+1);
    strcpy(p->strval,yytext);
    yylval.ast = p;
    return ID;}
{FLOAT}         {
    AstNode *p = create_node("FLOAT",3,yylineno);
    p->floval = atof(yytext);
    yylval.ast = p;
    return FLOAT;}

{BLANK} {}

.               {
    error_flag = true;
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);}
%%
void create_token(char *str, int type, int lineno)
{
    AstNode *p = create_node(str,type, lineno);
    yylval.ast = p;
}
