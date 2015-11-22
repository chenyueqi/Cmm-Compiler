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

void outputlable(struct InterCodes* , FILE* des);

void outputfunction(struct InterCodes* , FILE* des);

void outputalop(struct InterCodes* , FILE* des);

void outputassign(struct InterCodes* , FILE* des);

void outputgoto(struct InterCodes* , FILE* des);

void outputrelopgoto(struct InterCodes* , FILE* des);

void outputreturn(struct InterCodes* , FILE* des);

void outputdec(struct InterCodes* , FILE* des);

void outputarg(struct InterCodes* , FILE* des);

void outputcallfunc(struct InterCodes* , FILE* des);

void outputparam(struct InterCodes* , FILE* des);

void outputread(struct InterCodes* , FILE* des);

void outputwrite(struct InterCodes* , FILE* des);

void translate_function(struct tree_node* );

void translate_exp(struct tree_node* , Operand);
#endif
