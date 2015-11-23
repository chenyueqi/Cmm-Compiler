#ifndef __TRAN__
#define __TRAN__

#include"common.h"
typedef struct Operand_* Operand;

struct Operand_
{
	enum {VARIABLE , CONSTANT , TEMP , ADDRESS , LABLE_SIGN , FUNC} kind;
	union
	{
		int var_no;
		int value;
		int temp_no;
		int lable_no;
		char* func_name;
	}u;
};

struct InterCode
{
	enum {LABLE , FUNCTION , ASSIGN , ADD , SUB , MUL , DIV , ADD_ASSIGN , REF_ASSIGN , ASSIGN_REF , GOTO , RELOP_GOTO  , RETURN ,DEC , ARG , CALLFUNC , PARAM , READ , WRITE } kind;
	union
	{
		struct {Operand x;} lable;
		struct {Operand f;} function;
		struct {Operand x, y, z;} alop;
		struct {Operand x , y;} assignop;
		struct {Operand x;} gotolable;
		struct {Operand x, y, z , relop;} relopgoto;
		struct {Operand x;} ret;
		struct {Operand x ,  size;} dec;
		struct {Operand x;} arg;
		struct {Operand x , f;} callfunc;
		struct {Operand x;} param;
		struct {Operand x;} read;
		struct {Operand x;} write;
	}u;
};

struct InterCodes
{
	struct InterCode code;
	struct InterCodes* prev;
	struct InterCodes* next;
};

struct InterCodes* code_head;
struct InterCodes* current_code;
int variable_num;
int temp_num;

void insertcode(struct InterCodes* );

void outputoperand(Operand);

void outputlable(struct InterCodes*);

void outputfunction(struct InterCodes*);

void outputalop(struct InterCodes*);

void outputassign(struct InterCodes*);

void outputgoto(struct InterCodes*);

void outputrelopgoto(struct InterCodes*);

void outputreturn(struct InterCodes*);

void outputdec(struct InterCodes*);

void outputarg(struct InterCodes*);

void outputcallfunc(struct InterCodes*);

void outputparam(struct InterCodes*);

void outputread(struct InterCodes*);

void outputwrite(struct InterCodes*);

void translate_function(struct tree_node* );

void translate_exp(struct tree_node* , Operand);
#endif
