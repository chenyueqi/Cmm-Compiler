%{
    #include "common.h"
    #include "lex.yy.c"

    #define YYERROR_VERBOSE 1
    extern AstNode* root;
    extern bool error_flag;
%}
%union {
    AstNode *ast;
}

%token <ast> INT TYPE
%token <ast> FLOAT
%token <ast> ID SEMI COMMA ASSIGNOP RELOP
%token <ast> PLUS MINUS STAR DIV
%token <ast> AND OR DOT NOT
%token <ast> LP RP LB RB LC RC
%token <ast> STRUCT RETURN IF ELSE WHILE 

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NEG NOT 
%left LP RP LB RB DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%type <ast> Program ExtDefList ExtDef Specifier 
%type <ast> ExtDecList FunDec CompSt VarDec 
%type <ast> StructSpecifier OptTag Tag VarList 
%type <ast> ParamDec StmtList Stmt Exp DefList 
%type <ast> DecList Dec Def Args

%%
Program : ExtDefList {
            $$ = create_node("Program",1,@$.first_line);
            root = $$;
            Link_tree($$,$1);
        }
        ;

ExtDefList : ExtDef ExtDefList {
            $$ = create_node("ExtDefList", 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            }
            |   {
              $$ = NULL;
            }
            ;
ExtDef : Specifier ExtDecList SEMI{
            $$ = create_node("ExtDef",1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
       }
       | Specifier SEMI{
            $$ = create_node("ExtDef",1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
       }
       | Specifier FunDec CompSt{
            $$ = create_node("ExtDef",1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
        }
       | Specifier FunDec SEMI{
            $$ = create_node("ExtDef",1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
        }
        | error SEMI {}
        ;
ExtDecList : VarDec {
            $$ = create_node("ExtDecList", 1, @$.first_line);  
            Link_tree($$,$1);
        }
        | VarDec COMMA ExtDecList {
            $$ = create_node("ExtDecList", 1, @$.first_line);  
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
        }
        ;
Specifier : TYPE {
            $$ = create_node("Specifier",1, @$.first_line);
            Link_tree($$,$1);
          }
          | StructSpecifier {
            $$ = create_node("Specifier",1 , @$.first_line);
            Link_tree($$,$1);
          }
          ;
StructSpecifier : STRUCT OptTag LC DefList RC {
            $$ = create_node("StructSpecifier",1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
            Link_tree($$,$4);
            Link_tree($$,$5);
        }
        |   STRUCT Tag {
            $$ = create_node("StructSpecifier",1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
        }
        ;
OptTag : ID {
       $$ = create_node("OptTag",1, @$.first_line);
       Link_tree($$,$1);
       }
       | {
       $$ = NULL;
       }
       ;
Tag : ID {
        $$ = create_node("Tag", 1, @$.first_line);
        Link_tree($$,$1);
    }
    ;

VarDec: ID {
      $$ = create_node("VarDec",1, @$.first_line);
      Link_tree($$,$1);
      }
      | VarDec LB INT RB {
      $$ = create_node("VarDec",1, @$.first_line);
      Link_tree($$,$1);
      Link_tree($$,$2);
      Link_tree($$,$3);
      Link_tree($$,$4);
      }
      ;

FunDec : ID LP VarList RP{
       $$ = create_node("FunDec", 1, @$.first_line);
       Link_tree($$,$1);
       Link_tree($$,$2);
       Link_tree($$,$3);
       Link_tree($$,$4);
       }
       | ID LP RP {
       $$ = create_node("FunDec", 1, @$.first_line);
       Link_tree($$,$1);
       Link_tree($$,$2);
       Link_tree($$,$3);
       }
       | error RP {}
       ;

VarList : ParamDec COMMA VarList {
            $$ = create_node("VarList", 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
        }
        | ParamDec {
            $$ = create_node("VarList", 1, @$.first_line);
            Link_tree($$,$1);
        }
        ;

ParamDec : Specifier VarDec {
            $$ = create_node("ParamDec",1 ,@$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
         }
         ;

CompSt : LC DefList StmtList RC {
            $$ = create_node("CompSt",1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
            Link_tree($$,$4);
       }
       | error RC {}
       ;

StmtList : Stmt StmtList {
            $$ = create_node("StmtList",1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
         }
         | {
            $$ = NULL;
         }
         ;

Stmt : Exp SEMI {
            $$ = create_node("Stmt",1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
        }
        | CompSt {
            $$ = create_node("Stmt",1, @$.first_line);
            Link_tree($$,$1);
        }
        | RETURN Exp SEMI {
            $$ = create_node("Stmt",1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
        }
        | IF LP Exp RP Stmt    %prec LOWER_THAN_ELSE {
            $$ = create_node("Stmt",1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
            Link_tree($$,$4);
            Link_tree($$,$5);
        }
        | IF LP Exp RP Stmt ELSE Stmt {
            $$ = create_node("Stmt",1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
            Link_tree($$,$4);
            Link_tree($$,$5);
            Link_tree($$,$6);
            Link_tree($$,$7);
        }
        | WHILE LP Exp RP Stmt {
            $$ = create_node("Stmt",1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
            Link_tree($$,$4);
            Link_tree($$,$5);
        }
        | error RP {}
        | error SEMI{}
        ;
DefList : Def DefList {
            $$ = create_node("DefList",1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
        }
        | {
            $$ = NULL;
        }
        ;

Def : Specifier DecList SEMI {
            $$ = create_node("Def",1,@$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
    }
    ;

DecList : Dec {
            $$ = create_node("DecList", 1, @$.first_line);
            Link_tree($$,$1);
        }
        | Dec COMMA DecList {
            $$ = create_node("DecList", 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
        }
        ;

Dec : VarDec {
            $$ = create_node("Dec",1 , @$.first_line);
            Link_tree($$,$1);
    }
    | VarDec ASSIGNOP Exp {
            $$ = create_node("Dec",1 , @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
    }
    ;

Exp : Exp ASSIGNOP Exp {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
    }
    | Exp AND Exp {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
    }
    | Exp OR Exp {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
    }
    | Exp RELOP Exp {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
    }
    | Exp PLUS Exp {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
    }
    | Exp MINUS Exp {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
    }
    | Exp STAR Exp {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
    }
    | Exp DIV Exp {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
    }
    | LP Exp RP {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
    }
    | MINUS Exp %prec NEG {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
    }
    | NOT Exp {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
    }
    | ID LP Args RP {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
            Link_tree($$,$4);
    }
    | ID LP RP {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
    }
    | Exp LB Exp RB {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
            Link_tree($$,$4);
    }
    | Exp DOT ID {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
    }
    | ID {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
    }
    | INT {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
    }
    | FLOAT {
            $$ = create_node("Exp" , 1, @$.first_line);
            Link_tree($$,$1);
    }
    ;
Args : Exp COMMA Args {
            $$ = create_node("Args",1,@$.first_line);
            Link_tree($$,$1);
            Link_tree($$,$2);
            Link_tree($$,$3);
     }
     | Exp {
            $$ = create_node("Args",1,@$.first_line);
            Link_tree($$,$1);
     }
     ;

%%
yyerror(char *msg)
{
    error_flag = true;
    fprintf(stderr, "Error type B at Line %d : %s, near '%s'\n", yylineno, msg, yytext);
}

